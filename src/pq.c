#include <stdlib.h>
#include <stdio.h>
#include "pq.h"

Node *Node_new(char byte, unsigned long long freq) {
	Node *temp = (Node *) malloc(sizeof(Node));
	temp->byte = byte;
	temp->freq = freq;
	temp->left = NULL;
	temp->right = NULL;

	return temp;
}

PQ *PQ_new(unsigned int capacity) {
	PQ *temp = (PQ *) malloc(sizeof(PQ));

	temp->size = 0;
	temp->capacity = capacity;

	temp->heap = (Node **) calloc(capacity, sizeof(Node *));

	return temp;
}

void PQ_swap_nodes(PQ *pq, unsigned int a, unsigned int b) {
	Node *temp = pq->heap[a];
	pq->heap[a] = pq->heap[b];
	pq->heap[b] = temp;

	return;
}

void PQ_insert(PQ *pq, Node *node) {
	if (pq->size >= pq->capacity) {
		dprintf(2, "Inserting into max capacity priority queue\n");
		exit(1);
		return;
	}

	pq->heap[pq->size] = node;
	unsigned int idx = pq->size;
	pq->size++;


	while (idx) { //while not root
		unsigned int parent = (idx - 1) / 2;
		//Can move node up one layer
		if (pq->heap[parent]->freq > node->freq) {
			PQ_swap_nodes(pq, parent, idx);
			idx = parent;
		} else { //nodes final location is idx
			break;
		}
	}

	return;
}

Node *PQ_extract(PQ *pq) {
	if (!pq->size) {
		dprintf(2, "Extracting from empty priority queue\n");
		exit(2);
	}

	Node *res = pq->heap[0];
	pq->heap[0] = pq->heap[--pq->size];

	unsigned int idx = 0;
	while (idx < pq->size) {
		unsigned int min = idx;
		unsigned int left = (idx * 2) + 1;
		unsigned int right = (idx * 2) + 2;

		if (left < pq->size && pq->heap[left]->freq < pq->heap[min]->freq) {
			min = left;
		}

		if (right < pq->size && pq->heap[right]->freq < pq->heap[min]->freq) {
			min = right;
		}

		if (min != idx) {
			PQ_swap_nodes(pq, min, idx);
			idx = min;
		} else {
			break;
		}
	}


	return res;
}

