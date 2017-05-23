#!/bin/bash
ar crv libmyhello.a hello.o
gcc -o hello a.c -L. -lmyhello
