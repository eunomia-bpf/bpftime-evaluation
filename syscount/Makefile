
.PHONY: test 
test: test-log.txt

test-log.txt: ../ssl-nginx/wrk/wrk
	seq 1 10 | xargs -I {} ../ssl-nginx/wrk/wrk http://127.0.0.1:801/index.html -c 512 -t 4 -d 10 >> test-log.txt



