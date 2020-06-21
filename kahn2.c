#include "kahn.h"
#include <omp.h>

int main(int argc, const char *argv[]) {
	int ret_code;
	MM_typecode matcode;
	FILE *f;
	int M, N, nz;
	int i, *I, *J;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s [martix-market-filename]\n", argv[0]);
		exit(1);
	} else {
		if ((f = fopen(argv[1], "r")) == NULL) { exit(1); }
	}

	if (mm_read_banner(f, &matcode) != 0) {
		printf("Could not process Matrix Market banner.\n");
		exit(1);
	}

	if (mm_is_complex(matcode) && mm_is_matrix(matcode) && mm_is_sparse(matcode)) {
		printf("Sorry, this application does not support ");
		printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
		exit(1);
	}

	if ((ret_code = mm_read_mtx_crd_size(f, &M, &N, &nz)) != 0) { exit(1); }

	I = (int *)malloc(nz * sizeof(int));
	J = (int *)malloc(nz * sizeof(int));

	for (i = 0; i < nz; i++) {
		fscanf(f, "%d %d\n", &I[i], &J[i]);
		// Adjust from 1-based to 0-based indexing
		I[i]--;
		J[i]--;
	}

	if (f != stdin) { fclose(f); }

	// Create the graph
	vector nodes;
	vector_init(&nodes);
	for (i = 0; i < N; i++) {
		node_p node = createNode(i + 1);
		vector_add(&nodes, node);
	}
	// Add the edges to the graph
	for (i = 0; i < nz; i++) {
		addEdge(vector_get(&nodes, I[i]), vector_get(&nodes, J[i]));
	}

	// Print the graph
	// for (i = 0; i < N; i++) {
	// 	printNode(vector_get(&nodes, i));
	// }

	// Run the Kahn algorithm

	// Empty list that will contain the sorted elements
	vector L;
	vector_init(&L);

	// Set of pointers to all nodes with no incoming edges
	vector S;
	vector_init(&S);

	// Fill S with the nodes without incoming edges
	for (i = 0; i < vector_count(&nodes); i++) {
		node_p v = (node_p)vector_get(&nodes, i);
		if (v->Degree == 0 && v->Processed == 0) {
			v->Processed = 1;
			vector_add(&S, v);
		}
	}
#pragma omp parallel
{
	#pragma omp single
	{
	while (vector_count(&S)) {
		// Remove a node n from S
		node_p n = vector_get(&S, 0);
		vector_delete(&S, 0);
		// Add n to tail of L
		vector_add(&L, n);
		// For each node m with an edge e from n
		while (vector_count(&n->Vertices)) {
			node_p m = ((node_p)vector_get(&n->Vertices, 0));
			// Remove edge e from the graph
			#pragma omp critical
			removeEdge(n, vector_get(&n->Vertices, 0));
			// If m has no other incoming edges insert m into S
			if (m->Degree == 0) {
				m->Processed = 1;
				vector_add(&S, m);
			}
		}
}
}
}
	// If graph still has edges
	for (i = 0; i < N; i++) {
		if (!((node_p)vector_get(&nodes, i))->Processed) {
			printf("Could not run algorithm as graph has at least one cycle.");
			return 128;
		}
	}

	printf("\nPrinting the topologically sorted order\n");
	printf("L:\n");
	for (i = 0; i < vector_count(&L); i++) {
		printNode(vector_get(&L, i));
	}
	return 0;
}
