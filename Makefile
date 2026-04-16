
CC = gcc

SRC_DIR = src
BUILD_DIR = build

SRC = $(SRC_DIR)/main.c
OBJ = $(BUILD_DIR)/lp_solver.o
TARGET = $(BUILD_DIR)/lp_solver

# Compiler flags
CFLAGS = -std=c99 -Wall -Wextra -O2
LDFLAGS = -lm

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJ) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
