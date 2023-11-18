nginx -c $(pwd)/nginx.conf -p $(pwd) -e $(pwd)/nginx-error.txt
./sslsniff &
sslsniff_pid=$!
sleep 1s
elapsed_time=$( { time wget https://127.0.0.1:4043 -O /dev/null --no-check-certificate --no-proxy; } 2>&1 | awk '/real/ {print $2}' )
kill -SIGKILL $sslsniff_pid
rm -rf nginx.pid
killall -SIGKILL nginx
echo Time usage: $elapsed_time
