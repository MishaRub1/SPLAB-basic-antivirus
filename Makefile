CC := gcc
CFLAGS := -Wall -Wextra -Werror -std=c11 -g

.PHONY: all clean

all: AntiVirus

AntiVirus: AntiVirus.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f AntiVirus *.o
