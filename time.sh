ts=$(date +%s%N); 
./hw5.exe -c=chunks3 -s=sizes4;
tt=$((($(date +%s%N) - $ts)/1000000));
echo "Time taken: $tt milliseconds"