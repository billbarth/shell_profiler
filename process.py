#!/usr/bin/env python
# -*- python -*-
from __future__ import print_function
import os, sys, re
import yaml
from collections import defaultdict
import textwrap
import linecache

# There was a collection of total elapsed time in here, but that's a lie since
# it is duplicated across several nested executions of the shell, and we don't
# have the depth here.

# second argument is the time_type mode NOT CURRENTLY USED
# would need to read the whole file into memory to sort based in time_type
# could do if file was under X bytes

int_cmd_width=15
max_cmd_width=str(int_cmd_width) #could make this a command line arg later

def main():
  # Disable this argument until we figure out how to sort by it
  # time_type='etime' # figure out how to sort by this further down, see comment
                      # below

  # second arg is now the shell file that we want to print the line of, may want
  # to do something else later if the called thing on that line is als a script
  if (len(sys.argv) == 3):
    yaml_fn=sys.argv[1]
    # time_type=sys.argv[2]
    calling_script=sys.argv[2] # Maybe embed this in the YAML file
    print('Loading: ', yaml_fn)
  else:
    print('Usage: ',sys.argv[0], 'YAML-file main_script\nExiting...')
    exit(1)

  data={}

  first=True

  with open(yaml_fn) as f: 
    data=yaml.load(f)      #look at using load_all

  lines=[]
  for pid in data.keys(): # need to find a way to iterate here over the sorted
                          # time_types in data from the YAML file
                          # sorted(data,key=some function here of data [?][time_type],reverse=True):
    c=data[pid]['cmdline']
    cmd_str=c  #textwrap.fill(c+": ", 30) # disable for now, find a better way
               #to print later
               
    bash_line=int(data[pid]['line'])
    bash_source=data[pid]['file']
    et=float(data[pid]['etime'])
    ut=float(data[pid]['utime'])
    st=float(data[pid]['stime'])
    shlvl=int(data[pid]['shlvl'])

    #print('shlvl: ',shlvl)
    #    indent='  '*int(data[pid]['shlvl'])
    indent=''

    # Get a max length for shell script name here
    if (bash_source=="Not set:"):
      bash_source=calling_script


    if first:
      first=False
      maxmin="15.8"
      fmt_str="%-"+maxmin+"s  %4.4s: %"+"20.8"+"s %"+maxmin+"s %"+maxmin+"s %"+maxmin+"s"
      print(fmt_str % ("command","line","cmdline","elapsed","user","system"))
      print(fmt_str % (   "-"*15,"-"*15,   "-"*15,   "-"*15,"-"*15,  "-"*15))


    maxmin=max_cmd_width+"."+max_cmd_width
    fmt_str="%-"+maxmin+"s(%4d): %-"+"20.20"+"s %15.7g %15.7g %15.7g"
    print( fmt_str % (bash_source,bash_line,cmd_str, et, ut, st) )
    print('+'*shlvl+' '+bash_source+":"+str(bash_line)+":"+linecache.getline(bash_source,bash_line),end='')

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
