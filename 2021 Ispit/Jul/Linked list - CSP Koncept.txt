----------------------------------------
Ulancana lista N node procesa - CSP
Cuvaju elemente ili su slobodni
struct Node {
	T value;
	Node *next;
}

void head() {
	Node *head = headProc;

	void remove(int val) {
		Node *curr = head, *prev = null;
		while (curr && curr->value != val) prev = curr, curr = curr->next;
		if (!curr) return; // node not found
		(!prev ? head : prev->next) = curr->next;
		free::add(curr);
	}
}

void free() {
	Node *freeHead = null;

	void add(Node *node) {
		node->next = freeHead;
		freeHead = node;
	}
}

void start(int value) {
	head::remove(value);
}

void node(i:1..N) {
	?
}
-----------------
Potrebno prevesti sve procedure unutar nodea
