nginx -c $(pwd)/nginx.conf -p $(pwd)
time wget https://127.0.0.1:4043 -O /dev/null --no-check-certificate --no-proxy
nginx -c $(pwd)/nginx.conf -s stop
