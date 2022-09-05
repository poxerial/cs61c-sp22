#include "omp_apps.h"

// use pthread instead
#include <pthread.h>

/* -------------------------------Utilties, Do Not
 * Modify------------------------------*/
double *gen_array(int n) {
  double *array = (double *)malloc(n * sizeof(double));
  for (int i = 0; i < n; i++)
    array[i] = drand48();
  return array;
}

int verify(double *x, double *y,
           void (*funct)(double *x, double *y, double *z)) {
  double *z_v_add = (double *)malloc(ARRAY_SIZE * sizeof(double));
  double *z_oracle = (double *)malloc(ARRAY_SIZE * sizeof(double));
  (*funct)(x, y, z_v_add);
  for (int i = 0; i < ARRAY_SIZE; i++) {
    z_oracle[i] = x[i] + y[i];
  }
  for (int i = 0; i < ARRAY_SIZE; i++) {
    if (z_oracle[i] != z_v_add[i]){
        printf("at index %d: %lf != %lf\n", i, z_oracle[i], z_v_add[i]);
      return 0;
    }
  }
  return 1;
}

/* -------------------------------Vector
 * Addition------------------------------*/
typedef struct {
  double *x;
  double *y;
  double *z;
  int thread_id;
} v_add_args;

void v_add_naive(double *x, double *y, double *z) {
#pragma omp parallel
  {
    for (int i = 0; i < ARRAY_SIZE; i++)
      z[i] = x[i] + y[i];
  }
}

// Adjacent Method
void *v_add_adjacent(void *args) {
  v_add_args *inner = (v_add_args *)args;
  double *x = inner->x;
  double *y = inner->y;
  double *z = inner->z;
  int thread_id = inner->thread_id;
  for (int i = thread_id; i < ARRAY_SIZE; i += 12) {
    z[i] = x[i] + y[i];
  }
  return NULL;
}

void *v_add_chunks(void *args) {
  v_add_args *inner = (v_add_args *)args;
  double *x = inner->x;
  double *y = inner->y;
  double *z = inner->z;
  int thread_id = inner->thread_id;

  int i_begin = ARRAY_SIZE / 12 * thread_id;
  int i_end = ARRAY_SIZE / 12 * (thread_id + 1);
  for (int i = i_begin; i < i_end; i++) {
    z[i] = x[i] + y[i];
  }
  if (thread_id == 11)
    for (int i = i_end; i < ARRAY_SIZE; i++)
      z[i] = x[i] + y[i];
  return NULL;
}

void v_add_optimized_adjacent(double *x, double *y, double *z) {
  // TODO: Implement this function
  int threads_num = 12;

  pthread_t threads[threads_num];
  v_add_args argss[threads_num];
  for (int i = 0; i < 12; i++) {
    v_add_args args = {.x = x, .y = y, .z = z, .thread_id = i};
    argss[i] = args;
    pthread_create(threads + i, NULL, v_add_adjacent, (void *)(argss + i));
  }

  for (int i = 0; i < 12; i++) {
    pthread_join(threads[i], NULL);
  }
}

// Chunks Method
void v_add_optimized_chunks(double *x, double *y, double *z) {
  // TODO: Implement this function
  int threads_num = 12;
  pthread_t threads[threads_num];
  v_add_args argss[threads_num];
  for (int i = 0; i < 12; i++) {
    v_add_args args = {.x = x, .y = y, .z = z, .thread_id = i};
    argss[i] = args;
    pthread_create(threads + i, NULL, v_add_chunks, (void *)(argss + i));
  }

  for (int i = 0; i < threads_num; i++) {
    pthread_join(threads[i], NULL);
  }
}

/* -------------------------------Dot Product------------------------------*/
double dotp_naive(double *x, double *y, int arr_size) {
  double global_sum = 0.0;
#pragma omp parallel
  {
#pragma omp for
    for (int i = 0; i < arr_size; i++)
#pragma omp critical
      global_sum += x[i] * y[i];
  }
  return global_sum;
}

