CC = mpicc
CFLAGS = -O2

TARGET = reduce
SOURCES = reduce.c

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

# 1. sudo nano /etc/sysctl.d/10-ptrace.conf
# and change kernel.yama.ptrace_scope = 1 to 0

# 2. Restricts ptrace -- allow full access
# shared memory transport mechanism (btl_vader_single_copy_mechanism)
#--mca btl_vader_single_copy_mechanism none 
run: 
	mpirun -np 8 --oversubscribe -mca btl_vader_single_copy_mechanism none $(TARGET)

clean:
	rm -f $(TARGET) *.txt
