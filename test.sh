#!/bin/bash

# Mateusz Dudziński
# IPP, 2018L Task: "Maraton filmowy".

DIRECTORY=./tests
PROGRAM=./bin/program

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

    
done
