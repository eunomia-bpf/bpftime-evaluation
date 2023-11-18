nginx -c $(pwd)/nginx.conf -p $(pwd)
elapsed_time=$( { time wget https://127.0.0.1:4043 -O /dev/null --no-check-certificate --no-proxy; } 2>&1 | awk '/real/ {print $2}' )
nginx -c $(pwd)/nginx.conf -s stop
echo Time usage: $elapsed_time
