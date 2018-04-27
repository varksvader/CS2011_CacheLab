// Nils Backe nabacke
// Varnika Sinha vsinha2
// Team name: i_need_arrays

/* Ideas in Lab:
 * Givens:
 *  - E = 1 (line per set)
 *  - s = 5 (2^5 = 32 sets)
 *  - b = 5 (2^5 = 32 bytes per line)
 *  - 32x32 matrix (1024 elements, 1024*4 = 4096 bytes)
 *  ^ different for 64x64 and 67x61
 * Approaches:
 *  - Transpose blocks of matrix that gets stored fully in the cache at once
 * Ex with 32x32: 32*32*4 = 4096/1024 = 4 (wraps around 4 times), 32/4 = 8 (block size that can be held in cache)
 * Ex with 64x64: 64*64*4 = 16384/1024 = 16 (wraps around 16 times), 64/4 = 16 (block size that can be held in cache)
 * Advice: Do diagonals last so you don't overwrite current cache lines (makes it more optimized)
 */

/*
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/*
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N]) {
  int i, j, rowBlock, colBlock; // i is the row number, j is the columns number
	int diag = 0;
	int temp = 0; // variable to hold diagonal
	/* The access pattern for the defined problem sizes incorporate blocking;
   we define a sub-matrix of the matrix A with some size b to be a square block.
   The outer-loops iterate across these block structures, with the two inner loops
   iterating through each block. */
	if (N == 32) {
		//
		for (colBlock = 0; colBlock < N; colBlock += 8) {
			for (rowBlock = 0; rowBlock < N; rowBlock += 8) {
				for (i = rowBlock; i < rowBlock + 8; i ++) {
					for (j = colBlock; j < colBlock + 8; j ++) {
						if (i != j) {
							B[j][i] = A[i][j];
						}
						 else {
							//Reduce misses m < i*j in B by storing in temp instead of missing in B[j][i]
							temp = A[i][j];
							diag = i;
						}
					}
					//Transpose of a square-matrix has a unique property; no need to move elements on the diagonal.
					if (rowBlock == colBlock) {
						//Misses in B reduced to m < i
						B[diag][diag] = temp;
					}
				}
			}
		}
	}
	else if (N == 64) {
		//Iterate through matrix using column-major iteration over blocks
		for (colBlock = 0; colBlock < N; colBlock += 4) {
			for (rowBlock = 0; rowBlock < N; rowBlock += 4) {
				//Iterate over each row using row-major iteration
				for (i = rowBlock; i < rowBlock + 4; i ++) {
					for (j = colBlock; j < colBlock + 4; j ++) {
						if (i != j) {
							B[j][i] = A[i][j];
						}
						else {
							//On the diagonal
							temp = A[i][j];
							diag = i;
						}
					}
					if (rowBlock == colBlock) {
						B[diag][diag] = temp;
					}
				}
			}
		}
	}
	else {
		//Iterate through matrix using column-major iteration over blocks
		for (colBlock = 0; colBlock < M; colBlock += 16) {
			for (rowBlock = 0; rowBlock < N; rowBlock += 16) {
				// Since our sizes are prime, not all blocks will be square sub-matrices
				// Consider corner-case when (rowBlock + 16 > N) => invalid access. Explicit check for i, j < n, m
				for (i = rowBlock; (i < rowBlock + 16) && (i < N); i ++) {
					for (j = colBlock; (j < colBlock + 16) && (j < M); j ++) {
						if (i != j) {
							B[j][i] = A[i][j];
						}
						else {
							temp = A[i][j];
							diag = i;
						}
					}
					if (rowBlock == colBlock) {
						B[diag][diag] = temp;
					}
				}
	 		}
		}
	}
}

/*
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started.
 */

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);

}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}
