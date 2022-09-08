#include "matrix.h"
#include <omp.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

// Include SSE intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <immintrin.h>
#include <x86intrin.h>
#endif

/* Below are some intel intrinsics that might be useful
 * void _mm256_storeu_pd (double * mem_addr, __m256d a)
 * __m256d _mm256_set1_pd (double a)
 * __m256d _mm256_set_pd (double e3, double e2, double e1, double e0)
 * __m256d _mm256_loadu_pd (double const * mem_addr)
 * __m256d _mm256_add_pd (__m256d a, __m256d b)
 * __m256d _mm256_sub_pd (__m256d a, __m256d b)
 * __m256d _mm256_fmadd_pd (__m256d a, __m256d b, __m256d c)
 * __m256d _mm256_mul_pd (__m256d a, __m256d b)
 * __m256d _mm256_cmp_pd (__m256d a, __m256d b, const int imm8)
 * __m256d _mm256_and_pd (__m256d a, __m256d b)
 * __m256d _mm256_max_pd (__m256d a, __m256d b)
 */

/* Generates a random double between low and high */
double rand_double(double low, double high) {
  double range = (high - low);
  double div = RAND_MAX / range;
  return low + (rand() / div);
}

/* Generates a random matrix */
void rand_matrix(matrix *result, unsigned int seed, double low, double high) {
  srand(seed);
  for (int i = 0; i < result->rows; i++) {
    for (int j = 0; j < result->cols; j++) {
      set(result, i, j, rand_double(low, high));
    }
  }
}

/*
 * Returns the double value of the matrix at the given row and column.
 * You may assume `row` and `col` are valid. Note that the matrix is in
 * row-major order.
 */
double get(matrix *mat, int row, int col) {
  return mat->data[row * mat->cols + col];
}

/*
 * Sets the value at the given row and column to val. You may assume `row` and
 * `col` are valid. Note that the matrix is in row-major order.
 */
void set(matrix *mat, int row, int col, double val) {
  mat->data[row * mat->cols + col] = val;
}

/*
 * Allocates space for a matrix struct pointed to by the double pointer mat with
 * `rows` rows and `cols` columns. You should also allocate memory for the data
 * array and initialize all entries to be zeros. `parent` should be set to NULL
 * to indicate that this matrix is not a slice. You should also set `ref_cnt`
 * to 1. You should return -1 if either `rows` or `cols` or both have invalid
 * values. Return -2 if any call to allocate memory in this function fails.
 * Return 0 upon success.
 */
int allocate_matrix(matrix **mat, int rows, int cols) {
  // Task 1.2 TODO
  // HINTS: Follow these steps.
  // 1. Check if the dimensions are valid. Return -1 if either dimension is not
  // positive.
  // 2. Allocate space for the new matrix struct. Return -2 if allocating memory
  // failed.
  // 3. Allocate space for the matrix data, initializing all entries to be 0.
  // Return -2 if allocating memory failed.
  // 4. Set the number of rows and columns in the matrix struct according to the
  // arguments provided.
  // 5. Set the `parent` field to NULL, since this matrix was not created from a
  // slice.
  // 6. Set the `ref_cnt` field to 1.
  // 7. Store the address of the allocated matrix struct at the location `mat`
  // is pointing at.
  // 8. Return 0 upon success.
  if (rows <= 0 || cols <= 0)
    return -1;

  *mat = (matrix *)malloc(sizeof(matrix));
  if (!mat)
    return -2;

  (*mat)->data = (double *)calloc(rows * cols, sizeof(double));
  if (!(*mat)->data)
    return -2;

  (*mat)->rows = rows;
  (*mat)->cols = cols;
  (*mat)->parent = NULL;
  (*mat)->ref_cnt = 1;

  return 0;
}

/*
 * You need to make sure that you only free `mat->data` if `mat` is not a slice
 * and has no existing slices, or that you free `mat->parent->data` if `mat` is
 * the last existing slice of its parent matrix and its parent matrix has no
 * other references (including itself).
 */
void deallocate_matrix(matrix *mat) {
  // Task 1.3 TODO
  // HINTS: Follow these steps.
  // 1. If the matrix pointer `mat` is NULL, return.
  if (!mat)
    return;
  // 2. If `mat` has no parent: decrement its `ref_cnt` field by 1. If the
  // `ref_cnt` field becomes 0, then free `mat` and its `data` field.
  if (!mat->parent) {
    mat->ref_cnt--;
    if (mat->ref_cnt == 0) {
      free(mat->data);
      free(mat);
    }
  } else {
    deallocate_matrix(mat->parent);
    free(mat);
  }
  // 3. Otherwise, recursively call `deallocate_matrix` on `mat`'s parent, then
  // free `mat`.
}

