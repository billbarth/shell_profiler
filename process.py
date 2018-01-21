#!/usr/bin/env python
# -*- python -*-
from __future__ import print_function
import os, sys, re
import yaml

def main():
  if (len(sys.argv) > 0):
    fn=sys.argv[1]
    print('Loading: ', fn);

    with open(fn) as f:
      data=yaml.safe_load(f)

    print(data)

if ( __name__ == '__main__'):
  main()
