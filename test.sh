#!/bin/bash

# Mateusz Dudziński
# IPP, 2018L Task: "Maraton filmowy".

DIRECTORY=./tests
PROGRAM=./bin/program

CHECK_FOR_MEM_LEAKS=true

printGood() {
    echo -e "\e[1;32m$1\e[0m"
}

printBad() {
    echo -e "\e[1;31m$1\e[0m"
}

yesNoConfirm() {
    if [ $# -eq 0 ]; then PROMPT="Continue?"; else PROMPT=$1; fi

    read -p "$PROMPT [Y/n]? " CONT
    if [ "$CONT" = "n" ] || [ "$CONT" = "N" ] \
           || [ "$CONT" = "no" ] || [ "$CONT" = "No" ]; then
        false
    else
        true
    fi
}

# Check if specyfied program and direcotory are valid:
if [ ! -f $PROGRAM ]; then
    echo $PROGRAM " does not exist. Exitting..."
    exit 2
fi
if [ ! -d $DIRECTORY ]; then
    echo $DIRECTORY " does not exist. Exitting..."
    exit 2
fi

for i in $(ls $DIRECTORY | egrep -i '*.in'); do

    # Alias these variables to make it more readable.
    INPUT=$DIRECTORY/$i
    OUTPUT=$DIRECTORY/${i%in}out
    ERR=$DIRECTORY/${i%in}err

    echo -e "Doing test on file: $INPUT"

    # Check if correspoing .out and .err files exists.
    if [ ! -f $OUTPUT ] || [ ! -f $ERR ]; then
        # TODO: Message about no corresponding files!
        echo -e "No corresponding files for test: $INPUT! Skipping..."
        continue
    fi

    # Scripting test makes two temporary files my_out.out and my_out.err to
    # store program output.
    $PROGRAM < $INPUT > my_out.out 2> my_out.err
    PROGRAM_EXIT_CODE=$?

    if [ "$PROGRAM_EXIT_CODE" -ne "0" ]; then
        echo "Program execution failed with error code: $PROGRAM_EXIT_CODE"
        continue
    fi

    # TODO: Collapse differs!!!
    echo -n "Comparing standard output... "
    if diff my_out.out $OUTPUT &> /dev/null; then
        printGood "OK"
    else
        printBad "DIFFERS!"
        if yesNoConfirm "Show diff?"; then
            diff --color my_out.out $OUTPUT
            if yesNoConfirm "Abort testing?"; then
                exit 1
            fi
        fi
    fi

    echo -n "Comparing stderr output... "
    if diff my_out.err $ERR &> /dev/null; then
        printGood "OK"
    else
        printBad "DIFFERS!"
        if yesNoConfirm "Show diff?"; then
            diff --color my_out.err $ERR
            if yesNoConfirm "Abort testing?"; then
                exit 1
            fi
        fi
    fi

    # Unfortunetly we have to run a program again to look for memory
    # leaks. Note: valgrind will return 1 if there are leaks, 0 when they are
    # not. This differs from the default, when valgrind just returns programs
    # exit code.
    if $CHECK_FOR_MEM_LEAKS; then
        echo -n "Checking for memory leaks... "
        if valgrind --leak-check=full --log-file="my_valgr.log" \
                    --error-exitcode=1 $PROGRAM < $INPUT &> /dev/null; then
            printGood "OK"
        else
            printBad "LEAKS DETECTED!"
            if yesNoConfirm "Show Valgrind output?"; then
                cat my_valgr.log
                if yesNoConfirm "Abort testing?"; then
                    exit 1
                fi
            fi
        fi
    fi
done
