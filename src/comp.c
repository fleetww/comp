#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include "pq.h"

#define BUFSIZE 4096


int main(int argc, char** argv) {

	//need to start at 0, for no occurrence
	unsigned long long *freqs = (unsigned long long *) calloc(256, sizeof(unsigned long long));

	int fd = open(argv[1], O_RDONLY);

	char *buf = (char *) malloc(BUFSIZE);
	int cnt = 0;
	while ((cnt = read(fd, buf, BUFSIZE)) > 0) {
		for (int i = 0; i < cnt; i++) {
			freqs[(unsigned)buf[i]]++;
		}
	}

	PQ *pq = PQ_new(256);

	for (int i = 9; i >= 0; i--) {
		PQ_insert(pq, i, i);
	}

	for (unsigned int i = 0; i < pq->size; i++) {
		printf("%lld ", pq->heap[i]->freq);
	}

	printf("\n");


}
