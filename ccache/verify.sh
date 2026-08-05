#!/bin/bash
set -e

CACHE_DIR=$(pwd)/.ccache-cache
rm -rf "$CACHE_DIR" main.o

echo "╔══════════════════════════════════════════════════════╗"
echo "║          ccache 原理验证                             ║"
echo "╚══════════════════════════════════════════════════════╝"
echo ""

# ---------- 1 ----------
CCACHE_DIR=$CACHE_DIR CCACHE_NOHASHDIR=1 ccache -z > /dev/null 2>&1
echo "1. 首次编译 (cache miss)"
T1=$({ time CCACHE_DIR=$CACHE_DIR CCACHE_NOHASHDIR=1 ccache g++ -std=c++17 -c main.cpp -o main.o 2>&3; } 3>&2 2>&1)
echo "   耗时: $(echo "$T1" | grep real | awk '{print $2}')"

# ---------- 2 ----------
echo "2. 相同代码 (cache hit)"
T2=$({ time CCACHE_DIR=$CACHE_DIR CCACHE_NOHASHDIR=1 ccache g++ -std=c++17 -c main.cpp -o main.o 2>&3; } 3>&2 2>&1)
echo "   耗时: $(echo "$T2" | grep real | awk '{print $2}')"

# ---------- 3 ----------
sed -i '' 's/x = 10/x = 20/' main.cpp
echo "3. 修改源文件 (cache miss)"
T3=$({ time CCACHE_DIR=$CACHE_DIR CCACHE_NOHASHDIR=1 \
        ccache g++ -std=c++17 -c main.cpp -o main.o 2>&3; } 3>&2 2>&1)
echo "   耗时: $(echo "$T3" | grep real | awk '{print $2}')"
sed -i '' 's/x = 20/x = 10/' main.cpp

# ---------- 4 ----------
sed -i '' 's/(n <= 1)/(n < 1)/' math_util.h
echo "4. 修改头文件 (cache miss)"
T4=$({ time CCACHE_DIR=$CACHE_DIR CCACHE_NOHASHDIR=1 \
        ccache g++ -std=c++17 -c main.cpp -o main.o 2>&3; } 3>&2 2>&1)
echo "   耗时: $(echo "$T4" | grep real | awk '{print $2}')"
sed -i '' 's/(n < 1)/(n <= 1)/' math_util.h

# ---------- 5 ----------
echo "5. 修改编译选项 -O2 (cache miss)"
T5=$({ time CCACHE_DIR=$CACHE_DIR CCACHE_NOHASHDIR=1 \
        ccache g++ -std=c++17 -O2 -c main.cpp -o main.o 2>&3; } 3>&2 2>&1)
echo "   耗时: $(echo "$T5" | grep real | awk '{print $2}')"

# ---------- 6 ----------
echo "6. 相同 -O2 选项 (cache hit)"
T6=$({ time CCACHE_DIR=$CACHE_DIR CCACHE_NOHASHDIR=1 \
        ccache g++ -std=c++17 -O2 -c main.cpp -o main.o 2>&3; } 3>&2 2>&1)
echo "   耗时: $(echo "$T6" | grep real | awk '{print $2}')"

echo ""
echo "╔══════════════════════════════════════════════════════╗"
echo "║  统计信息                                           ║"
echo "╚══════════════════════════════════════════════════════╝"
CCACHE_DIR=$CACHE_DIR ccache -s | grep -E "Hits:|Misses:|Direct:|Preprocessed:|Cache size|Cacheable"

rm -rf "$CACHE_DIR" main.o
echo ""
echo "清理完成"
