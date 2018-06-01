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


	char *buf = (char *) malloc(BUFSIZE);

	if (argc > 1) {
		int fd = open(argv[1], O_RDONLY);
		int cnt = 0;
		while ((cnt = read(fd, buf, BUFSIZE)) > 0) {
			for (int i = 0; i < cnt; i++) {
				freqs[(unsigned)buf[i]]++;
			}
		}
	}

	PQ *pq = PQ_new(256);


}
