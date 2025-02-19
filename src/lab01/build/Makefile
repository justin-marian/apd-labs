CC = gcc
CFLAGS = -Wall -Wextra -O
LDFLAGS = -lpthread

OUT_DIR = ../out
SRC_DIR = ../src

.PHONY: all clean

all: $(OUT_DIR)/example $(OUT_DIR)/add_serial $(OUT_DIR)/add_parallel

$(OUT_DIR)/example: $(SRC_DIR)/example.c | $(OUT_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OUT_DIR)/add_serial: $(SRC_DIR)/add_serial.c | $(OUT_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(OUT_DIR)/add_parallel: $(SRC_DIR)/add_parallel.c | $(OUT_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

clean:
	rm -rf $(OUT_DIR) *.txt