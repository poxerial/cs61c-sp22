#include "transpose.h"

/* The naive transpose function as a reference. */
void transpose_naive(int n, int blocksize, int *dst, int *src) {
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            dst[y + x * n] = src[x + y * n];
        }
    }
}

/* Implement cache blocking below. You should NOT assume that n is a
 * multiple of the block size. */
void transpose_blocking(int n, int blocksize, int *dst, int *src) {
    for (int x = 0; x < n; x += blocksize) {
        for (int y = 0; y < n; y += blocksize) {
            for (int i = 0; i < (y + blocksize < n ? blocksize : n - y); i++) {
                int src_base = x + (i + y) * n;
                int dst_base = i + y + x * n;
                for (int j = 0; j < (x + blocksize < n ? blocksize : n - x); j++) {
                    dst[dst_base + j * n] = src[src_base + j];
                }
            }
        }
    }
}
