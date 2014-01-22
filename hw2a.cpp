#include <iostream> 
using namespace std; 

int is_prime (int p) {
    int i; 

    if(p<2) return 0;
    i=2; 

    while(i*i<=p) {
      if (p%i==0) return 0; 
      i++; 
    }
    return 1;
}

int main(int argc, char*argv[]) {
  int N = atoi(argv[1]);
  int T = atoi(argv[2]);

  omp_set_num_threads(T);
  boolean results[] = new boolean[N];

  #pragma omp for 
  for(int i=2; i<N; i++) {
    results[i] = is_prime(i);
  }

  if(N>=2) {
    cout << "2";
  }

  for(int i=3; i<N; i++) {
    if( results[i] ) cout << ", " << i;
  }
  cout << endl;

}