#!/bin/bash

/bin/echo "----In  ${BASH_SOURCE[0]}: $LINENO----"

./test_nest2.sh # bash-based
#./test_ksh.sh #ksh-based

/bin/echo "----Out ${BASH_SOURCE[0]}: $LINENO----"
