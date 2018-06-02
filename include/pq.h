typedef struct Node {
	unsigned char byte;
	unsigned long long freq;
	struct Node *left, *right;
} Node;

typedef struct PQ {
	unsigned int size;
	unsigned int capacity;

	Node **heap;
} PQ;

Node *Node_new(char byte, unsigned long long freq);

PQ *PQ_new(unsigned int capacity);

void PQ_insert(PQ *pq, Node *node);

void PQ_swap_nodes(PQ *pq, unsigned int a, unsigned int b);

Node *PQ_extract(PQ *pq);
