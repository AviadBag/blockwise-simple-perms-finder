#include "common.h"

/**
 * Calls the given calback for every permutation of length N (defined in common.h)
 * Returns how many <true> results the callback returned.
*/
unsigned long loop_perms(bool (*callback)(char*));