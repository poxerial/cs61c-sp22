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
    int column_num = n % blocksize;     //for the rest without block

    for (int x = 0; x < n; x += blocksize) {
        for (int y = 0; y < n; y += blocksize) {
            for (int i = 0; i < blocksize; i++) {
                int src_base = x + (i + y) * n;
                int dst_base = i + y + x * n;
                for (int j = 0; j < blocksize; j++) {
                    dst[dst_base + j * n] = src[src_base + j];
                }
            }
            if (column_num) {
                int row_num = x + blocksize < n ? blocksize : column_num;
                for (int i = 0; i < row_num; i++) {
                    int src_base = x + (i + y) * n;
                    int dst_base = y + x * n + i;
                    for (int j = 0; j < column_num; j++) {
                        dst[dst_base + j * n] = src[src_base + j];
                    }
                }
            }
        }
    }
}
