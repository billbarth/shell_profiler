#!/bin/bash

gcc -g -c main.c -o main_gcc.o
icc -c main.c -o main_icc.o
ifort -c main.f90 -o main_ifort.o
gfortran -c main.f90 -o main_gfortran.o

gcc -g -fPIC -shared -o libinit_gcc.so initializer.c
icc -fPIC -shared -o libinit_icc.so initializer.c

gcc -g -fPIC -shared -o libfini_gcc.so finalizer.c
icc -fPIC -shared -o libfini_icc.so finalizer.c

gcc -g -o gcc_main_gcc_init_gcc_fini main_gcc.o libinit_gcc.so libfini_gcc.so -Wl,-rpath,.
icc -o icc_main_gcc_init_gcc_fini main_icc.o libinit_gcc.so libfini_gcc.so -Wl,-rpath,.
gcc -g -o gcc_main_icc_init_icc_fini main_gcc.o libinit_icc.so libfini_icc.so -Wl,-rpath,.
icc -o icc_main_icc_init_icc_fini main_icc.o libinit_icc.so libfini_icc.so -Wl,-rpath,.

   ifort -o    ifort_main_gcc_init main_ifort.o    libinit_gcc.so -Wl,-rpath,.
gfortran -o gfortran_main_gcc_init main_gfortran.o libinit_gcc.so -Wl,-rpath,.
   ifort -o    ifort_main_icc_init main_ifort.o    libinit_icc.so -Wl,-rpath,.
gfortran -o gfortran_main_icc_init main_gfortran.o libinit_icc.so -Wl,-rpath,.


