#!/bin/bash

if [ ! -f "../out/mutex" ]
then
    echo "DOESN'T EXISIST mutex"
    exit
fi

for i in {1..10000}; do ./../out/mutex  2>&1 | grep 'a = 2000'; done