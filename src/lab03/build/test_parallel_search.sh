#!/bin/bash

if [ ! -f "../bin/parallel_binary_search" ]
then
    echo "DOESN'T EXISIST ../bin/parallel_binary_search"
    exit
fi

printf "Testing N = 100 P = 3 number = 4\n"
../bin/parallel_binary_search 100 3 4
printf "\n"

printf "Testing N = 100 P = 5 number = 18\n"
../bin/parallel_binary_search 100 5 18
printf "\n"

printf "Testing N = 100 P = 3 number = 19\n"
../bin/parallel_binary_search 100 3 19
printf "\n"

printf "Testing N = 100 P = 3 number = -1\n"
../bin/parallel_binary_search 100 3 -1
printf "\n"

printf "Testing N = 100 P = 3 number = 198\n"
../bin/parallel_binary_search 100 3 198
printf "\n"

printf "Testing N = 100 P = 3 number = 201\n"
../bin/parallel_binary_search 100 3 201
printf "\n"