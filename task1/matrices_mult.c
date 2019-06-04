#define DIM 1024
#define NTIMES 10000

#include <stdio.h>
#include <time.h>
#include <immintrin.h>  // for SIMD AVX,AVX2

const float sec_const = 1000000.0;

float m1[DIM][DIM] __attribute__((aligned(16)));
float m2[DIM][DIM] __attribute__((aligned(16)));
float res[DIM][DIM] __attribute__((aligned(16)));

int nothing(float m1[DIM][DIM], float res[DIM][DIM]){
  return (0);
}

int matrices_mult0()
{
  clock_t start_t;
  clock_t end_t;
  clock_t clock_delta;
  double clock_delta_sec;
  
  start_t = clock();

  for(int i = 0; i < DIM; i++){
    for(int j = 0; j < DIM; j++)
      for(int k = 0; k < DIM; k++)
        res[i][j] += m1[i][k] * m2[k][j];
    nothing(m1, res);
  }

  end_t = clock();
  clock_delta = end_t - start_t;
  clock_delta_sec = (double) (clock_delta / sec_const);
  printf("No vect.\t %.2f \t\n", clock_delta_sec);

  return 0;
}

int matrices_mult1()
{
  clock_t start_t;
  clock_t end_t;
  clock_t clock_delta;
  double clock_delta_sec;
  
  start_t = clock();

  float m2t[DIM][DIM] __attribute__((aligned(16)));
  for (int i = 0; i < DIM; i++)
    for (int j = 0; i < DIM; i++)
      m2t[i][j] = m2[j][i];

  __m256 rM1, rM2, rS, rR;

  for (int i = 0; i < DIM; i++){
    for (int j = 0; j < DIM; j++){
      rS = _mm256_setzero_ps();
      for (int k = 0; k < DIM; k += 8){
        rM1 = _mm256_load_ps(&m1[i][k]);
        rM2 = _mm256_load_ps(&m2t[j][k]);
        rR = _mm256_dp_ps(rM1, rM2);
        rS = _mm256_add_pd(rS, rR);
      }
      _mm256_store_ps(rR, rS);
    }
  }

  end_t = clock();
  clock_delta = end_t - start_t;
  clock_delta_sec = (double) (clock_delta / sec_const);
  printf("With vect.\t %.2f \t\n", clock_delta_sec);

  return 0;
}

int main(){
  printf("Function \t Time(Sec) \n");
  matrices_mult0();
  matrices_mult1();
  return 0;
}
