#include "cachelab.h"

// Nils Backe nabacke
// Varnika Sinha vsinha2
// Team name: i_need_arrays

int main()
{
    printSummary(0, 0, 0);
    return 0;
}

void printSummary(int hits, int misses, int evictions) {
	printf("hits:%d misses:%d evictions%d", hits, misses, evictions);
}
