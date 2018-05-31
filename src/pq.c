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

void PQ_insert(PQ *pq, char byte, unsigned long long freq) {
	if (pq->size >= pq->capacity) {
		dprintf(2, "Inserting into max capacity priority queue\n");
		exit(1);
		return;
	}

	Node *temp = Node_new(byte, freq);
	pq->heap[pq->size] = temp;
	unsigned int idx = pq->size;
	pq->size++;


	while (idx) { //while not root
		unsigned int parent = (idx - 1) / 2;
		//Can move temp up one layer
		if (pq->heap[parent]->freq > temp->freq) {
			PQ_swap_nodes(pq, parent, idx);
			idx = parent;
		} else { //temps final location is idx
			break;
		}
	}

	return;
}

Node *PQ_extract(PQ *pq) {
	if (pq->size == 0) {
		dprintf(2, "Extracting from empty priority queue\n");
		exit(2);
	}

	Node *min = pq->heap[0];
	pq->heap[0] = pq->heap[pq->size-1];
	pq->size--;

	int idx = 0;
	int left = (idx * 2) + 1;
	int right = (idx * 2) + 2;

	int max = (pq->heap[left]->freq >= pq->heap[right]->freq) ? left : right;



	return min;
}