/*
 * Allocates space for a matrix struct pointed to by `mat` with `rows` rows and
 * `cols` columns. Its data should point to the `offset`th entry of `from`'s
 * data (you do not need to allocate memory) for the data field. `parent` should
 * be set to `from` to indicate this matrix is a slice of `from` and the
 * reference counter for `from` should be incremented. Lastly, do not forget to
 * set the matrix's row and column values as well. You should return -1 if
 * either `rows` or `cols` or both have invalid values. Return -2 if any call to
 * allocate memory in this function fails. Return 0 upon success. NOTE: Here
 * we're allocating a matrix struct that refers to already allocated data, so
 * there is no need to allocate space for matrix data.
 */
int allocate_matrix_ref(matrix **mat, matrix *from, int offset, int rows,
                        int cols) {
  // Task 1.4 TODO
  // HINTS: Follow these steps.
  // 1. Check if the dimensions are valid. Return -1 if either dimension is not
  // positive.
  // 2. Allocate space for the new matrix struct. Return -2 if allocating memory
  // failed.
  // 3. Set the `data` field of the new struct to be the `data` field of the
  // `from` struct plus `offset`.
  // 4. Set the number of rows and columns in the new struct according to the
  // arguments provided.
  // 5. Set the `parent` field of the new struct to the `from` struct pointer.
  // 6. Increment the `ref_cnt` field of the `from` struct by 1.
  // 7. Store the address of the allocated matrix struct at the location `mat`
  // is pointing at.
  // 8. Return 0 upon success.
  if (rows <= 0 || cols <= 0)
    return -1;

  matrix *new_mat = (matrix *)malloc(sizeof(matrix));
  if (!new_mat)
    return -2;

  new_mat->data = from->data + offset;

  new_mat->cols = cols;
  new_mat->rows = rows;
  new_mat->parent = from;
  from->ref_cnt++;

  *mat = new_mat;

  return 0;
}

/*
 * Sets all entries in mat to val. Note that the matrix is in row-major order.
 */
void fill_matrix(matrix *mat, double val) {
  int num = mat->cols * mat->rows;
  __m256d val_vec4 = _mm256_set1_pd(val);
  int num_threads;
  int i_begin = 0, i_end = 0;
#pragma omp parallel private(i_begin, i_end)
  {
    num_threads = omp_get_num_threads();
    i_begin = num / num_threads * omp_get_thread_num();
    i_end = num / num_threads * (omp_get_thread_num() + 1);
    for (int i = i_begin; i < i_end; i += 4) {
      _mm256_storeu_pd(mat->data + i, val_vec4);
    }
    for (int i = i_end - 1; i >= i_end - i_end % 4; i--) {
      mat->data[i] = val;
    }
  }
  for (int i = num - 1; i >= num - num % num_threads; i--)
    mat->data[i] = val;
}

/*
 * Store the result of taking the absolute value element-wise to `result`.
 * Return 0 upon success.
 * Note that the matrix is in row-major order.
 */
int abs_matrix(matrix *result, matrix *mat) {
  int num = mat->cols * mat->rows;
  unsigned long zero_uint64[4] = {0x7fffffffffffffff, 0x7fffffffffffffff,
                                  0x7fffffffffffffff, 0x7fffffffffffffff};
  __m256d zero = _mm256_loadu_pd((double *)zero_uint64);
  int i_begin, i_end, num_threads;
#pragma omp parallel private(i_begin, i_end) shared(num_threads)
  {
    num_threads = omp_get_num_threads();
    i_begin = num / num_threads * omp_get_thread_num();
    i_end = num / num_threads * (omp_get_thread_num() + 1);
    for (int i = i_begin; i < i_end; i += 4) {
      __m256d src = _mm256_loadu_pd(mat->data + i);
      __m256d rst = _mm256_and_pd(zero, src);
      _mm256_storeu_pd(result->data + i, rst);
    }
    for (int i = i_end - 1; i >= i_end - i_end % 4; i--) {
      result->data[i] = mat->data[i] >= 0 ? mat->data[i] : -mat->data[i];
    }
  }
  for (int i = num - 1; i >= num - num % num_threads; i--)
    result->data[i] = mat->data[i] >= 0 ? mat->data[i] : -mat->data[i];

  return 0;
}

