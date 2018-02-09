#!/bin/ksh -i
#KSH is weird, but -i put it in interactive mode and sources $ENV
#however, ${.sh.lineno} seems to be set wrong (always 1)

## function dbg
## {
##   echo debugging
## 
##   
##   echo "${.sh.file} \${.sh.lineno} ${.sh.fun}"
## 
## 
##   
##   }
## 
## 
## trap dbg DEBUG

/bin/echo "In KSH"
#module list
/bin/echo "Out KSH"
/bin/echo $0


