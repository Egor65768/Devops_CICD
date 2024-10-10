#!/bin/bash

SUCCESS=0
FAIL=0
ALL=0
DIFF=""
STATUS=""

test() {
    peremen=$(echo "$@" | sed "s/OPTIONS/$var/")   
    ./s21_cat $peremen > ./s21_cat.log
    cat $peremen > cat.log
    DIFF=$(diff -s ./s21_cat.log cat.log)
    let "ALL++"
    if [ "$DIFF" == "Files ./s21_cat.log and cat.log are identical" ]
    then
        STATUS="SUCCESS"
        let "SUCCESS++"
    else
        STATUS="FAIL"
        let "FAIL++"
    fi
    echo "Test status: $STATUS. Fail:$FAIL Success:$SUCCESS All:$ALL Request:cat $peremen"
    rm -f ./s21_cat.log cat.log
}  

test1() {
    for i in "${proverka[@]}"
    do
        test "$i"
    done
} 

tests=(
"OPTIONS test/test_12.txt"
"OPTIONS test/test_12.txt test/test_2.txt"
"OPTIONS test/test_12.txt test/test_2.txt test/test_3.txt"
)
flags=(
    "b"
    "e"
    "n"
    "s"
    "t"
    "v"
)
proverka=(
"-b -e -n test/test_12.txt"
"test/test_1.txt test/test_4.txt"
"-e test/test_1.txt"
"-e -t test/test_1.txt test/test_4.txt"
"-b -n test/test_2.txt"
"test/test_2.txt test/test_4.txt"
"-s test/test_3.txt"
"-s test/test_3.txt test/test_4.txt"
"-v test/test_5.txt"
"-- test/test_5.txt"
"test/test_4.txt"
"-n -b test/test_1.txt"
"-s -n -e test/test_4.txt"
"-b -e -n -s -t -v test/test_3.txt"
"-benstv test/test_3.txt"
)

echo "\nTEST\n"
echo "\nTEST 1\n"
test1
printf "\n"
echo "FINAL RESULT: FAILED: $FAIL SUCCESS: $SUCCESS ALL: $ALL"
printf "\n"