/*
 * (OPTIONAL)
 * Store the result of element-wise negating mat's entries to `result`.
 * Return 0 upon success.
 * Note that the matrix is in row-major order.
 */
int neg_matrix(matrix *result, matrix *mat) {
  int num = mat->cols * mat->rows;
  unsigned long zero_uint64[4] = {0x8000000000000000, 0x8000000000000000,
                                  0x8000000000000000, 0x8000000000000000};
  __m256d zero = _mm256_loadu_pd((double *)zero_uint64);
  for (int i = 0; i <= num - 4; i += 4) {
    __m256d src = _mm256_loadu_pd(mat->data + i);
    __m256d rst = _mm256_or_pd(zero, src);
    _mm256_storeu_pd(result->data + i, rst);
  }
  for (int i = num - 1; i >= num - (num) % 4; i--) {
    result->data[i] = -mat->data[i];
  }
  return 0;
}

/*
 * Store the result of adding mat1 and mat2 to `result`.
 * Return 0 upon success.
 * You may assume `mat1` and `mat2` have the same dimensions.
 * Note that the matrix is in row-major order.
 */
int add_matrix(matrix *result, matrix *mat1, matrix *mat2) {
  int num = result->cols * result->rows;
  int i_begin, i_end, num_threads;
#pragma omp parallel private(i_begin, i_end) shared(num_threads)
  {
    num_threads = omp_get_num_threads();
    i_begin = num / num_threads * omp_get_thread_num();
    i_end = num / num_threads * (omp_get_thread_num() + 1);
    for (int i = i_begin; i < i_end; i += 4) {
      __m256d src1 = _mm256_loadu_pd(mat1->data + i);
      __m256d src2 = _mm256_loadu_pd(mat2->data + i);
      __m256d rst = _mm256_add_pd(src1, src2);
      _mm256_storeu_pd(result->data + i, rst);
    }
    for (int i = i_end - 1; i >= i_end - i_end % 4; i--) {
      result->data[i] = mat1->data[i] + mat2->data[i];
    }
  }
  for (int i = num - 1; i >= num - num % num_threads; i--)
    result->data[i] = mat1->data[i] + mat2->data[i];

  return 0;
}

/*
 * (OPTIONAL)
 * Store the result of subtracting mat2 from mat1 to `result`.
 * Return 0 upon success.
 * You may assume `mat1` and `mat2` have the same dimensions.
 * Note that the matrix is in row-major order.
 */
int sub_matrix(matrix *result, matrix *mat1, matrix *mat2) {
  for (int i = 0; i < result->cols * result->rows; i++)
    result->data[i] = mat1->data[i] - mat2->data[i];
  return 0;
}

/*
 * Store the result of multiplying mat1 and mat2 to `result`.
 * Return 0 upon success.
 * Remember that matrix multiplication is not the same as multiplying individual
 * elements. You may assume `mat1`'s number of columns is equal to `mat2`'s
 * number of rows. Note that the matrix is in row-major order.
 */
void transpose_blocking(int rows, int cols, double *dst, double *src) {
  int blocksize = 16;
  for (int x = 0; x < cols; x += blocksize) {
    for (int y = 0; y < rows; y += blocksize) {
      for (int i = 0; i < (y + blocksize < rows ? blocksize : rows - y); i++) {
        int src_base = x + (i + y) * cols;
        int dst_base = i + y + x * rows;
        for (int j = 0; j < (x + blocksize < cols ? blocksize : cols - x);
             j++) {
          dst[dst_base + j * rows] = src[src_base + j];
        }
      }
    }
  }
}

int mul_matrix(matrix *result, matrix *mat1, matrix *mat2) {
  int i, j, k, i_begin, i_end, num_threads;
  __m256d sum;
  double *mat2_T = (double *)malloc(mat2->cols * mat2->rows * sizeof(double));
  transpose_blocking(mat2->rows, mat2->cols, mat2_T, mat2->data);
#pragma omp parallel private(i, j, k, i_begin, i_end, sum) shared(num_threads)
  {
    num_threads = omp_get_num_threads();
    i_begin = result->rows / num_threads * omp_get_thread_num();
    i_end = result->rows / num_threads * (omp_get_thread_num() + 1);
    if (omp_get_thread_num() == num_threads - 1)
      i_end = result->rows;
    for (i = i_begin; i < i_end; i++) {
      for (j = 0; j < result->cols; j++) {
        sum = _mm256_setzero_pd();
        for (k = 0; k <= mat1->cols - 4; k += 4) {
          sum = _mm256_add_pd(
              sum,
              _mm256_mul_pd(_mm256_loadu_pd(mat1->data + j * mat1->cols + k),
                            _mm256_loadu_pd(mat2_T + i * mat2->rows + k)));
        }
        double rst = 0.0;
        double rst_vec[4];
        _mm256_storeu_pd(rst_vec, sum);
        for (int k = 0; k < 4; k++)
          rst += rst_vec[k];
        for (int k = mat1->cols - 1; k >= mat1->cols - mat1->cols % 4; k--)
          rst += mat1->data[j * mat1->cols + k] * mat2_T[i * mat2->rows + k];
        result->data[i + j * result->rows] = rst;
      }
    }
  }
  free(mat2_T);
  return 0;
}

