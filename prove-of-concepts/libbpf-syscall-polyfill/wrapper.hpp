#ifndef _WRAPPER_HPP
#define _WRAPPER_HPP

#include <cassert>
#include <cerrno>
#include <cinttypes>
#include <cstddef>
#include <functional>
#include <mutex>
#include <vector>
#include <unordered_map>
#include <memory>
#include <ebpf-core.h>
#include <linux/bpf.h>
#include "helpers_impl.hpp"
#include <variant>
#include <iostream>
#include <deque>
#include <functional>
#include <unordered_set>
struct VectorHash {
	size_t operator()(const std::vector<uint8_t> &v) const
	{
		std::hash<uint8_t> hasher;
		size_t seed = 0;
		for (auto &i : v) {
			seed ^= hasher(i) + 0x9e3779b9 + (seed << 6) +
				(seed >> 2);
		}
		return seed;
	}
};

struct EbpfProgWrapper {
	std::unique_ptr<ebpf_vm, decltype(&ebpf_destroy)> vm;
	EbpfProgWrapper() : vm(ebpf_create(), ebpf_destroy)
	{
		inject_helpers(vm.get());
		ebpf_set_lddw_helpers(vm.get(), map_ptr_by_fd, nullptr, map_val,
				      nullptr, nullptr);
	}
};

using HashMapImpl = std::unordered_map<std::vector<uint8_t>,
				       std::vector<uint8_t>, VectorHash>;
using ArrayMapImpl = std::vector<std::vector<uint8_t> >;

struct RingBuffer {
	uint32_t max_ent;
	std::mutex buf_lock;
	std::deque<std::vector<uint8_t> > buf;
	std::mutex reserve_lock;
	std::unordered_set<int32_t *> reserved_buf;
	~RingBuffer()
	{
		for (auto v : reserved_buf)
			delete[] v;
	}
	void *reserve(size_t size, int self_fd)
	{
		std::lock_guard<std::mutex> _guard(reserve_lock);
		// Align for 4bytes, so we can safely store self_fd and buffer
		// size
		auto buf = new int32_t[(size + 3) / 4 + 2];
		buf[0] = self_fd;
		buf[1] = (int32_t)(size);
		reserved_buf.insert(buf);
		// Offset for 4 bytes. In this way we can get the ringbuf fd in
		// bpf_ringbuf_submit
		return buf + 2;
	}
	void submit(const void *buf)
	{
		std::lock_guard<std::mutex> _guard(reserve_lock);

		auto data = (int32_t *)buf;
		auto itr = reserved_buf.find(data - 2);
		assert(itr != reserved_buf.end());
		auto len = data[-1];
		std::vector<uint8_t> data_store((uint8_t *)data,
						(uint8_t *)data + len);
		reserved_buf.erase(itr);
		this->appendElem(std::move(data_store));
	}
	void appendElem(std::vector<uint8_t> &&data)
	{
		std::lock_guard<std::mutex> _guard(buf_lock);
		this->buf.push_back(data);
		if (this->buf.size() > max_ent)
			this->buf.pop_front();
	}
	void
	extractElem(std::function<void(const std::vector<uint8_t> &)> callback)
	{
		std::lock_guard<std::mutex> _guard(buf_lock);
		if (!buf.empty()) {
			callback(buf.front());
			buf.pop_front();
		}
	}
};

using RingBufMapImpl = std::unique_ptr<RingBuffer>;

