CC=gcc

CFLAGS=-g3

OFLAGS=-march=native

inv_baseline:
	$(CC) -DBASELINE=1 $(CFLAGS) -$(O) invert.c common.c -o invert -lm

inv_v1:
	$(CC) -DV1=1 $(CFLAGS) -$(O) invert.c common.c -o invert -lm

inv_v2:
	$(CC) -DV2=1 $(CFLAGS) -$(O) invert.c common.c -o invert -lm

sob_baseline:
	$(CC) -DBASELINE=1 $(CFLAGS) -$(O) sobel.c common.c -o sobel -lm

clean:
	rm -Rf *~ invert sobel
