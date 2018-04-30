// Nils Backe nabacke
// Varnika Sinha vsinha2
// Team name: nabacke_vsinha2 (i_need_arrays)

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
  int i; // the current row number of the element in matrix A to be tranposed
  int j; // the current column number of the element in matrix A to be transpose
  int rowBlock; // the size of the block being transposed in terms of rows
  int colBlock; // the size of the block being transposed in terms of columns
  int diag = 0; // the current row and column number of the diagonal
  int temp = 0; // var to hold the current diagonal of the row
  /* The access pattern for the defined problem sizes incorporate blocking;
   we define a sub-matrix of the matrix A with some size b to be a square block.
   The outer-loops iterate across these block structures, with the two inner loops
   iterating through each block. */
  // If 32x32 matrix
  if (N == 32) {
    // Iterate through matrix using column-major iteration over blocks
    // Goes through each column block in matrix
    for (colBlock = 0; colBlock < N; colBlock += 8) {
      // Goes through each row block in matrix
      for (rowBlock = 0; rowBlock < N; rowBlock += 8) {
        // Iterate over each row using row-major iteration
        // Goes through each row in the block
	for (i = rowBlock; i < rowBlock + 8; i++) {
          // Goes through each column in that row
	  for (j = colBlock; j < colBlock + 8; j++) {
            // Checks if diagonal
	    if (i != j) { // if not, sets tranposed element to correct place
	      B[j][i] = A[i][j];
	    }
	    else { // otherwise holds on to it to avoid a cache miss
	      //Reduce misses m < i*j in B by storing in temp instead of missing in B[j][i]
	      temp = A[i][j];
	      diag = i;
	    }
	  }
	  // If at diagonal, sets the diagonal we held on to before
	  if (rowBlock == colBlock) {
	    //Misses in B reduced to m < i
	    B[diag][diag] = temp;
	  }
        }
      }
    }
  }
  // if 64x64 matrix
  else if (N == 64) {
    // Iterate through matrix using column-major iteration over blocks
    // Goes through each column block in matrix
    for (colBlock = 0; colBlock < N; colBlock += 4) {
      // Goes through each row block in matrix
      for (rowBlock = 0; rowBlock < N; rowBlock += 4) {
        // Iterate over each row using row-major iteration
        // Goes through each row in the block
	for (i = rowBlock; i < rowBlock + 4; i++) {
          // Goes through each column in that row
	  for (j = colBlock; j < colBlock + 4; j++) {
            // Checks if diagonal
	    if (i != j) { // if not, sets tranposed element to correct place
		B[j][i] = A[i][j];
	    }
	    else { // otherwise holds on to it to avoid a cache miss
		//Reduce misses m < i*j in B by storing in temp instead of missing in B[j][i]
		temp = A[i][j];
		diag = i;
	    }
	  }
          // If at diagonal, sets the diagonal we held on to before
	  if (rowBlock == colBlock) {
		//Misses in B reduced to m < i
		B[diag][diag] = temp;
	  }
	}
      }
    }
  }
  // other odd shaped matrix
  else {
    // Iterate through matrix using column-major iteration over blocks
    // Goes through each column block in matrix
    for (colBlock = 0; colBlock < M; colBlock += 16) {
      // Goes through each row block in matrix
      for (rowBlock = 0; rowBlock < N; rowBlock += 16) {
        // Iterate over each row using row-major iteration
	// Since our sizes are prime, not all blocks will be square sub-matrices
	// Consider corner-case when (rowBlock + 16 > N) => invalid access. Explicit check for i, j < n, m
	for (i = rowBlock; (i < rowBlock + 16) && (i < N); i ++) {
          // Same for columns also
	  for (j = colBlock; (j < colBlock + 16) && (j < M); j ++) {
            // Checks if diagonal
	    if (i != j) { // if not, sets tranposed element to correct place
		B[j][i] = A[i][j];
	    }
	    else { // otherwise holds on to it to avoid a cache miss
		//Reduce misses m < i*j in B by storing in temp instead of missing in B[j][i]
		temp = A[i][j];
		diag = i;
            }
	  }
	  // If at diagonal, sets the diagonal we held on to before
	  if (rowBlock == colBlock) {
	     //Misses in B reduced to m < i
	     B[diag][diag] = temp;
	  }
        }
      }
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
