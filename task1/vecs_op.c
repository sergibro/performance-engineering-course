#define LEN 100000
#define NTIMES 10000

#include <stdio.h>
#include <time.h>
#include <immintrin.h>  // for SIMD AVX,AVX2

const float sec_const = 1000000.0;

float a[LEN] __attribute__((aligned(16)));
float b[LEN] __attribute__((aligned(16)));
float c[LEN] __attribute__((aligned(16)));
float d[LEN] __attribute__((aligned(16)));
float res[LEN] __attribute__((aligned(16)));

int nothing(float a[LEN], float res[LEN]){
	return (0);
}

int vectors_op0()
{
  clock_t start_t;
  clock_t end_t;
  clock_t clock_delta;
  double clock_delta_sec;
  
  start_t = clock();

  for (int n=0; n < NTIMES; n++){
    for (int i = 0; i < LEN; i++)
      res[i] = a[i] * b[i] + c[i] * d[i];
  	nothing(a, res);
  }

  end_t = clock();
  clock_delta = end_t - start_t;
  clock_delta_sec = (double) (clock_delta / sec_const);
  printf("No vect.\t %.2f \t\n", clock_delta_sec);

  return 0;
}

int vectors_op1()
{
  clock_t start_t;
  clock_t end_t;
  clock_t clock_delta;
  double clock_delta_sec;
  
  start_t = clock();

  __m256 rA, rB, rC, rD, rM1, rM2, rR;
  for (int n = 0; n < NTIMES; n++){
  	for (int i = 0; i < LEN; i += 8){  // from documentation - https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=5656,3996,2179,130,3327&techs=AVX,AVX2&text=_mm256_load_ps
  	  rA = _mm256_load_ps(&a[i]);
	    rB = _mm256_load_ps(&b[i]);
	    rC = _mm256_load_ps(&c[i]);
	    rD = _mm256_load_ps(&d[i]);
	    rM1 = _mm256_mul_ps(rA, rB);
	    rM2 = _mm256_mul_ps(rC, rD);
	    rR = _mm256_add_ps(rM1, rM2); 
	    _mm256_store_ps(&res[i], rR);
	}
	nothing(a, res);
  }

  end_t = clock();
  clock_delta = end_t - start_t;
  clock_delta_sec = (double) (clock_delta / sec_const);
  printf("With vect.\t %.2f \t\n", clock_delta_sec);

  return 0;
}

int main(){
  printf("Function \t Time(Sec) \n");
  vectors_op0();
  vectors_op1();
  return 0;
}
