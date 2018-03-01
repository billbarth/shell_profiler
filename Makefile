CC=gcc

ifeq ($(CC),gcc)
STD_FLAG=--std=c99
else
STD_FLAG=-std=c99
endif

CFLAGS:=-fPIC -shared -g $(STD_FLAG)
SRC:=init_fini.c
OBJ:=$(patsubst %.c, %.o, $(SRC))
LDFLAGS=-ldl
EXEC:=libshell_profiler.so

all:: $(EXEC)

.PHONY: clean neat clobber echo

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

init_fini.o: init_fini.c

echo:
	@echo $(OBJ)


neat:
	$(RM) *~ .*~

clean: neat
	$(RM) $(OBJ)

clobber: clean
	$(RM) $(EXEC) 



