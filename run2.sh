#!/bin/bash

./ifort_main_gcc_init 
./ifort_main_gcc_init 
./gfortran_main_gcc_init 
./gfortran_main_icc_init 
./icc_main_gcc_init 
./icc_main_icc_init 
./gcc_main_gcc_init 
./gcc_main_icc_init

LD_PRELOAD=`pwd`/libinit_gcc2.so ./gcc_main
LD_PRELOAD=`pwd`/libinit_gcc2.so ./icc_main
LD_PRELOAD=`pwd`/libinit_gcc2.so ./ifort_main
LD_PRELOAD=`pwd`/libinit_gcc2.so ./gfortran_main

LD_PRELOAD=`pwd`/libinit_icc2.so ./gcc_main
LD_PRELOAD=`pwd`/libinit_icc2.so ./icc_main
LD_PRELOAD=`pwd`/libinit_icc2.so ./ifort_main
LD_PRELOAD=`pwd`/libinit_icc2.so ./gfortran_main

