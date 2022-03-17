#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
// #define VECTOR_SIZE 65536

// OpenCL kernel which is run for every work item created.
const char* saxpy_kernel = "__kernel                                  "
                           "void saxpy_kernel(float alpha,            "
                           "                  __global float *A,      "
                           "                  __global float *B,      "
                           "                  __global float *C)      "
                           "{                                         "
                           "    //Get the index of the work-item      \n"
                           "    int index = get_global_id(0);         "
                           "    C[index] = alpha* A[index] + B[index];"
                           "}                                         ";

int main(int argc,char** argv)
{
    if(argc!=3) {printf("Need 2 args\n");return 1;}
    int i;
    // argv 1, argv 2
    unsigned int VECTOR_SIZE = atoi(argv[1]);
    size_t local_size=atoi(argv[2]);
    // Allocate space for vectors A, B and C
    float alpha = 2.0;
    float *A = (float*)calloc(VECTOR_SIZE,sizeof(float)),*B = (float*)calloc(VECTOR_SIZE,sizeof(float)),*C = (float*)calloc(VECTOR_SIZE,sizeof(float));
    for (i = 0; i < VECTOR_SIZE; i++) {
        A[i] = i;
        B[i] = VECTOR_SIZE - i;
    }

    // Get platform and device information
    cl_platform_id* platforms = NULL;
    cl_uint num_platforms;
    // Set up the Platform
    cl_int clstat=0;
    // Get the devices list and choose the device you want to run on
    cl_device_id* device_list = NULL;
    cl_uint num_devices;

    clstat = clGetPlatformIDs(0,NULL,&num_platforms);
    platforms = (cl_platform_id*)malloc(num_platforms*sizeof(cl_platform_id));
    clstat = clGetPlatformIDs(1,platforms,NULL);

    clstat = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices);
    device_list = (cl_device_id*)
        calloc(num_devices,sizeof(cl_device_id));
    clstat = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, num_devices, device_list, NULL);

    // Create one OpenCL context for each device in the platform
    cl_context context = clCreateContext(NULL, num_devices, device_list, NULL, NULL, &clstat);

    // Create a command queue

    cl_command_queue cmd_queue = clCreateCommandQueueWithProperties(context, device_list[0], NULL, &clstat);

    // Create memory buffers on the device for each vector
    cl_mem A_clmem = clCreateBuffer(context, CL_MEM_READ_ONLY, VECTOR_SIZE * sizeof(float), NULL, &clstat);
    cl_mem B_clmem = clCreateBuffer(context, CL_MEM_READ_ONLY, VECTOR_SIZE * sizeof(float), NULL, &clstat);
    cl_mem C_clmem = clCreateBuffer(context, CL_MEM_WRITE_ONLY, VECTOR_SIZE * sizeof(float),NULL,&clstat);

    // Copy the Buffer A and B to the device
    clstat = clEnqueueWriteBuffer(cmd_queue, A_clmem, CL_TRUE, 0, VECTOR_SIZE * sizeof(float), A, 0, NULL, NULL);
    clstat = clEnqueueWriteBuffer(cmd_queue, B_clmem, CL_TRUE, 0, VECTOR_SIZE * sizeof(float), B, 0, NULL, NULL);
    
    // Create a program from the kernel source
    cl_program prg = clCreateProgramWithSource(context, 1, (const char**)&saxpy_kernel, NULL, &clstat);

    // Build the program
    clstat = clBuildProgram(prg, 1, device_list, NULL, NULL,NULL);

    // Create the OpenCL kernel
    cl_kernel kernel = clCreateKernel(prg, "saxpy_kernel", &clstat);

    // Set the arguments of the kernel
    clstat = clSetKernelArg(kernel, 0, sizeof(float), (void*)&alpha);
    clstat = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&A_clmem);
    clstat = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&B_clmem);
    clstat = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void*)&C_clmem);

    // Execute the OpenCL kernel on the list
    size_t global_size = VECTOR_SIZE; // Process the entire lists
    // size_t local_size = 64; // Process one item at a time
    // cl_event event; // event listener
    clstat = clEnqueueNDRangeKernel(cmd_queue, kernel, 1, 0, &global_size, &local_size, 0, NULL, NULL);

    // Read the cl memory C_clmem on device to the host variable C
    clstat = clEnqueueReadBuffer(cmd_queue, C_clmem, CL_TRUE, 0, VECTOR_SIZE * sizeof(float), C, 0, NULL, NULL);

    // Clean up and wait for all the comands to complete.
    clstat = clFlush(cmd_queue);
    clstat = clFinish(cmd_queue);

    

    // Display the result to the screen
    for (i = 0; i < VECTOR_SIZE; i++)
        printf("%f * %f + %f = %f,\n", alpha, A[i], B[i], C[i]);


    // Finally release all OpenCL allocated objects and host buffers.
    clstat = clReleaseKernel(kernel);
    clstat = clReleaseProgram(prg);
    clstat = clReleaseMemObject(A_clmem);
    clstat = clReleaseMemObject(B_clmem);
    clstat = clReleaseMemObject(C_clmem);
    clstat = clReleaseCommandQueue(cmd_queue);
    clstat = clReleaseContext(context);
    free(A);
    free(B);
    free(C);
    free(platforms);
    free(device_list);
    return 0;
}