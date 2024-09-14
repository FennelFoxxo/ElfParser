CC=gcc
CFLAGS=-I. -Iinclude -g -Wall

TEST_TARGET=testelf
TEST_SRC=examples/testelf.c

EXAMPLE_TARGET=example
EXAMPLE_SRC=examples/example.c src/parse.c src/parse32.c src/parse64.c


.PHONY: all

all: $(TEST_TARGET) $(EXAMPLE_TARGET)

$(TEST_TARGET): $(TEST_SRC)
	$(CC) $(CFLAGS) $(TEST_SRC) -o $(TEST_TARGET) -Wno-unused-variable

$(EXAMPLE_TARGET): $(EXAMPLE_SRC)
	$(CC) $(CFLAGS) $(EXAMPLE_SRC) -o $(EXAMPLE_TARGET)