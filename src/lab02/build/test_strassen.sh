#!/bin/bash

N=1000

if [ ! -f "../out/strassen" ]
then
    echo "DOESN'T EXISIST strassen"
    exit
fi

if [ ! -f "../out/strassen_par" ]
then
    echo "DOESN'T EXISIST strassen_par"
    exit
fi

./../out/strassen $N > seq.txt
./../out/strassen_par $N > par.txt

diff seq.txt par.txt
