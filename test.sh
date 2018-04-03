#!/bin/bash

# Mateusz Dudziński
# IPP, 2018L Task: "Maraton filmowy".

# Can be overriden by the 1st and 2nd arguments. Rest of args are ingores.
PROGRAM=./marathon
DIRECTORY=./tests

# This slows down the testing script, Uses valgrind with memcheck (default) as a
# leakcheck tool.
CHECK_FOR_MEM_LEAKS=true

# The max allocation memory done by the program and the name of the test at
# which it happend.
MAX_ALLOCATED_SIZE=0
MAX_ALLOCATION_FILE=""

# Same as above, but with time.
MAX_TIME=0.0
MAX_TIME_FILE=""

PROGRAM_OUT=`mktemp`
PROGRAM_ERR=`mktemp`
PROGRAM_VALG=`mktemp`

# Script will failed tests into this file in order to print them as a summary when
# testing is finished.
WRONG_TESTS=`mktemp`

# Check the console args (program name, and default test dir). My scirpt does
# provide defaults, even thought it was not ordered in the task description.
if [ $# -ge 1 ]; then
    PROGRAM=$1
fi
if [ $# -ge 2 ]; then
    DIRECTORY=$2
fi

# Check if args are OK...
if [ ! -x $PROGRAM ]; then
    echo "$PROGRAM does not exist or is not an executable file. Exitting..."
    exit 2
    # This is a case when user enters i.eg. 'marathon' instead of './marathon',
    # because the file does exist, but the name must be modyfied to run as a
    # command.
elif ! command -v $PROGRAM &> /dev/null; then
    PROGRAM_FIXED="./"$PROGRAM
    if ! command -v $PROGRAM_FIXED &> /dev/null; then

        echo "$PROGRAM does exist but I have no \
idea how to run it as a command. Exitting..."
        exit 2
    else
        PROGRAM=$PROGRAM_FIXED
    fi
fi

if [ ! -d $DIRECTORY ]; then
    echo $DIRECTORY " does not exist. Exitting..."
    exit 2
fi

for i in ${DIRECTORY}/*.in; do
    # Alias these variables to make it more readable.
    INPUT=$i
    OUTPUT=${i%in}out
    ERR=${i%in}err

    echo -e "Doing test on file: \e[1m$INPUT\e[0m"

    # Check if correspoing .out and .err files exists.
    if [ ! -f $OUTPUT ] || [ ! -f $ERR ]; then
        echo "No corresponding files for test: $INPUT Skipping..."
        continue
    fi

    # I usedthis not unix 'time' coz i wasn't sure what will be the output of $?
    TIME_START=$(date +%s.%N)
    $PROGRAM < $INPUT > $PROGRAM_OUT 2> $PROGRAM_ERR
    PROGRAM_EXIT_CODE=$?
    TIME_END=$(date +%s.%N)

    # 'awk' is used to print trailing 0, that [bc] does not.

    TIME_DIFF=$(echo "$TIME_END - $TIME_START" | bc \
                    | awk '{printf "%f", $0}')

    # bash doesnt do great job (or I just cant use it correctly) when it comes
    # to compare floating point numbers, so I used 'bc'.
    if (( `echo "$TIME_DIFF > $MAX_TIME" | bc` )); then
        MAX_TIME="$TIME_DIFF"
        MAX_TIME_FILE=$INPUT
    fi

    # [TEST_OK] is true unless one of the tests fails.
    TEST_OK=true

    if [ "$PROGRAM_EXIT_CODE" -ne "0" ]; then
        echo -n "Program execution failed with error code: "
        echo -e "\e[1;31m$PROGRAM_EXIT_CODE\e[0m"

        TEST_OK=false
    else
        echo -e "Program execution            \e[1;32mOK\e[0m \
(Time: ${TIME_DIFF} s)"
    fi

    echo -n "Comparing standard output... "
    if diff $PROGRAM_OUT $OUTPUT &> /dev/null; then
        echo -e "\e[1;32mOK\e[0m"
    else
        echo -e "\e[1;31mDIFFERS!\e[0m"
        TEST_OK=false
    fi

    echo -n "Comparing stderr output...   "
    if diff $PROGRAM_ERR $ERR &> /dev/null; then
        echo -e "\e[1;32mOK\e[0m"
    else
        echo -e "\e[1;31mDIFFERS!\e[0m"
        TEST_OK=false
    fi

    # Unfortunetly we have to run a program again to look for memory leaks.
    if $CHECK_FOR_MEM_LEAKS; then
        echo -n "Checking for memory leaks... "

        valgrind --leak-check=full --log-file="$PROGRAM_VALG" \
                 --error-exitcode=1 $PROGRAM < $INPUT &> /dev/null
        VALGR_EXIT_CODE=$?

        USAGE_INFO=`cat $PROGRAM_VALG | grep 'total heap usage'`
        regex="==[0-9]+==\s+total heap usage: ([0-9,]+) allocs, \
([0-9,]+) frees, ([0-9,]+) bytes allocated"
        if [[ $USAGE_INFO =~ $regex ]]; then
            # [tr] is used to remove ',' from the string so i get a number.
            ALLOCATED_SIZE=`echo ${BASH_REMATCH[3]} | tr --delete ,`
            if [ $ALLOCATED_SIZE -ge $MAX_ALLOCATED_SIZE ];then
                MAX_ALLOCATED_SIZE="$ALLOCATED_SIZE"
                MAX_ALLOCATION_FILE="$INPUT"
            fi
        fi

        if [ "$VALGR_EXIT_CODE" -eq "0" ]; then
            echo -ne "\e[1;32mOK\e[0m"
            echo " (Total allocated: $ALLOCATED_SIZE B)"
        else
            echo -e "\e[1;31mLEAKS DETECTED!\e[0m"
            TEST_OK=false
        fi
    fi

    if ! $TEST_OK; then
        echo "$INPUT" >> $WRONG_TESTS
    fi
done

WRONG_TESTS_NUMBER=`wc -l < $WRONG_TESTS`

echo ""
echo "Testing done."
if [ "$WRONG_TESTS_NUMBER" -eq "0" ]; then
    echo -e "All tests \e[1;32mOK\e[0m."
else
    echo -e "\e[1;31m$WRONG_TESTS_NUMBER\e[0m tests failed:"
    cat $WRONG_TESTS
fi
echo "Max time: $MAX_TIME_FILE ($MAX_TIME s)."
if $CHECK_FOR_MEM_LEAKS; then
    echo "Max allocation at file: $MAX_ALLOCATION_FILE ($MAX_ALLOCATED_SIZE B)."
fi
# cleanup:
rm $PROGRAM_OUT
rm $PROGRAM_ERR
rm $PROGRAM_VALG
rm $WRONG_TESTS
