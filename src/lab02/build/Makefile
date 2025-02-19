CC = gcc
CFLAGS = -Wall -Wextra -O
LDFLAGS = -lpthread

SRC_DIR = ../src
OUT_DIR = ../out

# List of source files to compile
SOURCES = mutex.c barrier.c \
		  strassen.c strassen_par.c \
		  multiply_seq.c multiply_outer.c multiply_middle.c multiply_inner.c
EXECUTABLES = $(patsubst %.c,$(OUT_DIR)/%,$(SOURCES))

.PHONY: all clean

all: $(EXECUTABLES)

$(OUT_DIR)/%: $(SRC_DIR)/%.c
	@mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -rf $(OUT_DIR) *.txt
