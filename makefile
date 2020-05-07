.PHONY: clean
OFLAGS = -lm -Wall -Werror
CFLAGS = -g -c
VPATH = src
COMPILER = gcc

bin/main: build/main.o build/coder.o build/command.o
	$(COMPILER) $^ -o $@ $(OFLAGS)

build/main.o: main.c
	$(COMPILER) $(CFLAGS)  $^ -o $@
build/coder.o: coder.c
		$(COMPILER) $(CFLAGS)  $^ -o $@
build/command.o: command.c
			$(COMPILER) $(CFLAGS)  $^ -o $@

clean:
	rm -rf build/*.o bin/main
