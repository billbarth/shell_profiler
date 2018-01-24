#!/usr/bin/env python
# -*- python -*-
from __future__ import print_function
import os, sys, re
import yaml
from collections import defaultdict

def main():
  if (len(sys.argv) > 0):
    yaml_fn=sys.argv[1]
    script_fn=sys.argv[2]
    print('Loading: ', yaml_fn)

    data={}
    
    with open(yaml_fn) as f:
      data=yaml.safe_load(f)

    cmds=defaultdict(float)

    for pid in data.keys():
      c=data[pid]['cmd']
      et=float(data[pid]['etime'])
      cmds[c]+=et
#      print(pid,c,et)

    for c in sorted(cmds,key=cmds.get,reverse=True):
      print(c,": ",cmds[c])
      

if ( __name__ == '__main__'):
  main()
