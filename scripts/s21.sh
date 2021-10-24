#!/bin/bash

echo "MemoryType;BlockSize;ElementType;BufferSize;LaunchNum;Timer;WriteTime;AverageWriteTime;WriteBandwidth;\
AbsError(write);RelError(write);ReadTime;AverageReadTime;ReadBandwidth;AbsError(read);RelError(read);" > ../data/output.csv

cache_line=$(getconf LEVEL1_DCACHE_LINESIZE)
cache_lvl1=$(getconf LEVEL1_DCACHE_SIZE)
cache_lvl2=$(getconf LEVEL2_CACHE_SIZE)
cache_lvl3=$(getconf LEVEL3_CACHE_SIZE)
cache_more=$(($cache_lvl3 + $cache_lvl2))

cd ../source
make
./../source/main --launch-count 3 --memory-type RAM --block-size $cache_line
./../source/main --launch-count 3 --memory-type RAM --block-size $cache_lvl1
./../source/main --launch-count 3 --memory-type RAM --block-size $cache_lvl2
./../source/main --launch-count 3 --memory-type RAM --block-size $cache_lvl3
./../source/main --launch-count 3 --memory-type RAM --block-size $cache_more
