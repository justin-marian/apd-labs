#!/bin/bash

N=1000
P=2

if [ ! -f "../out/multiply_seq" ]
then
    echo "DOESN'T EXISIST multiply_seq"
    exit
fi

if [ ! -f "../out/multiply_outer" ]
then
    echo "DOESN'T EXISIST multiply_outer"
    exit
fi

if [ ! -f "../out/multiply_middle" ]
then
    echo "DOESN'T EXISIST multiply_middle"
    exit
fi

if [ ! -f "../out/multiply_inner" ]
then
    echo "DOESN'T EXISIST multiply_inner"
    exit
fi

./..out/multiply_seq $N > seq.txt
./..out/multiply_outer $N $P > par_outer.txt
./..out/multiply_middle $N $P > par_middle.txt
./..out/multiply_inner $N $P > par_inner.txt

diff seq.txt par_outer.txt
diff seq.txt par_middle.txt
diff seq.txt par_inner.txt
