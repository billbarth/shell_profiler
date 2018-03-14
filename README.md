# BASH Shell Sript Profiler (`shell_profiler`)

## Getting Started

### Prerequisites

* GNU make
* Python 2.7.2+
* Linux with ELF binaries

### Installing

Untar or clone.

```make```

## Running the tests

The tests are basically non-existent, but there are a few sample shell scripts that you can use to try out the profiler.

```export SP_OUTFILE=foo.yaml; rm $SP_OUTFILE; ./profile.sh test_nest1.sh```

isn't a bad place to start. Then you can examine the output YAML file:

```
./process.py $SP_OUTFILE test_nest1.sh

         start time command          line               cmdline         elapsed            user          system
------------------- --------         ----              --------        --------        --------        --------
2018-03-14 16:05:00 test_nest1.sh  (   1): /bin/bash ./test_nes        0.026579        0.002682        0.003576
2018-03-14 16:05:00 ./test_nest1.sh(   5): /bin/bash ./test_nes        0.014575        0.002968        0.002968
2018-03-14 16:05:00 ./test_nest2.sh(   4): /bin/ls                     0.001389               0        0.003183
2018-03-14 16:05:00 ./test_nest1.sh(   3): /bin/echo ----In  ./        0.000254               0        0.002134
2018-03-14 16:05:00 ./test_nest2.sh(   3): /bin/echo In ./test_         0.00022               0        0.001876
2018-03-14 16:05:00 ./test_nest2.sh(   5): /bin/echo Out ./test        0.000194        0.000878        0.000878
2018-03-14 16:05:00 ./test_nest1.sh(   7): /bin/echo ----Out ./        0.000191               0        0.001712

```

## Deployment

----


Basic instructions should be:

put shell_profiler.tar.gz on the Linux system where you want to use it.
tar xf shell_profiler.tar.gz
cd shell_profiler
make
export SP_OUTFILE=whatever.yaml    # the .yaml extension is optional
./profile.sh yourscript.sh
./process.py $SP_OUTFILE yourscript.sh

If SP_OUTFILE isn’t set to a valid file name, profiler.sh will crash (the LD_PRELOADed library, at least). That’s my first bug to fix and was my biggest problem yesterday. If this points to an existing file, it will append to that file, which the post-processor won’t handle. It’s best to remove $SP_OUTFILE between runs or archive it to a different name or place if you want to hang on to it. Also best to save copies of your shell script between runs if you want to maintain the history since process.py needs to refer to it to know what lines/commands to show you. $SP_OUTFILE will get one YAML record per executable in your script. So, it’s not every line, but it could be more than one per line if you run commands together with semicolons (which works fine). If you have lots of executions, this file can get pretty bit. It’s about 6 lines per execution. I also tested (at your suggestion):

for I in something; do
  ./aout $i &
done
wait

...and it works just fine. It doesn’t blow up the data gathering or the YAML file, but the post-processing might not be what you want to see. I added the start time as YYYY-MM-DD HH:MM:SS format to each reported line (I have the time to the microsecond if you want more resolution HH.MM.SS.XXXXXX or something else, you can also have the UNIX epoch if you prefer it with microsecond resolution).

The YAML file is updated at exit for each execution, so it’s possible that lots of backgrounded tasks could try to update it at once. There are no locks for this, so id you do a lot of high-frequency backgrounding, it could corrupt the YAML output. I haven’t pushed this mode hard enough to try to obtain executions that nearly exit at the same time or add locks to serialize the output. Let me know or give me an example if it blows up.

There are a couple of innocuous test shell scrips called test_*.sh some of which call each other (for testing’s sake). profile.sh needs to make a copy of your shell script to yourscript.sh_temp in the local directory (which it will delete when profile.sh exits). This was/is done in case modifications to the script are/were necessary. I think this will get factored out in near-future versions. I was working on a version that would rip out your #!/bin/ksh and unilaterally replace it with #!/bin/bash in the copy and run that (which probably wouldn’t work well, so it doesn’t try to do that anymore, though there is a fair amount of compatibility between ksh and bash, but not complete as you have recently discovered). The copy and the temp file are not edited by the current profile.sh wrapper. You will see the _temp file listed as the source shell script in the output of process.py instead of yourscript.sh, and I’ll clean this up in a future version of process.py.

Let me know if it doesn’t work for you on any system (Linux only since it only workx on ELF binaries and requires python 2.something, and I don’t know how far back it will go. It works with the python/2.7.13 module loaded on Stampede2. Any 2.7.x should be fine, but I have doubts about whether it will work with any 3.x. I not an Python expert, so I haven’t made that transition yet.