CC=gcc
CFLAGS=-g
BINS=server

all: $(BINS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf *.dSYM $(BINS)