#!/bin/bash

# Command for testing:
# hexdump -v -e '1/4 "%08x\n"' out

for f in ./test/*.asm
do
    echo "$f"
    
    NAME=`basename $f .asm`
    build/src/rvas $f
    ACTUAL=`hexdump -v -e '1/4 "%08x\n"' out`
    EXPECTED=`cat ./test/$NAME.out`
    
    if [[ $ACTUAL == $EXPECTED ]] ; then
        echo "Pass!"
        echo ""
    else
        echo "Error: Invalid!"
        echo ""
        echo "Actual:"
        echo $ACTUAL
        echo "Expected"
        echo $EXPECTED
        echo ""
        
        rm out
        exit 1
    fi
done

rm out

echo ""
echo "Done"
echo ""
