#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include <sys/time.h>

#include <CL/cl.h>
#include "cdf97.h"
#include "util.h"
#include "ocl_util.h"

int main(int argc, char *argv[]) {

    struct	timeval t1, t2, tr;
    double	elapsedTime = 0;

	if(argc<2) {
		printf("./dwt3d input.dat [level]");
		return -1;
	}

	char* fname = argv[1];
	char fname_out[50] = "dwt3d_out.dat";

	int level = 1;

	if (argc>2)
		level = atoi(argv[2]);

	int m, n, p;
	int i;
	float *img;
	float *tmp;

	// read the 3D image binary
	FILE* fin = fopen(fname, "rb");
	fread(&m, 1, sizeof(int), fin);
	fread(&n, 1, sizeof(int), fin);
	fread(&p, 1, sizeof(int), fin);

	img = (float*)calloc(n*m*p, sizeof(float));
	tmp = (float*)calloc(n*m*p, sizeof(float));

	fread (img, n*m*p, sizeof(float), fin);
	fclose(fin);
/* BEGINNING OF TIMED STUFF */
	//OPENCL 
	// Use this to check the output of each API call
	cl_int status;  
	 
	// Retrieve the number of platforms
	cl_uint numPlatforms = 0;
	status = clGetPlatformIDs(0, NULL, &numPlatforms);

	// Allocate enough space for each platform
	cl_platform_id *platforms = NULL;
	platforms = (cl_platform_id*)malloc(
	    numPlatforms*sizeof(cl_platform_id));
     
	// Fill in the platforms
	status = clGetPlatformIDs(numPlatforms, platforms, NULL);

	    cl_int platform_index = -1;
	    char cBuffer[1024];
	    for (i=0; i<numPlatforms; i++) {
		    clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, sizeof(cBuffer), cBuffer, NULL);
		    if (strstr(cBuffer, "Intel") != NULL) {
			    platform_index = i;
			    break;
		    }
	    }
	    if (platform_index < 0) {
		    printf("Cannot find platforms support OpenCL.\n");
		    return -1;
	    }
	    else {
		    printf("Selected platform '%s'. %d\n", cBuffer, platform_index);
	    }

	// Retrieve the number of devices
	cl_uint numDevices = 0;
	status = clGetDeviceIDs(platforms[platform_index], CL_DEVICE_TYPE_CPU, 0, 
	    NULL, &numDevices);

	// Allocate enough space for each device
	cl_device_id *devices;
	devices = (cl_device_id*)malloc(
	    numDevices*sizeof(cl_device_id));

	// Fill in the devices 
	status = clGetDeviceIDs(platforms[platform_index], CL_DEVICE_TYPE_ALL,        
	    numDevices, devices, NULL);

	// Create a context and associate it with the devices
	cl_context context;
	context = clCreateContext(NULL, numDevices, devices, NULL, 
	    NULL, &status);

	// Create a command queue and associate it with the device 
	cl_command_queue cmdQueue;
	cmdQueue = clCreateCommandQueue(context, devices[0], 0, 
	    &status);

	cl_program program1, program2, program3, program4;

	status = utilProgramFromFile("program1.cl", context, numDevices, devices, &program1);

	if (status != CL_SUCCESS) {
		printf("Error in reading program from file, code %d\n", status);
		return -1;
	}
	status = utilProgramToBinary("program1.bin", &program1);
