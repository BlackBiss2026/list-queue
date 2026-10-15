CC      ?= gcc
CFLAGS  = -std=c99 -Wall -Wextra -pedantic -g -Iinclude
SRCS    = src/list.c src/queue.c
TESTS   = tests/tests.c

.PHONY: all test asan werror rules clean

all: build/tests

build:
	mkdir -p build

build/tests: $(SRCS) $(TESTS) include/list.h include/queue.h | build
	$(CC) $(CFLAGS) $(SRCS) $(TESTS) -o $@ -lm

# Same tests, built with AddressSanitizer: catches leaks, double free,
# use-after-free and out-of-bounds access.
build/tests_asan: $(SRCS) $(TESTS) include/list.h include/queue.h | build
	$(CC) $(CFLAGS) -fsanitize=address,undefined -fno-omit-frame-pointer \
	    $(SRCS) $(TESTS) -o $@ -lm

test: build/tests
	./build/tests all

asan: build/tests_asan
	ASAN_OPTIONS=detect_leaks=1 UBSAN_OPTIONS=halt_on_error=1 ./build/tests_asan all

# The code must compile without a single warning.
werror: | build
	$(CC) $(CFLAGS) -Werror -c src/list.c -o build/list.o
	$(CC) $(CFLAGS) -Werror -c src/queue.c -o build/queue.o

rules:
	python3 tests/check_rules.py src/list.c src/queue.c

clean:
	rm -rf build
