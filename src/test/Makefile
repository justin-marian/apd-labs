CC = gcc
CFLAGS = -Wall -Wextra -O
LDFLAGS = -lpthread

SRC_DIR = .
OUT_DIR = .

.PHONY: all build

build: $(OUT_DIR)/testapd

$(OUT_DIR)/testapd: $(SRC_DIR)/testapd.c | $(OUT_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf testapd