/*
	status = utilProgramFromFile("program2.cl", context, numDevices, devices, &program2);

	if (status != CL_SUCCESS) {
		printf("Error in reading program from file, code %d\n", status);
		return -1;
	}
	status = utilProgramToBinary("program2.bin", &program2);

	status = utilProgramFromFile("program3.cl", context, numDevices, devices, &program3);

	if (status != CL_SUCCESS) {
		printf("Error in reading program from file, code %d\n", status);
		return -1;
	}
	status = utilProgramToBinary("program3.bin", &program3);

	status = utilProgramFromFile("program4.cl", context, numDevices, devices, &program4);

	if (status != CL_SUCCESS) {
		printf("Error in reading program from file, code %d\n", status);
		return -1;
	}
	status = utilProgramToBinary("program4.bin", &program4);
*/


	cl_kernel kernel1, kernel2, kernel3, kernel4;
	kernel1 = clCreateKernel(program1, "pone", &status);
	//kernel2 = clCreateKernel(program2, "ptwo", &status);
	//kernel3 = clCreateKernel(program3, "pthree", &status);
	//kernel4 = clCreateKernel(program4, "pfour", &status);





	    // Create OpenCL buffers
	cl_mem bufImg;
	cl_mem bufTmp;

	bufImg = clCreateBuffer(context, CL_MEM_READ_WRITE, n*m*p*sizeof(float), NULL, &status);
	bufTmp = clCreateBuffer(context, CL_MEM_READ_WRITE, n*m*p*sizeof(float), NULL, &status);

	// Associate the input and output buffers with the kernel 
	status = clSetKernelArg(kernel1, 0, sizeof(int), &n);
	status = clSetKernelArg(kernel1, 1, sizeof(int), &m);
	status = clSetKernelArg(kernel1, 2, sizeof(cl_mem), &bufImg);
	status = clSetKernelArg(kernel1, 3, sizeof(cl_mem), &bufTmp);	 
	/*
	status = clSetKernelArg(kernel2, 0, sizeof(int), &n);
	status = clSetKernelArg(kernel2, 1, sizeof(int), &m);
	status = clSetKernelArg(kernel2, 2, sizeof(cl_mem), &bufImg);
	status = clSetKernelArg(kernel2, 3, sizeof(cl_mem), &bufTmp);	 
	status = clSetKernelArg(kernel3, 0, sizeof(int), &n);
	status = clSetKernelArg(kernel3, 1, sizeof(int), &m);
	status = clSetKernelArg(kernel3, 2, sizeof(cl_mem), &bufImg);
	status = clSetKernelArg(kernel3, 3, sizeof(cl_mem), &bufTmp);	 
	status = clSetKernelArg(kernel4, 0, sizeof(int), &n);
	status = clSetKernelArg(kernel4, 1, sizeof(int), &m);
	status = clSetKernelArg(kernel4, 2, sizeof(cl_mem), &bufImg);
	status = clSetKernelArg(kernel4, 3, sizeof(cl_mem), &bufTmp);	 
*/



	status = clEnqueueWriteBuffer(cmdQueue, bufImg, CL_FALSE, 0, n*m*p*sizeof(float), img, 0, NULL, NULL);

	    size_t work_size[1] = {0};
	    size_t group_size[1] = {256};

		work_size[0] = n;				    


    gettimeofday(&t1, NULL);

	// 3D DWT transform, which is 2D DWT for each image slices
	for (i=0; i<p; i++) {
		float* x = img+i*n*m;
		float* tempbank = tmp+i*n*m;

		int nlevel = (level < 0 ? -level : level);

		int N = n;
		int M = m;

		int plevel = 1<<nlevel-1;
		int i_level;
		// this means image is not big enough for transformation 
		// which we assume will not happen
		if (N/plevel < 2 || M/plevel < 2) 
			return;

		if (level > 0) {	
			// forward DWT (1. col; 2. row)
			for (i_level=0; i_level<nlevel; i_level++) {
				status = clEnqueueWriteBuffer(cmdQueue, bufImg, CL_FALSE, 0, n*m*p*sizeof(float), img, 0, NULL, NULL);
				// Stage 1
			    //fwt97_pd(x, m, n, M, N);
			    status = clSetKernelArg(kernel1, 4, sizeof(int), &i);
				status = clEnqueueNDRangeKernel(cmdQueue, kernel1, 1, NULL, work_size, NULL, 0, NULL, NULL);
				clEnqueueReadBuffer(cmdQueue, bufImg, CL_TRUE, 0, n*m*p*sizeof(float), img, 0, NULL, NULL);
				// Stage 2
				fwt97_dl_foo(x, tempbank, m, n, M, N);
			    //status = clSetKernelArg(kernel2, 4, sizeof(int), &i);
				//status = clEnqueueNDRangeKernel(cmdQueue, kernel2, 1, NULL, work_size, NULL, 0, NULL, NULL);
				// Stage 3
				fwt97_pd(tempbank, n, m, n, m);
			    //status = clSetKernelArg(kernel3, 4, sizeof(int), &i);
				//status = clEnqueueNDRangeKernel(cmdQueue, kernel3, 1, NULL, work_size, NULL, 0, NULL, NULL);
				
				// Stage 4
				fwt97_dl_bar(x, tempbank, n, m, N, M);
			    //status = clSetKernelArg(kernel4, 4, sizeof(int), &i);
				//status = clEnqueueNDRangeKernel(cmdQueue, kernel4, 1, NULL, work_size, NULL, 0, NULL, NULL);
				// work on the upper left image (low pass filtered) in the next level
				n /= 2;
				m /= 2;
			}
		}
		else {				
			// backward DWT, omitted for the project
			;	
		}
	}
	//get info back out		
//	clEnqueueReadBuffer(cmdQueue, bufImg, CL_TRUE, 0, n*m*p*sizeof(float), img, 0, NULL, NULL);

    gettimeofday(&t2, NULL);
    timersub(&t1, &t2, &tr);
    printf("Execute time: %.2f sec\n", fabs(tr.tv_sec+(double)tr.tv_usec/1000000.0));

	FILE* fout = fopen(fname_out, "wb+");
	fwrite(&m,	 sizeof(int), 1, fout);
	fwrite(&n, 	 sizeof(int), 1, fout);
	fwrite(&p, 	 sizeof(int), 1, fout);
	fwrite(img,  sizeof(float), n*m*p, fout);
	fclose(fout);

	free(img);
	free(tmp);

	return 0;
}
