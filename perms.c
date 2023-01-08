#include <string.h>

#include "common.h"

/* Function to swap values at two pointers */
void swap(char* x, char* y)
{
    char temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

unsigned long _loop_perms(char* a, int l, int r, bool (*callback)(char*))
{
    static unsigned long offset = 0;
    unsigned long count = 0;
    int i;
    if (l == r)
    {
        if (callback(a))
            count++;
    }
    else 
    {
        for (i = l; i <= r; i++) 
        {
            swap((a + l), (a + i));
            count += _loop_perms(a, l + 1, r, callback);
            swap((a + l), (a + i)); // backtrack
        }
    }
    
    return count;
}

unsigned long loop_perms(bool (*callback)(char*))
{
    char original_perm[N];
    for (int i = 0; i < N; i++)
        original_perm[i] = i+1;

    return _loop_perms(original_perm, 0, N-1, callback);
}