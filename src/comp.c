#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include "pq.h"

#define BUFSIZE 4096

//Returns number of bytes given the number of bits involved
#define NBYTES(N) ((unsigned int)(((N)+7)/8))


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

	//Encoding *codes = GenerateCodes(root);

	//Test code
	PreFixPrint(root);

	return 0;
}

void GenerateCodesHelper(Node *node, Encoding *codes, unsigned int nbits, unsigned char *code) {
	//Node is an actual encoding, not a joined node
	if (!node->left && !node->right) {
		memcpy(codes[node->byte].code, code, 32);
		return;
	}

	if (node->left) {
		unsigned char new_code[32];
		for (unsigned int i = 0; i < 32; i++) {
			new_code[i] = code[i];
		}
		unsigned int next_byte = NBYTES(nbits+1);

		if (next_byte==32) {
			dprintf(2, "Inserting past end of code\n");
			exit(4);
		}

		//number of bits from left of current byte
		unsigned int rmbit = nbits - 8*next_byte;
		code[next_byte] |= (0x1 << (8-rmbit+1));

		GenerateCodesHelper(node->left, codes, nbits+1, new_code);
	}
	if (node->right) {
		unsigned char new_code[32];
		for (unsigned int i = 0; i < 32; i++) {
			new_code[i] = code[i];
		}
		unsigned int next_byte = NBYTES(nbits+1);

		//Sanity check, this shouldn't actually be reachable
		if (next_byte==32) {
			dprintf(2, "Inserting past end of code\n");
			exit(4);
		}

		//number of bits from right of current byte
		unsigned int rmbit = nbits - 8*next_byte;
		code[next_byte] |= (0x1 << (8-rmbit+1));

		GenerateCodesHelper(node->right, codes, nbits+1, new_code);
	}

	return;
}

//codes[byte]=encoding struct
Encoding *GenerateCodes(Node *root) {
	Encoding *codes = calloc(256, sizeof(Encoding));

	unsigned char temp[32];
	memset(temp, 0, 32);

	GenerateCodesHelper(root, codes, 0, temp);

	return codes;
}
