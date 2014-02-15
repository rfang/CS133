CC=gcc

EXEDIR	= .

DWT3_EXEC = dwt3d

LIBRARY=-L${OPENCL_ROOT}/lib64 -lOpenCL
COMPILE=-I${OPENCL_ROOT}/include -O3 


all: exec 

exec: cdf97.* util.* headers
	$(CC) -o $(EXEDIR)/$(DWT3_EXEC) cdf97.c dwt3d_main.c util.c ocl_util.c $(LIBRARY) ${COMPILE}

headers:
	/usr/local/cs133/lab2/example2/cl_trans.sh program1.cl
 
clean:
	rm -rf *.o
	rm -rf $(EXEDIR)/$(DWT3_EXEC)
