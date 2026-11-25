CC := gcc
CFLAGS := -m32 -g -Wall

.PHONY: all clean

all: virusDetector

virusDetector: AntiVirus.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f virusDetector *.o
