#!/bin/bash

/bin/echo Before sleep
/bin/sleep 2 &
/bin/echo After sleep
/bin/echo 0987654321
/bin/echo Before wait
wait
/bin/echo After wait
#export PS4='+ '
#set +x
