#!/usr/bin/env python
# -*- python -*-
from __future__ import print_function
import os, sys, re
import yaml
from collections import defaultdict

# There was a collection of total elapsed time in here, but that's a lie since
# it is duplicated across several nested executions of the shell, and we don't
# have the depth here.

def main():
  time_type='etime'
  if (len(sys.argv) > 0):
    yaml_fn=sys.argv[1]
    script_fn=sys.argv[2]
    print('Loading: ', yaml_fn)

    data={}
    
    with open(yaml_fn) as f:
      data=yaml.safe_load(f)

    et_cmds=defaultdict(float)
    ut_cmds=defaultdict(float)
    st_cmds=defaultdict(float)


    for pid in data.keys():
      c=data[pid]['cmd']
      et=float(data[pid]['etime'])
      ut=float(data[pid]['utime'])
      st=float(data[pid]['stime'])
      et_cmds[c]+=et
      ut_cmds[c]+=ut
      st_cmds[c]+=st
      #      print(pid,c,et)

    if(time_type == 'etime'):
      for c in sorted(ut_cmds,key=ut_cmds.get,reverse=True):
        print(c,": ",et_cmds[c])
    elif(time_type == 'utime'):
      for c in sorted(ut_cmds,key=ut_cmds.get,reverse=True):
        print(c,": ",ut_cmds[c])
    else:
      for c in sorted(st_cmds,key=st_cmds.get,reverse=True):
        print(c,": ",st_cmds[c])

if ( __name__ == '__main__'):
  main()
