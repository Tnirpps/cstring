BIN_DIR = .bin
TEST_BINARY = $(BIN_DIR)/tests
TEST_FILE = tests/main.c
CC = gcc
CFLAGS = -fsanitize=address -fsanitize=undefined -g -Wall -Wextra
all: run_tests

$(TEST_BINARY): $(TEST_FILE)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(TEST_BINARY) $(TEST_FILE)

run_tests: $(TEST_BINARY)
	./$(TEST_BINARY)

clean:
	rm -r $(BIN_DIR)
