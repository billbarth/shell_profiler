#!/bin/bash

CMD="/bin/echo 'execing out of shell script!'"

/bin/echo "about to exec $CMD"
exec $CMD
/bin/echo "done execing $CMD, shouldn't be here!"
