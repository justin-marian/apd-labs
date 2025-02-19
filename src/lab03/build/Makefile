CC = gcc
CFLAGS = -lpthread -lm -Wall -O

SRC_DIR = ../src
SOURCES = $(wildcard $(SRC_DIR)/*.c)

OUT_DIR = ../bin
EXECUTABLES = $(patsubst $(SRC_DIR)/%.c,$(OUT_DIR)/%,$(SOURCES))

.PHONY: all clean

all: $(EXECUTABLES)

$(OUT_DIR)/%: $(SRC_DIR)/%.c | $(OUT_DIR)
	$(CC) $< -o $@ $(CFLAGS)

$(OUT_DIR):
	mkdir -p $@

clean:
	rm -rf $(OUT_DIR)
