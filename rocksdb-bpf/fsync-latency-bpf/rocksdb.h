/* SPDX-License-Identifier: (LGPL-2.1 OR BSD-2-Clause) */
#ifndef __OPENSNOOP_H
#define __OPENSNOOP_H

struct avg_latency_t {
  unsigned long long count;
   long long latency_to_write;
   long long latency_to_fsync_exit;
};

#endif /* __OPENSNOOP_H */
