#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFFERENS=" "

declare tests=(
"FLAG re tests/test_0_grep.txt"
"FLAG for s21_grep.c s21_grep.h Makefile"
"FLAG for s21_grep.c"
"FLAG for s21_grep.c s21_grep.h Makefile"
"FLAG nt s21_grep.c "
"FLAG regex s21_grep.c"
"FLAG while s21_grep.c Makefile"
"FLAG text  tests/test_7_grep.txt"
"FLAG int tests/test_7_grep.txt"
)

declare extra=(
"-k"
"-s 2 tests/test_1_grep.txt"
"-o [o tests/test_3_grep.txt"
"program tests/test_2_grep.txt"
"-s j tests/file.txt"
"-n FAIL tests/test_1_grep.txt"
"-sne int tests/test_4_grep.txt"
"-n echo tests/test_1_grep.txt"
"-n 21 tests/test_1_grep.txt"
"-on for tests/test_1_grep.txt tests/test_2_grep.txt"
"-vg tests/test_5_grep.txt"
)

testing()
{
    t=$(echo $@ | sed "s/FLAG/$var/")  #  @ все аргументы переданыне скрипту, sed заменяет FLAG тем что находится в $var
    ./s21_grep $t > test_s21_grep.log
    grep $t > test_sys_grep.log
    DIFFERENS="$(diff -s test_s21_grep.log test_sys_grep.log)"  #  diff сравнивает файлы, флаг -s для вывода текста что они схожи
    (( COUNTER++ ))
    if [ "$DIFFERENS" == "Files test_s21_grep.log and test_sys_grep.log are identical" ]  # вот эту надпись выдает -s если схожи
    then
      (( SUCCESS++ ))
      printf "SUCCESS  "
    else
      (( FAIL++ ))
      printf "FAIL  "
    fi
    echo "fail = $FAIL / succes = $SUCCESS ( $COUNTER ) grep $t"
}

# придуманые тесты
for i in "${extra[@]}"
do
    testing $i
done

printf "\n"
echo "-- 1 --"
printf "\n"

# 1 параметр
for var1 in i v c l n h s o
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done

printf "\n"
echo "-- 2 --"
printf "\n"

rm test_s21_grep.log test_sys_grep.log

echo "FAIL: $FAIL"
echo "SUCCESS: $SUCCESS"
echo "ALL: $COUNTER"

if [ $FAIL -ne 0 ];
then
    exit 1
else
    exit 0
fi