struct EbpfMapWrapper {
	enum bpf_map_type type;
	uint32_t key_size;
	uint32_t value_size;
	uint32_t max_entries;
	uint64_t flags;
	std::string name;
	std::variant<HashMapImpl, ArrayMapImpl, RingBufMapImpl> impl;
	bool frozen = false;
	EbpfMapWrapper(const EbpfMapWrapper &) = delete;
	EbpfMapWrapper(EbpfMapWrapper &&) noexcept = default;
	EbpfMapWrapper &operator=(const EbpfMapWrapper &) = delete;
	EbpfMapWrapper &operator=(EbpfMapWrapper &&) noexcept = default;
	int mapDelete(const void *key)
	{
		if (type == BPF_MAP_TYPE_HASH) {
			HashMapImpl &map = std::get<HashMapImpl>(impl);
			std::vector<uint8_t> key_vec((uint8_t *)key,
						     (uint8_t *)key + key_size);
			map.erase(key_vec);
			return 0;
		} else if (type == BPF_MAP_TYPE_ARRAY) {
			return -ENOTSUP;
		} else {
			assert(false);
		}
	}
	const void *mapLookup(const void *key)
	{
		if (type == BPF_MAP_TYPE_HASH) {
			HashMapImpl &map = std::get<HashMapImpl>(impl);
			std::vector<uint8_t> key_vec((uint8_t *)key,
						     (uint8_t *)key + key_size);
			if (auto itr = map.find(key_vec); itr != map.end()) {
				return (const void *)&itr->second[0];
			} else {
				return nullptr;
			}

		} else if (type == BPF_MAP_TYPE_ARRAY) {
			auto key_val = *(uint32_t *)key;

			ArrayMapImpl &map = std::get<ArrayMapImpl>(impl);
			if (key_val >= map.size())
				return nullptr;
			return (const void *)&map.at(key_val)[0];
		} else {
			assert(false);
		}
	}
	int mapUpdate(const void *key, const void *value, uint64_t flags)
	{
		if (frozen) {
			return -EINVAL;
		}
		if (type == BPF_MAP_TYPE_HASH) {
			HashMapImpl &map = std::get<HashMapImpl>(impl);
			std::vector<uint8_t> key_vec((uint8_t *)key,
						     (uint8_t *)key + key_size);
			std::vector<uint8_t> value_vec((uint8_t *)value,
						       (uint8_t *)value +
							       value_size);
			map[key_vec] = value_vec;

		} else if (type == BPF_MAP_TYPE_ARRAY) {
			auto key_val = *(uint32_t *)key;

			std::vector<uint8_t> value_vec((uint8_t *)value,
						       (uint8_t *)value +
							       value_size);
			ArrayMapImpl &map = std::get<ArrayMapImpl>(impl);
			if (key_val >= map.size())
				return -E2BIG;
			map[key_val] = value_vec;
		}
		return 0;
	}
	const void *first_value_addr()
	{
		if (type == BPF_MAP_TYPE_HASH) {
			HashMapImpl &map = std::get<HashMapImpl>(impl);
			if (map.empty())
				return nullptr;
			return (const void *)&map.begin()->second[0];

		} else if (type == BPF_MAP_TYPE_ARRAY) {
			ArrayMapImpl &map = std::get<ArrayMapImpl>(impl);
			return (const void *)&map.at(0)[0];
		} else {
			assert(false);
		}
	}
	EbpfMapWrapper(enum bpf_map_type type, uint32_t key_size,
		       uint32_t value_size, uint32_t max_ent, uint64_t flags,
		       std::string name)
		: type(type), key_size(key_size), value_size(value_size),
		  max_entries(max_ent), flags(flags)

	{
		switch (type) {
		case BPF_MAP_TYPE_HASH: {
			impl = std::unordered_map<std::vector<uint8_t>,
						  std::vector<uint8_t>,
						  VectorHash>();
			break;
		}
		case BPF_MAP_TYPE_ARRAY: {
			impl = std::vector<std::vector<uint8_t> >(
				max_entries,
				std::vector<uint8_t>(value_size, 0));
			break;
		}
		case BPF_MAP_TYPE_RINGBUF: {
			std::cerr << "Creating ringbuf map `" << name << "`"
				  << std::endl;
			impl = std::make_unique<RingBuffer>();
			break;
		}
		default: {
			std::cerr << "Unsupported map type " << type
				  << std::endl;
			exit(1);
		}
		}
	}
};
struct PerfEventWrapper {};
struct BpfLinkWrapper {
	uint32_t prog_fd, target_fd;
	BpfLinkWrapper(uint32_t prog_fd, uint32_t target_fd)
		: prog_fd(prog_fd), target_fd(target_fd)
	{
	}
};
using EbpfObj = std::variant<EbpfProgWrapper, EbpfMapWrapper, PerfEventWrapper,
			     BpfLinkWrapper>;

static const char *name_mapping[] = { "Program", "Map", "PerfEvent",
				      "BpfLink" };

#endif
