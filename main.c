#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/sysinfo.h>
#include <inttypes.h>

#define N 10

typedef struct t_permutation
{
    int length;
    char* data;
} permutation;

typedef struct t_thread_data
{
    uintmax_t from;
    uintmax_t to;
} thread_data;

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

uintmax_t factorial(int n)
{
    uintmax_t result = 1;
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

bool check_block_decomposition(permutation block)
{
    // Seperator represents after what index to seperate
    for (int seperator = 0; seperator < block.length - 1; seperator++)
    {
        permutation block1 = {seperator+1, block.data};
        permutation block2 = {block.length - block1.length, block.data + seperator + 1};

        if (is_block(block1) && is_block(block2))
            if (check_plus_minus(block1, block2))
                return true;
    }

    return false;
}

bool check_perm(char* perm)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = i+1; j < N; j++)
        {
            int length = j - i + 1;

            permutation sub_perm;
            sub_perm.data = perm + i;
            sub_perm.length = length;
            
            if (is_block(sub_perm))
                if (check_block_decomposition(sub_perm)) return false;
        }
    }
    
    return true;
}

// Puts in the given array the [i] permutation of length N
// Alerted from https://stackoverflow.com/a/7919887
void get_permutation(int i, char* perm)
{
   int j, k = 0;
   int fact[N];

   // compute factorial numbers
   fact[k] = 1;
   while (++k < N)
      fact[k] = fact[k - 1] * k;

   // compute factorial code
   for (k = 0; k < N; ++k)
   {
      perm[k] = i / fact[N - 1 - k] + 1;
      i = i % fact[N - 1 - k];
   }

   // readjust values to obtain the permutation
   // start from the end and check if preceding values are lower
   for (k = N - 1; k > 0; --k)
      for (j = k - 1; j >= 0; --j)
         if (perm[j] <= perm[k])
            perm[k]++;
}

// arg is of type thread_data
void* check_permutations(void* arg)
{
    thread_data* data = (thread_data*) arg;
    uintmax_t no_of_perms = data->to - data->from + 1;
    uintmax_t* result = malloc(sizeof(uintmax_t));
    *result = 0;
    char perm[N];
    for (uintmax_t i = data->from; i <= data->to; i++)
    {
        get_permutation(i, perm);
        if (check_perm(perm)) (*result)++;
    }

    printf("RESULT=%ju\n", *result);
    return result;
}

int main()
{
    uintmax_t no_of_perms = factorial(N);
    printf("There are %ju permutations for n=%d\n", no_of_perms, N);

    // We are leaving one core free
    int threads = get_nprocs() - 1;
    printf("Running on %d cores\n", threads);

    if (no_of_perms < threads)
    {
        printf("You have more threads than perms.. Probably N is TOO low\n");
        return 0;
    }

    uintmax_t perms_per_thread = no_of_perms / threads;
    printf("%ju perms per thread\n", perms_per_thread);
    
    uintmax_t done = 0;
    pthread_t* tids = calloc(threads, sizeof(pthread_t));

    // Create a thread for every core
    uintmax_t from = 0;
    for (int i = 0; i < threads; i++, from += perms_per_thread)
    {
        thread_data* data = malloc(sizeof(thread_data));
        data->from = from;
        data->to = from + perms_per_thread - 1;
        done += perms_per_thread;
        pthread_create(tids + i, NULL, check_permutations, data);
    }

    // Wait for all the threads and get results
    uintmax_t result = 0;
    for (int i = 0; i < threads; i++)
    {
        uintmax_t* return_value;
        pthread_join(tids[i], (void**) &return_value);
        result += *return_value;
        free(return_value);
    }

    // Do leftovers
    printf("There are left %ju perms to check\n", no_of_perms - done);
    thread_data data = {from: done, to: no_of_perms-1};
    uintmax_t* r = check_permutations(&data);
    result += *r;
    free(r);

    printf("\n----------------\nFINAL RESULT=%ju\n", result);

    free(tids);
    return 0;
}