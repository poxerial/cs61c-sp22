#include <emmintrin.h>
#include <time.h>
#include <stdio.h>
#include <x86intrin.h>
#include "simd.h"

long long int sum(int vals[NUM_ELEMS]) {
    clock_t start = clock();

    long long int sum = 0;
    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        for(unsigned int i = 0; i < NUM_ELEMS; i++) {
            if(vals[i] >= 128) {
                sum += vals[i];
            }
        }
    }
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return sum;
}

long long int sum_unrolled(int vals[NUM_ELEMS]) {
    clock_t start = clock();
    long long int sum = 0;

    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        for(unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
            if(vals[i] >= 128) sum += vals[i];
            if(vals[i + 1] >= 128) sum += vals[i + 1];
            if(vals[i + 2] >= 128) sum += vals[i + 2];
            if(vals[i + 3] >= 128) sum += vals[i + 3];
        }

        // TAIL CASE, for when NUM_ELEMS isn't a multiple of 4
        // NUM_ELEMS / 4 * 4 is the largest multiple of 4 less than NUM_ELEMS
        // Order is important, since (NUM_ELEMS / 4) effectively rounds down first
        for(unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++) {
            if (vals[i] >= 128) {
                sum += vals[i];
            }
        }
    }
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return sum;
}

long long int sum_simd(int vals[NUM_ELEMS]) {
    clock_t start = clock();
    __m128i _127 = _mm_set1_epi32(127); // This is a vector with 127s in it... Why might you need this?
    long long int result = 0; // This is where you should put your final result!
    /* DO NOT MODIFY ANYTHING ABOVE THIS LINE (in this function) */

    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        __m128i sum = _mm_setzero_si128();
        unsigned int i_sup = NUM_ELEMS / 4 * 4;
        for(unsigned int i = 0; i < i_sup; i += 4) {
            __m128i val_i = _mm_loadu_si128((__m128i_u *)(vals + i));
            sum = _mm_add_epi32(sum, _mm_and_si128(_mm_cmpgt_epi32(val_i, _127), val_i));
        }

        int result_vec4[4];
        _mm_storeu_si128((__m128i_u *)result_vec4, sum);
        for (int i = 0; i < 4; i++)
            result += result_vec4[i];

        /* Hint: you'll need a tail case. */
        for (int i = NUM_ELEMS - 1; i >= i_sup; i--)
            if (vals[i] >= 128)
                result += vals[i];
    }

    /* DO NOT MODIFY ANYTHING BELOW THIS LINE (in this function) */
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return result;
}

long long int sum_simd_unrolled(int vals[NUM_ELEMS]) {
    clock_t start = clock();
    __m128i _127 = _mm_set1_epi32(127);
    long long int result = 0;
    /* DO NOT MODIFY ANYTHING ABOVE THIS LINE (in this function) */

    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        /* YOUR CODE GOES HERE */
        __m128i sum = _mm_setzero_si128();
        unsigned int i_sup = NUM_ELEMS / 16 * 16;
        for(unsigned int i = 0; i < i_sup; i += 16) {
            __m128i val_i = _mm_loadu_si128((__m128i_u *)(vals + i));
            sum =_mm_add_epi32(sum, _mm_and_si128(_mm_cmpgt_epi32(val_i, _127), val_i));
            val_i = _mm_loadu_si128((__m128i_u *)(vals + i + 4));
            sum = _mm_add_epi32(sum, _mm_and_si128(_mm_cmpgt_epi32(val_i, _127), val_i));
            val_i = _mm_loadu_si128((__m128i_u *)(vals + i + 8));
            sum = _mm_add_epi32(sum, _mm_and_si128(_mm_cmpgt_epi32(val_i, _127), val_i));
            val_i = _mm_loadu_si128((__m128i_u *)(vals + i + 12));
            sum = _mm_add_epi32(sum, _mm_and_si128(_mm_cmpgt_epi32(val_i, _127), val_i));
        }

        int result_vec4[4];
        _mm_storeu_si128((__m128i_u *)result_vec4, sum);
        for (int i = 0; i < 4; i++)
            result += result_vec4[i];

        /* Hint: you'll need a tail case. */
        float result_f = 0;

        for (int i = NUM_ELEMS - 1; i >= NUM_ELEMS / 16 * 16; i--)
            if (vals[i] >= 128)
                result += vals[i];
    }

    /* DO NOT MODIFY ANYTHING BELOW THIS LINE (in this function) */
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return result;
}
