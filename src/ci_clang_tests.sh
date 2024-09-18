#!bin/bash

echo "cat style"

cd ./src/cat/
style_result=$(clang-format -n -style=Google -Werror *.c)
error=$?
echo $style_result
if [ $error != 0 ]; then
    exit 1;
fi
cd ../../

echo "grep style"

cd ./src/grep/
style_result=$(clang-format -n -style=Google -Werror *.c)
error=$?
echo $style_result
if [ $error != 0 ]; then
    exit 2;
fi

echo "finish style tests"