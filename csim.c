#include "cachelab.h"

// Nils Backe nabacke
// Varnika Sinha vsinha2
// Team name: i_need_arrays

/* Ideas for cache:
   - each line should be a struct
   - each set should be an array of structs 
   - cache should be an array of arrays of structs (2D array of structs)
   - function to manipulate the cache (adding and flushing) 
*/

int main()
{
    printSummary(0, 0, 0);
    return 0;
}

void printSummary(int hits, int misses, int evictions) {
	printf("hits:%d misses:%d evictions%d", hits, misses, evictions);
}
