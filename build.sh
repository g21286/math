#!/bin/bash

file=set

if [ $# -gt 0 ]; then
	file=$1
fi

gcc -Wall -Wextra -Wno-unused-parameter $file.c -o $file.exe

