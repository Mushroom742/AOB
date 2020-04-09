CC=gcc

CFLAGS=-g3

OFLAGS=-march=native

inv_baseline:
	$(CC) -DBASELINE=1 $(CFLAGS) $(OFLAGS) -$(O) invert.c common.c -o invert -lm -fopenmp

inv_one_loop:
	$(CC) -DONE_LOOP=1 $(CFLAGS) $(OFLAGS) -$(O) invert.c common.c -o invert -lm -fopenmp

inv_bitflip:
	$(CC) -DBITFLIP=1 $(CFLAGS) $(OFLAGS) -$(O) invert.c common.c -o invert -lm -fopenmp

inv_unroll_8:
	$(CC) -DUNROLL_8=1 $(CFLAGS) $(OFLAGS) -$(O) invert.c common.c -o invert -lm -fopenmp

inv_unroll_8_bitflip:
	$(CC) -DUNROLL_8_BITFLIP=1 $(CFLAGS) $(OFLAGS) -$(O) invert.c common.c -o invert -lm -fopenmp

inv_parallel:
	$(CC) -DPARALLEL=1 $(CFLAGS) $(OFLAGS) -$(O) invert.c common.c -o invert -lm -fopenmp

inv_parallel_bitflip:
	$(CC) -DPARALLEL_BITFLIP=1 $(CFLAGS) $(OFLAGS) -$(O) invert.c common.c -o invert -lm -fopenmp

inv_unroll_8_parallel:
	$(CC) -DUNROLL_8_PARALLEL=1 $(CFLAGS) $(OFLAGS) -$(O) invert.c common.c -o invert -lm -fopenmp

inv_unroll_8_parallel_bitflip:
	$(CC) -DUNROLL_8_PARALLEL_BITFLIP=1 $(CFLAGS) $(OFLAGS) -$(O) invert.c common.c -o invert -lm -fopenmp

sob_baseline:
	$(CC) -DBASELINE=1 $(CFLAGS) $(OFLAGS) -$(O) sobel.c common.c -o sobel -lm

sob_one_loop:
	$(CC) -DONE_LOOP=1 $(CFLAGS) $(OFLAGS) -$(O) sobel.c common.c -o sobel -lm

sob_without_convolve:
	$(CC) -DWITHOUT_CONVOLVE=1 $(CFLAGS) $(OFLAGS) -$(O) sobel.c common.c -o sobel -lm

sob_unroll:
	$(CC) -DUNROLL=1 $(CFLAGS) $(OFLAGS) -$(O) sobel.c common.c -o sobel -lm

sob_unroll_full:
	$(CC) -DUNROLL_FULL=1 $(CFLAGS) $(OFLAGS) -$(O) sobel.c common.c -o sobel -lm


clean:
	rm -Rf *~ invert sobel *.gcda *.dyn *.dpi *.lock
