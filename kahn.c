#include "kahn.h"

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

	
	for (i = 0; i < 4; i++) {
		removeEdge(vector_get(&nodes, i), vector_get(&nodes, 4));
	}

	// Print the graph
	for (i = 0; i < N; i++) {
		printNode(vector_get(&nodes, i));
	}



	return 0;
}