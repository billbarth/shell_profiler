#!/usr/bin/env python
# -*- python -*-
from __future__ import print_function
import os, sys, re
import yaml
from collections import defaultdict
import textwrap

# There was a collection of total elapsed time in here, but that's a lie since
# it is duplicated across several nested executions of the shell, and we don't
# have the depth here.

# second argument is the time_type mode NOT CURRENTLY USED

def main():
  time_type='etime' # remove this and update the usage
  if (len(sys.argv) == 3):
    yaml_fn=sys.argv[1]
    time_type=sys.argv[2]
    print('Loading: ', yaml_fn)
  else:
    print('Usage: ',sys.argv[0], 'YAML-file (etime|utime|stime)\nExiting...')
    exit(1)

  data={}

  with open(yaml_fn) as f:
    data=yaml.safe_load(f)


  for pid in data.keys(): # need to find a way to iterate here over the sorted
                          # time_types in data from the YAML file
                          # sorted(data,key=some function here of data [?][time_type],reverse=True):
    c=data[pid]['cmd']
    cmd_str=c #textwrap.fill(c+": ", 30) # disable for now, find a better way to
              #                           print later
    bash_line=int(data[pid]['line'])
    bash_source=data[pid]['file']
    et=float(data[pid]['etime'])
    ut=float(data[pid]['utime'])
    st=float(data[pid]['stime'])

    # Get a max length for shell script name here
    if (bash_source=="Not set:"):
      bash_source=10*' '

    fmt_str="%-s(%-9d): %s %-15.7g %-15.7g %-15.7g"
    print( fmt_str % (bash_source,bash_line,cmd_str, et, ut, st) )

  ## Aggregate all the data below. Now we have line numbers, so print that
  ## instead
  
  ## et_cmds=defaultdict(float)
  ## ut_cmds=defaultdict(float)
  ## st_cmds=defaultdict(float)

  ##  maxlen=0
  ##  for pid in data.keys():
  ##    c=data[pid]['cmd']
  ##    maxlen=max(len(c),maxlen)
  ##    bash_line=int(data[pid]['line'])
  ##    et=float(data[pid]['etime'])
  ##    ut=float(data[pid]['utime'])
  ##    st=float(data[pid]['stime'])
  ##    et_cmds[c]+=et
  ##    ut_cmds[c]+=ut
  ##    st_cmds[c]+=st
    #      print(pid,c,et)

  ## make all 3 prints like the first one
  ## use textwrap to shorten the line to < 80 chars

  ## Old code that worked on aggregated data

  ##  if(time_type == 'etime'):
  ##    for c in sorted(ut_cmds,key=ut_cmds.get,reverse=True):
  ##      fmt_str="(%-9d): %-30s %-15.7g %-15.7g %-15.7g"
  ##      cmd_str=textwrap.fill(c+": ", 30)
  ##      tm_str=fmt_str % (bash_line,cmd_str, et_cmds[c], ut_cmds[c], st_cmds[c])
  ##      out_str=tm_str
  ##      print(out_str)
  ##  elif(time_type == 'utime'):
  ##    for c in sorted(ut_cmds,key=ut_cmds.get,reverse=True):
  ##      print(c,": ",et_cmds[c], ut_cmds[c], st_cmds[c])
  ##  else:
  ##    for c in sorted(st_cmds,key=st_cmds.get,reverse=True):
  ##      print(c,": ",et_cmds[c], ut_cmds[c], st_cmds[c])

if ( __name__ == '__main__'):
  main()
