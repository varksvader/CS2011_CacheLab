#include <stdio.h>
#include "cachelab.h"

// Nils Backe nabacke
// Varnika Sinha vsinha2
// Team name: i_need_arrays

/* Remember all commenting rules apply!
   - Create header comments for functions
*/

/* Ideas/Outline:
   - Create a function reading the command line arguments, creating various
     types of caches (possibly create a function that prints out the usage if
     user enters it wrong)
   - Cache Structure
      - each line should be a struct
      - each set should be (a struct containing) array of structs
      - cache should be an array of arrays of structs (2D array of structs)
   - Other functions manipulating the cache
      - Build up cache (when first running program)
      - Searching for specific data (getting a hit/miss)
      - Flushing data (LRU) when full
   - Print out performance of cache at the end (use a struct to keep track)
*/

// Memory address var specifically to hold 64 bits
typedef unsigned long long int mem_addr_t;

// Structs
// Line
typedef struct {
  int valid;
  mem_addr_t tag;
  char* data;
  int lastUsed;
} cache_line;

// Set
typedef struct {
  cache_line *lines;
} cache_set;

// Cache
typedef struct {
  cache_set *sets;
} cache;

int main() {
    printSummary(0, 0, 0);
    return 0;
}

/**
 * Prints out usage information of command line arguments
 */
 void printUsage(void) {
   printf("Usage: ./csim-ref [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n");
   printf("-h: Optional help flag that prints usage info\n");
   printf("-v: Optional verbose flag that displays trace info\n");
   printf("-s <s>: Number of index bits (number of sets is 2^s)\n");
   printf("-E <E>: Associativity (number of lines per set)\n");
   printf("-b <b>: Number of block bits (the block size is 2^b)\n");
   printf("-t <tracefile>: Name of valgrind trace to replay\n");
 }

/**
 * Prints out summary of the cache
 * @param hits number of hits from the cache
 * @param misses number of misses from the cache
 * @param evictions number of evictions from the cache
 */
void printSummary(int hits, int misses, int evictions) {
	printf("hits:%d misses:%d evictions%d", hits, misses, evictions);
}
