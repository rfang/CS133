#include <iostream> 
#include <cstdlib> 
#include <omp.h> 
#include <cmath>
#include <iomanip>
using namespace std; 

struct Point { 
	double x;
	double y;
	
	Point() {
		x=0;
		y=0;
	}
};

int main(int argc, char* argv[]) {
	if(argc < 2) {
		cout << "whoops check your arguments " << endl;
		exit(1);
	}
	int N = atoi(argv[1]);

	Point* fifo = new Point[32];
	Point* ploc = fifo;
	Point* cloc = fifo;		
	int num_in = 0;
	int nProd = 0;
	int nCons = 0; 
	Point* ctemp;
	Point* ptemp;

	srand(time(NULL));
	#pragma omp parallel sections shared(nProd, nCons, fifo, num_in)  
	{
		#pragma omp section 
		while(nProd < N) {
			if ( nProd-nCons < 32 ) {
				ptemp = ploc; 
				if (ploc == fifo+32) 
					ploc = fifo; 
				else 
					ploc = ploc + 1;
				ptemp->x = (double)rand()/RAND_MAX;
				ptemp->y = (double)rand()/RAND_MAX;
				//cout << ptemp->y << " " << ptemp->x << endl;
				nProd ++;
			}			
		} 	
		
		#pragma omp section
		while(nCons < N) {
			if (nProd-nCons > 0) {
				ctemp = cloc;
				if(cloc == fifo+32) 
					cloc = fifo;
				else
					cloc = cloc+1; 
				double x = ctemp->x-0.5;
				double y = ctemp->y-0.5;
				double r = sqrt(x*x + y*y);
				if (r<=0.5) num_in++;
				nCons ++; 
			}
		}
	}
	double pi = (4.0*num_in)/N;
	//cout << "Num_In" << num_in << endl;
	
	cout << setprecision(7) << "Pi: " << pi << endl;

}
