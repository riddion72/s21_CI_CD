#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""

declare -a tests=(
"VAR tests/test_case_cat.txt"
)

declare -a extra=(
"-s tests/test_1_cat.txt"
"-b tests/test_1_cat.txt"
"-t tests/test_3_cat.txt"
"-n tests/test_2_cat.txt"
"tests/no_file.txt"
"-n -b tests/test_1_cat.txt"
"-s -n -e tests/test_4_cat.txt"
"tests/test_1_cat.txt"
"-n tests/test_1_cat.txt"
"-n tests/test_1_cat.txt tests/test_2_cat.txt"
"-v tests/test_5_cat.txt"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./s21_cat $t > test_s21_cat.log
    cat $t > test_sys_cat.log
    DIFF_RES="$(diff -s test_s21_cat.log test_sys_cat.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_cat.log and test_sys_cat.log are identical" ]
    then
      (( SUCCESS++ ))
    else
      (( FAIL++ ))
    fi
    echo "fail = $FAIL / succes = $SUCCESS ( $COUNTER ) cat $t"
    rm test_s21_cat.log test_sys_cat.log
}

# специфические тесты
for i in "${extra[@]}"
do
    var="-"
    testing $i
done

# 1 параметр
for var1 in b e n s t v
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done

echo "FAIL: $FAIL"
echo "SUCCESS: $SUCCESS"
echo "ALL: $COUNTER"

if [ $FAIL -ne 0 ];
then
    exit 1
else
    exit 0
fi
