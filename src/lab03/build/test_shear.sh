#!/bin/bash

if [ ! -f "../bin/shear" ]
then
    echo "DOESN'T EXISIST ../bin/shear"
    exit
fi

if ../bin/shear 10 3 | grep -q "CORRECT SORT"; then
    echo "CORRECT SORT for L = 10 P = 3"
else
    echo "INCORRECT SORT for L = 10 P = 3"  
fi

if ../bin/shear 10 5 | grep -q "CORRECT SORT"; then
    echo "CORRECT SORT for L = 10 P = 5"
else
    echo "INCORRECT SORT for L = 10 P = 5" 
fi

if ../bin/shear 10 6 | grep -q "CORRECT SORT"; then
    echo "CORRECT SORT for L = 10 P = 6"
else
    echo "INCORRECT SORT for L = 10 P = 6"
fi

if ../bin/shear 10 7 | grep -q "CORRECT SORT"; then
    echo "CORRECT SORT for L = 10 P = 7"
else
    echo "INCORRECT SORT for L = 10 P = 7"
fi

if ../bin/shear 50 7 | grep -q "CORRECT SORT"; then
    echo "CORRECT SORT for L = 50 P = 7"
else
    echo "INCORRECT SORT for L = 50 P = 7"
fi

if ../bin/shear 100 4 | grep -q "CORRECT SORT"; then
    echo "CORRECT SORT for L = 100 P = 4"
else
    echo "INCORRECT SORT for L = 100 P = 4"
fi

if ../bin/shear 100 6 | grep -q "CORRECT SORT"; then
    echo "CORRECT SORT for L = 100 P = 6"
else
    echo "INCORRECT SORT for L = 100 P = 6"
fi

if ../bin/shear 100 7 | grep -q "CORRECT SORT"; then
    echo "CORRECT SORT for L = 100 P = 7"
else
    echo "INCORRECT SORT for L = 100 P = 7"
fi