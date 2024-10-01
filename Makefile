CC := gcc

CFLAGS := -Wall -Wextra -Wpedantic -Wconversion -Werror -O3 --std=gnu23
LDFLAGS := -lSDL2

SRC_DIR := src
BIN_DIR := bin

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)
TARGET := $(BIN_DIR)/main

all: $(TARGET)

$(TARGET): $(OBJS) $(LIB_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR)/*.o $(TARGET)

.PHONY: all clean
