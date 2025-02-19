#!/bin/bash

N=1000  # SIZE ARRAY
T=2     # NR OF THREADS
R=10    # NR ELEMENTS ON A ROW

if [ ! -f "../out/example" ]
then
    echo "DOESN'T EXISIST example"
    exit
fi

if [ ! -f "../out/add_serial" ]
then
    echo "DOESN'T EXISIST add_serial"
    exit
fi

if [ ! -f "../out/add_parallel" ]
then
    echo "DOESN'T EXISIST add_parallel"
    exit
fi

../out/add_serial $N $R > add_serial.txt
../out/add_parallel $N $R $T > add_parallel.txt
../out/example > example.txt
