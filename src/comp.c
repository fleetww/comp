#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include "pq.h"

#define BUFSIZE 4096


typedef struct Encoding {
	unsigned short nbits;
	unsigned char code[32]; //a single encoding could need 256 bits
} Encoding;

Encoding Encoding_new() {
	Encoding code = {
		0,
		{0,0}
	};

	return code;
}

Encoding *GenerateCodes(Node *root);

//Test code
void PreFixPrint(Node *node) {
	if (!node->left && !node->right) {
		printf("%c : %lld\n", node->byte, node->freq);
	} else {
		printf("%lld\n", node->freq);
	}

	if (node->left) {
		PreFixPrint(node->left);
	}

	if (node->right) {
		PreFixPrint(node->right);
	}

	return;
}

int main(int argc, char** argv) {

	//need to start at 0, for no occurrence
	unsigned long long *freqs = (unsigned long long *) calloc(256, sizeof(unsigned long long));


	char *buf = (char *) malloc(BUFSIZE);

	//Generate the frequencies of each char
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

	//Insert all the characters present with their respective frequencies
	for (unsigned char c = 0; c < 256; c++) {
		if (freqs[c]) {
			Node *node = Node_new(c, freqs[c]);
			PQ_insert(pq, node);
		}
	}

	while (pq->size >= 2) {
		Node *min1 = PQ_extract(pq);
		Node *min2 = PQ_extract(pq);

		//A actual character node is represented by both left and right being NULL
		Node *join = Node_new(0, min1->freq + min2->freq);

		join->left = min1;
		join->right = min2;

		PQ_insert(pq, join);
	}

	Node *root = PQ_extract(pq);

	Encoding *codes = GenerateCodes(root);

	//Test code
	PreFixPrint(root);

	return 0;
}

void GenerateCodesHelper(Node *node, Encoding *codes, Encoding code) {
	//Node is an actual encoding, not a joined node
	if (!node->left && !node->right) {

	}
}

Encoding *GenerateCodes(Node *root) {
	Encoding *codes = calloc(256, sizeof(Encoding));

	Encoding code = Encoding_new();

	GenerateCodesHelper(root, codes, code);

	return codes;
}
