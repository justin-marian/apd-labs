#!/bin/bash

if [ ! -f "../bin/oets" ]
then
    echo "DOESN'T EXISIST ../bin/oets"
    exit
fi

if ../bin/oets 100 3 | grep -q "CORRECT SORT"; then
    echo "CORRECT SORT for N = 100 P = 3"
else
    echo "INCORRECT SORT for N = 100 P = 3"  
fi

if ../bin/oets 100 5 | grep -q "CORRECT SORT"; then
    echo "CORRECT SORT for N = 100 P = 5"
else
    echo "INCORRECT SORT for N = 100 P = 5" 
fi

if ../bin/oets 100 6 | grep -q "CORRECT SORT"; then
    echo "CORRECT SORT for N = 100 P = 6"
else
    echo "INCORRECT SORT for N = 100 P = 6"
fi

if ../bin/oets 100 7 | grep -q "CORRECT SORT"; then
    echo "CORRECT SORT for N = 100 P = 7"
else
    echo "INCORRECT SORT for N = 100 P = 7"
fi

if ../bin/oets 500 7 | grep -q "CORRECT SORT"; then
    echo "CORRECT SORT for N = 500 P = 7"
else
    echo "INCORRECT SORT for N = 500 P = 7"
fi

if ../bin/oets 1000 4 | grep -q "CORRECT SORT"; then
    echo "CORRECT SORT for N = 1000 P = 4"
else
    echo "INCORRECT SORT for N = 1000 P = 4"
fi

if ../bin/oets 1000 6 | grep -q "CORRECT SORT"; then
    echo "CORRECT SORT for N = 1000 P = 6"
else
    echo "INCORRECT SORT for N = 1000 P = 6"
fi

if ../bin/oets 1000 7 | grep -q "CORRECT SORT"; then
    echo "CORRECT SORT for N = 1000 P = 7"
else
    echo "INCORRECT SORT for N = 1000 P = 7"
fi