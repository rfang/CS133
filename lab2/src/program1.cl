
#define a1 (-1.586134342)
#define a2 (-0.05298011854)
#define a3 (0.8829110762)
#define a4 (0.4435068522)
#define k1 1.1496043988602418
#define k2 (1/1.1496043988602418)
#define x(i, j)	img_start[(i)*m+(j)]

__kernel 
void pone ( 
		int 	n,
		int 	m,
		__global float* img,
		__global float* tmp,
		int		p 
)
{
	int i;
	float* img_start;
	float* tmp_start;
	
	i = get_group_id(0);
	img_start = img + p*m*n; 
	tmp_start = tmp + p*m*n;
/*
    	int j;
	for (j=1; j<m-2; j+=2) {
	x(i, j) += a1*(x(i, j-1) + x(i, j+1));
	} 
	x(i, m-1) += 2*a1*x(i, m-2);
	// Update 1
	for (j=2; j<m; j+=2) {
		x(i, j) += a2*(x(i, j-1) + x(i, j+1));
	}
	x(i, 0) += 2*a2*x(i, 1);
	// Predict 2
	for (j=1; j<m-2; j+=2) {
		x(i, j) += a3*(x(i, j-1) + x(i, j+1));
	}
	x(i, m-1) += 2*a3*x(i, m-2);
	// Update 2
	
	for (j=2; j<m; j+=2) {
		x(i, j) += a4*(x(i, j-1) + x(i, j+1));
	}
	x(i, 0) += 2*a4*x(i, 1);
*/
}				