/*
 * Store the result of raising mat to the (pow)th power to `result`.
 * Return 0 upon success.
 * Remember that pow is defined with matrix multiplication, not element-wise
 * multiplication. You may assume `mat` is a square matrix and `pow` is a
 * non-negative integer. Note that the matrix is in row-major order.
 */

//shitcode
void pow_matrix_two_pows(double *result, double *mat, int rows_cols,
                         int two_pow) {

  matrix rst = {.cols = rows_cols, .rows = rows_cols, .data = result};

  double *temp_data = (double *)malloc(rows_cols * rows_cols * sizeof(double));
  matrix temp = {.cols = rows_cols, .rows = rows_cols, .data = temp_data};

  memcpy(result, mat, rows_cols * rows_cols * sizeof(double));
  int flag_next_is_temp = 1;

  for (; two_pow > 0; two_pow--) {
    if (flag_next_is_temp) {
      mul_matrix(&temp, &rst, &rst);
      flag_next_is_temp = 0;
    } else {
      mul_matrix(&rst, &temp, &temp);
      flag_next_is_temp = 1;
    }
  }
  if (!flag_next_is_temp)
    memcpy(result, temp_data, rows_cols * rows_cols * sizeof(double));
  free(temp_data);
}

int pow_matrix(matrix *result, matrix *mat, int pow) {
  if (mat->cols != mat->rows || mat->cols != result->cols ||
      result->cols != result->rows)
    return -1;

  double *result_temp_data = (double *)calloc(mat->cols * mat->rows, sizeof(double));
  matrix result_temp_m = {.cols = mat->cols, .rows = mat->rows, .data = result_temp_data};
  matrix *result_temp = &result_temp_m;

  memset(result->data, 0, result->cols * result->rows);
  memset(result_temp->data, 0, result->cols * result->rows);
  for (int i = 0; i < result->cols; i++) {
    set(result, i, i, 1.0);
    set(result_temp, i, i, 1.0);
  }

  double *temp1 = (double *)malloc(mat->cols * mat->rows * sizeof(double));
  double *temp2 = (double *)malloc(mat->cols * mat->rows * sizeof(double));
  memcpy(temp1, mat->data, result->cols * result->rows * sizeof(double));
  matrix temp1_m = {.cols = mat->cols, .rows = mat->rows, .data = temp1};
  matrix temp2_m = {.cols = mat->cols, .rows = mat->rows, .data = temp2};

  int flag_next_is_temp2 = 1;
  int flag_result_is_not_result_temp = 0;
  unsigned u_pow = pow;
  int i = 0;

  while (u_pow > 0) {
    for (; i < 32 && u_pow > 0; i++) {
      if (u_pow & 1)
        {
          matrix *src = flag_next_is_temp2 ? &temp1_m : &temp2_m;
          matrix *rst = flag_next_is_temp2 ? &temp2_m : &temp1_m;
          pow_matrix_two_pows(rst->data, src->data, mat->cols, i);
          flag_next_is_temp2 = flag_next_is_temp2 ? 0 : 1;
          
          matrix *result_next = flag_result_is_not_result_temp ? result_temp : result;
          src = flag_result_is_not_result_temp ? result : result_temp;
          mul_matrix(result_next, rst, src);
          flag_result_is_not_result_temp = flag_result_is_not_result_temp ? 0 : 1;

          u_pow = u_pow >> 1;
          i = 1;
          break;
        }
      u_pow = u_pow >> 1;
    }
  }
  if (!flag_result_is_not_result_temp)
    memcpy(result->data, result_temp->data, result->cols * result->rows * sizeof(double));

  free(temp1);
  free(temp2);
  return 0;
}
