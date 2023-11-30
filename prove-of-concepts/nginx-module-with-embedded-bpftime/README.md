# nginx-bpftime module

A simple nginx module that could run ebpf program through bpftime when handling requests.

## How to compile

- Modify CMakelists.txt, change the nginx URL in `ExternalProject_Add`, if your nginx version doesn't match it
- Build the target `nginx` of the cmake project at root
- Run `make -j` in bpf-prog
## How to use
- Run syscall server through `bpftime load ./nginx_test` in folder `bpf-prog`
- Run nginx using `LD_LIBRARY_PATH=./build nginx -p . -c nginx.conf` in the project root
- Send requests to the server. Such as `curl http://127.0.0.1:9023` or `curl http://127.0.0.1:9023?X=y`, see if the request was forbidden
