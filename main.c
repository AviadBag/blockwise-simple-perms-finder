#include <stdio.h>
#include <stdlib.h>

#include "perms.h"
#include "common.h"

typedef struct t_permutation
{
    int length;
    char* data;
} permutation;

void print_perm(permutation perm)
{
    for (int i = 0; i < perm.length; i++)
    {
        if (i < perm.length-1)
            printf("%d, ", perm.data[i]);
        else
            
            printf("%d\n", perm.data[i]);
    }
}

unsigned long factorial(int n)
{
    unsigned long result = 1;
    for (int i = 1; i <= n; i++)
        result *= i;

    return result;
}

int perm_max(permutation perm)
{
    int max = 0;
    for (int i = 0; i < perm.length; i++)
        if (perm.data[i] > max)
            max = perm.data[i];

    return max;
}

int perm_min(permutation perm)
{
    int min = perm.data[0];
    for (int i = 0; i < perm.length; i++)
        if (perm.data[i] < min)
            min = perm.data[i];

    return min;
}

bool is_block(permutation sub_perm)
{
    int max = perm_max(sub_perm);
    int min = perm_min(sub_perm);

    return (max - min + 1 == sub_perm.length);
}

bool check_plus_minus(permutation block1, permutation block2)
{
    int max1 = perm_max(block1);
    int max2 = perm_max(block2);
    int min1 = perm_min(block1);
    int min2 = perm_min(block2);

    return (max1 < min2) || (min1 > max2);
}

bool check_perm(char* perm)
{
    // printf("----- CHECKING FOR PERMUTATION ------\n");
    // print_perm(perm, N);
    int old_i;
    for (int i = 0; i < N; i++)
    {
        old_i = i;
        permutation first_block;
        first_block.length = 0;
        for (int j = i; j < N; j++)
        {
            int length = j - i + 1;
            if (length == N)
                continue; // The entire permutation is not considered a block

            permutation sub_perm;
            sub_perm.data = perm + i;
            sub_perm.length = length;
            if (is_block(sub_perm))
            {
                if (first_block.length > 0)
                {
                    // We found two blocks in a row
                    permutation entire_block = {first_block.length + sub_perm.length, first_block.data};
                    if (check_plus_minus(first_block, sub_perm) && is_block(entire_block)) return false;
                }
                else
                {
                    first_block = sub_perm;
                    old_i = i;
                    i = j+1;
                }
            }
        }
        i = old_i;
    }    

    permutation s_perm;
    s_perm.length = N;
    s_perm.data = perm;
    print_perm(s_perm);
    
    return true;
}

int main()
{
    unsigned long no_of_perms = factorial(N);
    printf("There are %lu permutations for n=%d\n", no_of_perms, N);

    printf("RESULT=%lu\n", loop_perms(check_perm));
    
    return 0;
}