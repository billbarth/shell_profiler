#!/bin/bash

# Assumes that SP_OUTFILE is set. Test for this later.

## if ! echo $SP_OUTFILE | grep -q '^/'; then
##   echo "SP_OUTFILE must be an absolute path"
## else
##   export SP_OUTFILE=`pwd`/sp.yaml
##   echo "using SP_OUTFILE=$SP_OUTFILE instead"
## fi

export MY_SHELL_SCRIPT="${1}_temp"


export BASH_ENV_ORIG=$BASH_ENV
export TRAP_ENV=`mktemp -p . .trap_env.XXXXXXXX`

#echo $TRAP_ENV

if file $1 | grep -q -E 'Bourne-Again'; then
  # set up for bash

cat > "$TRAP_ENV" << 'EOF'

. $BASH_ENV_ORIG

trap ' export LN="$LINENO"; export BS="$BASH_SOURCE"; echo "$LN $BS"'  DEBUG

EOF

export BASH_ENV=$TRAP_ENV

### Maybe put this back if the script is KSH.

### elif file $1 | grep -q -E 'Korn'; then
###   echo "Found Korn shell. Copying script and converting to Bash"
### 
### # setup for Korn
### cat > "$TRAP_ENV" << 'EOF'
### 
### . $BASH_ENV_ORIG
### 
### trap ' export LN="${.sh.lineno}"; export BS="${.sh.file}"; echo "TRAP $LN $BS"'  DEBUG
### 
### EOF
### 
###

### Skip this if not really needed. Shouldn't copy big scripts unless
### we need to edit them

if [ -f $MY_SHELL_SCRIPT ]; then
  rm $MY_SHELL_SCRIPT
fi

cp "$1" $MY_SHELL_SCRIPT

chmod +x $MY_SHELL_SCRIPT

### echo "Found ksh main script, converting to Bash"
### 
### sed -e '0,/ksh/{s;#!/bin/.*sh.*;#!/bin/bash;g}' ${1} > $MY_SHELL_SCRIPT
### 
### export ENV=$TRAP_ENV
### 
elif file $1 | grep -q -E 'POSIX shell'; then
#set up for /bin/sh
cat > "$TRAP_ENV" << 'EOF'

. $BASH_ENV_ORIG

trap ' export LN="$LINENO"; export BS="$BASH_SOURCE"; echo "$LN $BS"'  DEBUG

EOF

if [ -f $MY_SHELL_SCRIPT ]; then
  rm $MY_SHELL_SCRIPT
fi
cp "$1" $MY_SHELL_SCRIPT

chmod +x $MY_SHELL_SCRIPT

#sed -e '0,/sh/{s;#!/bin/.*sh.*;#!/bin/bash;g}' ${1} > $MY_SHELL_SCRIPT

export BASH_ENV=$TRAP_ENV

fi

## echo "--------------------"
## cat $TRAP_ENV
## echo "--------------------"

function clean_up
{
  if [ -f $TRAP_ENV ]; then
    rm $TRAP_ENV
    
  fi
}

trap clean_up EXIT

export LN=1
export BASH_SOURCE

# Actually required to export
PR_PATH=$(dirname $(readlink -f "$0"))

echo "Path to .so: " "${PR_PATH}"

export LD_PRELOAD="${PR_PATH}"/libshell_profiler.so
export SHLVL=0

echo "Dollar star: $*"
if [ -f $MY_SHELL_SCRIPT ]; then
  cmd="$MY_SHELL_SCRIPT ${@:2}"
else
  cmd="$*"
fi

echo Running: "$cmd"

$cmd

unset LD_PRELOAD




