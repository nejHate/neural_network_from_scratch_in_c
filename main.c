#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int data_len = 60000; // 60k
int input_vector = 28 * 28;
int output_vector = 10;
float learning_rate = 0.0000005;
int iterations = 1000; // 50
int64_t mul = 0;

void f_forward(float *inputs, float *weights, float *output) {
  float result = 0;
  for (int i = 0; i < output_vector; i++) {
    for (int j = 0; j < input_vector; j++) {
      result += inputs[j] * weights[j * output_vector + i];
      ++mul;
      //printf("#%d %d %d\n", 0, i, j);
      //printf("%f %f\n", inputs[j], weights[j * output_vector + i]);
    }
    output[i] += result;
    result = 0;
  }
  //exit(0);
}

void f_matrix_multiplication(float *matrix_a, float *matrix_b, float *matrix_c,
                             int size_a1, int size_a2, int size_b2) {
  // printf("%d %d %d\n", size_a1, size_a2, size_b2);
  for (int a1 = 0; a1 < size_a1; a1++) {     // 1  // data_len
    for (int b2 = 0; b2 < size_b2; b2++) {   // 10  // output_vector
      for (int a2 = 0; a2 < size_a2; a2++) { // 28 x 28  // input_vector
        matrix_c[b2] += matrix_a[a1 * size_a2 + a2] * matrix_b[a2 * size_b2 + b2];
        mul++;
        //printf("#%d %d %d\n", a1, b2, a2);
        //printf("%f %f\n", matrix_a[a1 * size_a2 + a2], matrix_b[a2 * size_b2 + b2]);
      }
    }
  }
  //exit(0);
}

int f_find_max(float *array){
  float max = array[0];
  int maxid = 0;
  for (int i = 0; i < output_vector; i++) {
    if(array[i] > max){
      max = array[i];
      maxid = i;
    }
  }
  return maxid;
}

void f_initialize_random_array(float weights[], int len) {
  for (int i = 0; i < len; i++) {
    weights[i] = (float)rand() / (float)RAND_MAX;
    // weights[i] = 2.0;  // DELETE
    // weights[i] = (float)i; // DELETE
  }
  return;
}

int main(void) {
  struct timespec res1, res2, res3, res4;
  clock_gettime(CLOCK_REALTIME, &res1);

  srand(time(NULL));
  srand(0);  // DELETE
  float *inputs = malloc(sizeof(float) * data_len * input_vector);
  float *outputs = malloc(sizeof(float) * data_len * output_vector);
  float *weights = malloc(sizeof(float) * output_vector * input_vector);
  
  //f_initialize_random_array(inputs, data_len * input_vector);
  f_initialize_random_array(weights, output_vector * input_vector);

  FILE *file = fopen("training_data_in.txt", "r");
  if (file == NULL) {
    printf("error in opening file\n");
  }
  char c;
  for (int i = 0; i < data_len * input_vector; i++) {
    c = fgetc(file);
    if (i % 1000 == 0) {
      // printf("%c\n", c);
    }
    if (i > (60000 * 28 * 28 - 10000)) {
      // printf("%d\n", i);
      // printf("%ud\n", (uint)c);  // UNDO
    }
    inputs[i] = (float)c/255.0;
  }
  fclose(file);

  file = fopen("training_data_out1.txt", "r");
  for(int i = 0; i < data_len; i++){
    c = fgetc(file);
    for(int j = 0; j < output_vector; j++){
      outputs[i * output_vector + j] = 0.0;
    }
    //printf("%d \n", i * output_vector + c);
    outputs[i * output_vector + c] = 1.0;
  }
  fclose(file);
  for (int i = 0; i < data_len; i++){
    for(int j = 0; j < 10; j++){
      //printf("%f ", outputs[i * 10 + j]);
    }
    //printf("\n");
  }
  //exit(0);

  float *forward = malloc(sizeof(float) * output_vector);
  float *error = malloc(sizeof(float) * output_vector);
  float total_error = 0.0;
  float minimum_error = 1.0;
  
  int total_right = 0;

  clock_gettime(CLOCK_REALTIME, &res2);

  for (int i = 0; i < iterations; i++) {
    for (int j = 0; j < data_len; j++) {
      //f_forward(&inputs[j * input_vector], weights, forward);
      f_matrix_multiplication(&inputs[j * input_vector], weights, forward, 1, 28*28, 10);
      if(j == 2){
        for (int k = 0; k < 10; k++){
          printf("%f ", forward[k]);
        }
        printf("\n");
        for (int k = 0; k < 10; k++){
          printf("%f ", outputs[j * 10 + k]);
        }
        printf("\n\n");
      }
      for (int k = 0; k < output_vector; k++) {
        error[k] = forward[k] - outputs[j * output_vector + k];
        total_error += error[k];
      }
      for (int k = 0; k < output_vector; k++) {
        for (int l = 0; l < input_vector; l++) {
          weights[l * output_vector + k] -= learning_rate * error[k] * inputs[j * input_vector + l];
          ++mul;
          ++mul;
        }
      }
      //for (int k = 0; k < data_len; k++) {
        //break;
        /*
        well this nn is training to do something else 
        so this don't work
        */
      //printf("%d %d\n", f_find_max(forward), f_find_max(&outputs[j]));
      if (f_find_max(forward) == f_find_max(&outputs[j])){
        //printf("%d %d\n", f_find_max(forward), f_find_max(&outputs[i]));
        /*
        for(int lol = 0; lol < 10; lol++){
          printf("%.4f ", forward[lol]);
        }
        printf("\n");
        for(int lol = 0; lol < 10; lol++){
          printf("%.4f ", outputs[lol]);
        }
        printf("\n");
        //printf("%d ", total_right);
        */
        total_right++;
      }
      for (int k = 0; k < output_vector; k++) {
        error[k] = 0;
        forward[k] = 0;
      }
    }
    
    total_error = total_error / data_len;
    printf("%f  ", total_error);
    printf("%d\n", total_right);
    total_right = 0;
    if (fabs(total_error) <= 0.005) {
      // learning_rate = fabs(total_error) * 2;  // UNDO
    }
    if (fabs(total_error) < minimum_error) {
      minimum_error = fabs(total_error);
    }
    total_error = 0.0;
  }

  clock_gettime(CLOCK_REALTIME, &res3);

  for (int i = 0; i < output_vector; i++) {
    break;
    for (int j = 0; j < input_vector; j++) {
      printf("# %.4f , %d ", weights[j * output_vector + i],
             j * output_vector + i);
    }
    printf("\n");
  }

  printf("%.12f\n", minimum_error);
  int exponent = 0;
  while (minimum_error < 1.0) {
    if (exponent == 15) {
      printf("Error: %.12f\n", minimum_error);
      break;
    }
    minimum_error *= 10.0;
    ++exponent;
  }

  long ms_runtime = (res3.tv_sec * 1000000000 + res3.tv_nsec) -
                    (res2.tv_sec * 1000000000 + res2.tv_nsec);

  printf("final total error: %fe-%d\n", minimum_error, exponent);
  printf("total multiplication: %ld\n", mul);
  printf("ms to calculate %ld\n", ms_runtime);
  printf("mul per sec %lf\n",
         (double)mul / (double)((double)ms_runtime / (double)1000000000));
  // printf("%.15f\n", minimum_error);
  free(forward);
  free(error);

  free(inputs);
  free(outputs);
  free(weights);

  clock_gettime(CLOCK_REALTIME, &res4);
  //printf("time: %ld.%ld\n", res1.tv_sec, res1.tv_nsec);
  //printf("time: %ld.%ld\n", res2.tv_sec, res2.tv_nsec);
  //printf("time: %ld.%ld\n", res3.tv_sec, res3.tv_nsec);
  //printf("time: %ld.%ld\n", res4.tv_sec, res4.tv_nsec);

  printf("time to train: %ld.%ld s\n",
         -((res3.tv_nsec - res2.tv_nsec) < 0) + (res3.tv_sec - res2.tv_sec),
         labs(res3.tv_nsec - res2.tv_nsec));
  return 0;
}

