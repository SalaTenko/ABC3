#!/bin/bash

echo "MemoryType;BlockSize;ElementType;BufferSize;LaunchNum;Timer;WriteTime;AverageWriteTime;WriteBandwidth;\
AbsError(write);RelError(write);ReadTime;AverageReadTime;ReadBandwidth;AbsError(read);RelError(read);" > ../data/output.csv

mem_size=4

cd ../source
make

for (( i=1; i <= 20; i++ ))
do
./../source/main -l 1 -m SSD -b $mem_size"Mb" --maximum-buffer
mem_size=$(( $mem_size + 4))
done

#mem_size=4
#for (( i=1; i <= 20; i++ ))
#do
#./../source/main -l 1 -m flash -b $mem_size"Mb" --maximum-buffer
#mem_size=$(( $mem_size + 4))
#done
