CC = gcc
CFLAGS = -g

src_srl := array1.c
src_not_srl := array2.c

all: clear srl_opt srl_nopt nsrl_opt nsrl_nopt

srl_opt:
	$(CC) $(CFLAGS) $(src_srl) -O2 -o serial_optimized

srl_nopt:
	$(CC) $(CFLAGS) $(src_srl) -O0 -o serial_non_optimized

nsrl_opt:
	$(CC) $(CFLAGS) $(src_not_srl) -O2 -o not_serial_optimized

nsrl_nopt:
	$(CC) $(CFLAGS) $(src_not_srl) -O0 -o not_serial_non_optimized

clear:
	rm -f serial_optimized serial_non_optimized
	rm -f not_serial_optimized not_serial_non_optimized