#!/bin/bash

gcc -c main.c -o main_gcc.o
icc -c main.c -o main_icc.o
ifort -c main.f90 -o main_ifort.o
gfortran -c main.f90 -o main_gfortran.o

gcc -fPIC -shared -o libinit_gcc2.so initializer2.c
icc -fPIC -shared -o libinit_icc2.so initializer2.c

gcc -o gcc_main_gcc_init main_gcc.o libinit_gcc2.so -Wl,-rpath,.
icc -o icc_main_gcc_init main_icc.o libinit_gcc2.so -Wl,-rpath,.
gcc -o gcc_main_icc_init main_gcc.o libinit_icc2.so -Wl,-rpath,.
icc -o icc_main_icc_init main_icc.o libinit_icc2.so -Wl,-rpath,.

   ifort -o    ifort_main_gcc_init main_ifort.o    libinit_gcc2.so -Wl,-rpath,.
gfortran -o gfortran_main_gcc_init main_gfortran.o libinit_gcc2.so -Wl,-rpath,.
   ifort -o    ifort_main_icc_init main_ifort.o    libinit_icc2.so -Wl,-rpath,.
gfortran -o gfortran_main_icc_init main_gfortran.o libinit_icc2.so -Wl,-rpath,.

gcc -o gcc_main main_gcc.o
icc -o icc_main main_icc.o

   ifort -o    ifort_main main_ifort.o
gfortran -o gfortran_main main_gfortran.o 


