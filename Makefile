CC=gcc

CFLAGS:=-fPIC -shared
SRC:=initializer.c finalizer.c
OBJ:=$(patsubst %.c, %.o, $(SRC))

.PHONY: echo

echo:
	echo $(OBJ)

libshell_profiler.so: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.c:
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

main: main.o
	$(CC) $(CFLAGS) -o $@ $^
