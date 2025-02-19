CC = mpicc
CFLAGS = -O2
MATHFLAGS = -lm

TARGET = ranksort
SOURCES = ranksort.c

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(MATHFLAGS)

# 1. sudo nano /etc/sysctl.d/10-ptrace.conf
# and change kernel.yama.ptrace_scope = 1 to 0

# 2. Restricts ptrace -- allow full access
# shared memory transport mechanism (btl_vader_single_copy_mechanism)
#--mca btl_vader_single_copy_mechanism none 
run1:
	mpirun -np 2  -mca btl_vader_single_copy_mechanism none $(TARGET) a1.txt

run2:
	mpirun -np 14 -mca btl_vader_single_copy_mechanism none --oversubscribe $(TARGET) a2.txt

clean:
	rm -f $(TARGET)