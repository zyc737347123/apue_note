#!/bin/bash
gcc -fPIC -c hello.c
gcc -shared -o libmyhello.so hello.o
#mv libmyhello.so /usr/lib/
#gcc -o hello a.c -lmyhello 
gcc -o hello a.c -L. -lmyhello
