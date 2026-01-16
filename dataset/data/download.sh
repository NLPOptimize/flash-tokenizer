#!/bin/bash
rm -f data.*
ㅍ
echo "Starting dataset download..."

wget -q https://github.com/NLPOptimize/flash-tokenizer/releases/download/Dataset/data.z01 &
wget -q https://github.com/NLPOptimize/flash-tokenizer/releases/download/Dataset/data.z02 &
wget -q https://github.com/NLPOptimize/flash-tokenizer/releases/download/Dataset/data.z03 &
wget -q https://github.com/NLPOptimize/flash-tokenizer/releases/download/Dataset/data.z04 &
wget -q https://github.com/NLPOptimize/flash-tokenizer/releases/download/Dataset/data.zip &

wait

echo "Extracting the compressed file..."

rm -f texts_*.parquet

#7z x data.zip
unzip data.zip
rm -f data.*
