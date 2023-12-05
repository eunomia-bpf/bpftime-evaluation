
rm -rf /test_sqlite/*


nohup ./ycsb -load -db sqlite -P workloads/workloada -P sqlite/sqlite.properties -s >> scan_result.txt 
nohup ./ycsb -run -db sqlite -P workloads/workloada -P sqlite/sqlite.properties -s  >>  scan_result.txt 
nohup ./ycsb -run -db sqlite -P workloads/workloadb -P sqlite/sqlite.properties -s  >>  scan_result.txt 
nohup ./ycsb -run -db sqlite -P workloads/workloadc -P sqlite/sqlite.properties -s  >>  scan_result.txt 
nohup ./ycsb -run -db sqlite -P workloads/workloadd -P sqlite/sqlite.properties -s  >>  scan_result.txt 
nohup ./ycsb -run -db sqlite -P workloads/workloadf -P sqlite/sqlite.properties -s  >>  scan_result.txt 
nohup ./ycsb -run -db sqlite -P workloads/workloade -P sqlite/sqlite.properties -s  >>  scan_result.txt 
