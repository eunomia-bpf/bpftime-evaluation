nginx -c $(pwd)/nginx.conf -p $(pwd)
nginx_pids=$(pgrep nginx)
bpftime load ./sslsniff &
sslsniff_pid=$!
sleep 3s
echo Attach started
for pid in $nginx_pids; do
    bpftime attach $pid
done
echo Attach done
elapsed_time=$( { time wget https://127.0.0.1:4043 -O /dev/null --no-check-certificate --no-proxy; } 2>&1 | awk '/real/ {print $2}' )
killall -SIGKILL nginx
kill -SIGKILL $sslsniff_pid
rm -rf nginx.pid

echo Time usage: $elapsed_time