/*
if(i == 0){
  printf(" ");
}


gcc ./main.c -o main -g -Werror -Wextra -Wno-everything -pthread -lm
-Wformat-overflow=2 -fsanitize=address -fno-omit-frame-pointer




  FILE *file = fopen("training_data_in.txt", "r");

  unsigned char c;
  for(int i = 0; i < data_len * input_vector; i++){


    c = fgetc(file);
    inputs[i] = (float)c / (float)255;
    printf("%f ", inputs[i]);
    ////printf("%.3f ", inputs[i]);
  }
  fclose(file);


==47068==
==47068== I   refs:      49,868,989,722
==47068== I1  misses:             3,190
==47068== LLi misses:             1,679
==47068== I1  miss rate:           0.00%
==47068== LLi miss rate:           0.00%
==47068==
==47068== D   refs:      29,946,781,670  (23,949,747,476 rd   + 5,997,034,194
wr)
==47068== D1  misses:     4,675,418,398  ( 4,664,053,611 rd   +    11,364,787
wr)
==47068== LLd misses:           266,748  (        17,514 rd   +       249,234
wr)
==47068== D1  miss rate:           15.6% (          19.5%     +           0.2% )
==47068== LLd miss rate:            0.0% (           0.0%     +           0.0% )
==47068==
==47068== LL refs:        4,675,421,588  ( 4,664,056,801 rd   +    11,364,787
wr)
==47068== LL misses:            268,427  (        19,193 rd   +       249,234
wr)
==47068== LL miss rate:             0.0% (           0.0%     +           0.0% )
==47068==
==47068== Branches:       1,582,505,796  ( 1,574,627,560 cond +     7,878,236
ind)
==47068== Mispredicts:       25,879,685  (    25,878,483 cond +         1,202
ind)
==47068== Mispred rate:             1.6% (           1.6%     +           0.0% )


  void f_forward(float *inputs, float *weights, float *output) {
    float result = 0;
    for (int i = 0; i < output_vector; i++) {
      for (int j = 0; j < input_vector; j++) {
        result += inputs[j] * weights[j * output_vector + i];
        ++mul;
      }
      output[i] = result;
      result = 0;
    }
  }

  void f_matrix_multiplication(float *matrix_a, float *matrix_b, float
*matrix_c, int size_a1, int size_a2, int size_b2){
    //printf("%d %d %d\n", size_a1, size_a2, size_b2);
    for(int a1 = 0; a1 < size_a1; a1++){				// 1  //
data_len for(int b2 = 0; b2 < size_b2; b2++){			// 10  //
output_vector for(int a2 = 0; a2 < size_a2; a2++){		// 28 x 28  //
input_vector matrix_c[a1 * size_b2 + b2] += matrix_a[a1 * size_a2 + a2] *
matrix_b[a2 * size_b2 + b2]; mul++;
        }
      }
    }
  }

gcc ./main.c -o main -O3 -fwhole-program -funroll-all-loops -mavx2


*/