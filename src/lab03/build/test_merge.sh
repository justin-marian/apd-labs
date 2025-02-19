#!/bin/bash

if [ ! -f "../bin/merge" ]
then
    echo "DOESN'T EXISIST ../bin/merge"
    exit
fi

if ../bin/merge 64 3 | grep -q "CORRECT SORT"; then
    echo "CORRECT SORT for N = 64 P = 3"
else
    echo "INCORRECT SORT for N = 64 P = 3"  
fi

if ../bin/merge 128 4 | grep -q "CORRECT SORT"; then
    echo "CORRECT SORT for N = 128 P = 4"
else
    echo "INCORRECT SORT for N = 128 P = 4" 
fi

if ../bin/merge 128 7 | grep -q "CORRECT SORT"; then
    echo "CORRECT SORT for N = 128 P = 7"
else
    echo "INCORRECT SORT for N = 128 P = 7"
fi

if ../bin/merge 512 2 | grep -q "CORRECT SORT"; then
    echo "CORRECT SORT for N = 512 P = 2"
else
    echo "INCORRECT SORT for N = 512 P = 2"
fi

if ../bin/merge 512 7 | grep -q "CORRECT SORT"; then
    echo "CORRECT SORT for N = 512 P = 7"
else
    echo "INCORRECT SORT for N = 512 P = 7"
fi

if ../bin/merge 1024 4 | grep -q "CORRECT SORT"; then
    echo "CORRECT SORT for N = 1024 P = 4"
else
    echo "INCORRECT SORT for N = 1024 P = 4"
fi

if ../bin/merge 1024 6 | grep -q "CORRECT SORT"; then
    echo "CORRECT SORT for N = 1024 P = 6"
else
    echo "INCORRECT SORT for N = 1024 P = 6"
fi

if ../bin/merge 1024 7 | grep -q "CORRECT SORT"; then
    echo "CORRECT SORT for N = 1024 P = 7"
else
    echo "INCORRECT SORT for N = 1024 P = 7"
fi
