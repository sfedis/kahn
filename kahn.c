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
		if ((f = fopen(argv[1], "r")) == NULL) {
			exit(1);
		}
	}

	if (mm_read_banner(f, &matcode) != 0) {
		printf("Could not process Matrix Market banner.\n");
		exit(1);
	}

	/*  This is how one can screen matrix types if their application */
	/*  only supports a subset of the Matrix Market data types.      */

	if (mm_is_complex(matcode) && mm_is_matrix(matcode) && mm_is_sparse(matcode)) {
		printf("Sorry, this application does not support ");
		printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
		exit(1);
	}

	/* find out size of sparse matrix .... */

	if ((ret_code = mm_read_mtx_crd_size(f, &M, &N, &nz)) != 0) {
		exit(1);
	}

	/* reseve memory for matrices */
	I = (int *)malloc(nz * sizeof(int));
	J = (int *)malloc(nz * sizeof(int));

	for (i = 0; i < nz; i++) {
		fscanf(f, "%d %d\n", &I[i], &J[i]);
		I[i]--; /* adjust from 1-based to 0-based */
		J[i]--;
	}

	if (f != stdin) {
		fclose(f);
	}

	struct Graph *graph = createGraph(N);

	for (i = 0; i < nz; i++) {
		fprintf(stdout, "Vertex from node %d to node %d\n", I[i] + 1, J[i] + 1);
		addEdge(graph, I[i] + 1, J[i] + 1);
	}

	printGraph(graph);

	return 0;
}