// Manual Reduction
typedef struct {
    double *x;
    double *y;
    int arr_size;
    int thread_id;
    double sum;
} dotp_args;

void *dotp(void *args) {
    double sum = 0.0;

    dotp_args *content = (dotp_args *)args;
    double *x = content->x;
    double *y = content->y;
    int arr_size = content->arr_size;
    int thread_id = content->thread_id;

    int i_begin = arr_size / 12 * thread_id;
    int i_end = arr_size / 12 * (thread_id + 1);
    for (int i = i_begin; i < i_end; i++)
        sum += x[i] * y[i];
    if (thread_id == 11)
        for (int i = i_end; i < arr_size; i++)
        sum += x[i] * y[i];
    
    content->sum = sum;

    return NULL;
}

double dotp_manual_optimized(double *x, double *y, int arr_size) {
  double global_sum = 0.0;
    int threads_num = 12;
    pthread_t threads[threads_num];
    dotp_args argss[threads_num];

    for (int i = 0; i < threads_num; i++)
    {
        dotp_args args = {.x = x, .y = y, .arr_size = arr_size, .thread_id = i, .sum = 0.0};
        argss[i] = args;
        pthread_create(threads + i, NULL, dotp, argss + i);
    }
    for (int i = 0; i < threads_num; i++)
    {
        pthread_join(threads[i], NULL);
        global_sum += argss[i].sum;
    }

// Do NOT use the `reduction` directive here!
  return global_sum;
}

// Reduction Keyword
double dotp_reduction_optimized(double *x, double *y, int arr_size) {
  double global_sum = 0.0;
  // TODO: Implement this function
  #pragma omp parallel for reduction(+:global_sum) 
  for (int i = 0; i < arr_size; i++)
    global_sum += x[i] * y[i];
  return global_sum;
}

char *compute_dotp(int arr_size) {
  // Generate input vectors
  char *report_buf = (char *)malloc(BUF_SIZE), *pos = report_buf;
  double start_time, run_time;

  double *x = gen_array(arr_size), *y = gen_array(arr_size);
  double serial_result = 0.0, result = 0.0;

  // calculate result serially
  for (int i = 0; i < arr_size; i++) {
    serial_result += x[i] * y[i];
  }

  int num_threads = omp_get_max_threads();
  for (int i = 1; i <= num_threads; i++) {
    omp_set_num_threads(i);
    start_time = omp_get_wtime();
    for (int j = 0; j < REPEAT; j++)
      result = dotp_manual_optimized(x, y, arr_size);
    run_time = omp_get_wtime() - start_time;
    pos += sprintf(pos, "Manual Optimized: %d thread(s) took %f seconds\n", i,
                   run_time);

    // verify result is correct (within some threshold)
    if (fabs(serial_result - result) > 0.001) {
      pos += sprintf(pos, "Incorrect result!\n");
      *pos = '\0';
      return report_buf;
    }
  }

  for (int i = 1; i <= num_threads; i++) {
    omp_set_num_threads(i);
    start_time = omp_get_wtime();

    for (int j = 0; j < REPEAT; j++) {
      result = dotp_reduction_optimized(x, y, arr_size);
    }

    run_time = omp_get_wtime() - start_time;
    pos += sprintf(pos, "Reduction Optimized: %d thread(s) took %f seconds\n",
                   i, run_time);

    // verify result is correct (within some threshold)
    if (fabs(serial_result - result) > 0.001) {
      pos += sprintf(pos, "Incorrect result!\n");
      *pos = '\0';
      return report_buf;
    }
  }

  // Only run this once because it's too slow..
  omp_set_num_threads(1);
  start_time = omp_get_wtime();
  for (int j = 0; j < REPEAT; j++)
    result = dotp_naive(x, y, arr_size);
  run_time = omp_get_wtime() - start_time;

  pos += sprintf(pos, "Naive: %d thread(s) took %f seconds\n", 1, run_time);
  *pos = '\0';
  return report_buf;
}
