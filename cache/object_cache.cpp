#include <stdlib.h>
#include <stdio.h>
#include <sys/times.h>
#include <time.h>
#include <xmmintrin.h>
#include <vector>

const float sec_const = 1000000.0;

int nothing(int sum, int a){
	return(0);
}

class Sample_object {
	public:
	   bool flag = false;
	   u_int8_t dump[1024];
};

template <size_t P> class SC {
	public:
	   int flag = 1;
	   char dump[P];
};


int read_flag1(size_t len){
  clock_t start_t;
  clock_t end_t;
  clock_t clock_delta;
  double clock_delta_sec;
  
  volatile int f;
  //bool f;
	
  std::vector<SC<1024*1024>> testVector;
  size_t vector_len = len;
  
  for ( size_t i = 0; i < vector_len; i++ ){
    class SC<1024*1024> a;
    a.flag = rand();
    for (int i=0;i<1024*1024;i++){
        a.dump[i] = 1;
    }
    testVector.push_back(a);
  }
  
  //printf("%d %u\n", testVector[0].flag, testVector[0].dump[0] );  
                
  start_t = clock();
  
  for (std::vector<SC<1024*1024>>::iterator it = testVector.begin() ; it != testVector.end(); ++it)
    f = it->flag;
    //std::cout << ' ' << *it;
    
  end_t = clock();
  clock_delta = end_t - start_t;
  clock_delta_sec = (double) (clock_delta / sec_const);
  printf("1; %.6f\n", clock_delta_sec);
  
  //DATA 
  
  
  return(0);
}


int main(){
  read_flag1(1024);
  return 0;
}

