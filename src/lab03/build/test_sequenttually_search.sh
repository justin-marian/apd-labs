#!/bin/bash

if [ ! -f "../bin/sequenttually_binary_search" ]
then
    echo "DOESN'T EXISIST ../bin/sequenttually_binary_search"
    exit
fi

printf "Testing N = 100 number = 4\n"
../bin/sequenttually_binary_search 100 4
printf "\n"

printf "Testing N = 100 number = 18\n"
../bin/sequenttually_binary_search 100 18
printf "\n"

printf "Testing N = 100 number = 19\n"
../bin/sequenttually_binary_search 100 19
printf "\n"

printf "Testing N = 100 number = -1\n"
../bin/sequenttually_binary_search 100 -1
printf "\n"

printf "Testing N = 100 number = 198\n"
../bin/sequenttually_binary_search 100 198
printf "\n"

printf "Testing N = 100 number = 201\n"
../bin/sequenttually_binary_search 100 201
printf "\n"