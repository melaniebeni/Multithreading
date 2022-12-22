#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <math.h>

// global arrays
int *InpArray;
int arithAvg[18000];
int sum[18000];
int rOfSum[18000];

void *ArithAvg(void *arg) {   // calculates the arithmetic average of each segment
	int segment = *((int*)arg);
	int sum = 0;
    int *ptr = arithAvg;
	for(int i=segment*1000; i<(segment+1)*1000; i++) {  // finds the sum of the segment
		 sum += InpArray[i];
	 }
	ptr[segment] = sum/1000;  // stores the arithmetic average in pointer to the global array
	pthread_exit(NULL);
}

void *Sum(void *arg) { // calculates the sum of each segment
    int segment = *((int*)arg);
	int sum1 = 0;
    int *ptr = sum;   // pointer to the sum global array
	for(int i=segment*1000; i<(segment+1)*1000; i++) {  // find the sum of each segment
		sum1 += InpArray[i];
	}
	ptr[segment] = sum1;  // store the sum in the pointer to the global array
	pthread_exit(NULL);
}

void *rootOfSum(void *arg) {  // calculates the root of the sum of the squares
	int segment = *((int *)arg);
    long sum = 0;
    int *ptr = rOfSum;
	for(int i=segment*1000; i<(segment+1)*1000; i++) {
		sum += (InpArray[i]*InpArray[i]);   // finds the sum of the square of each number in the segment
	}
    ptr[segment] = sqrt(sum);  // finds the square root and stores it in the pointer to the global array
	pthread_exit(NULL);
}

void *runner(void *param) {     // creates and joins the worker computation threads
    pthread_attr_t attr;
    pthread_t compGeomAvg[18];
	pthread_t compArithAvg[18];
	pthread_t compSum[18];
    pthread_attr_init(&attr);
    int seg = *((int*)param);  // get the segment number
    // creates pthreads
	pthread_create(&compGeomAvg[seg], &attr, Sum, param);
	pthread_create(&compArithAvg[seg], &attr, ArithAvg, param);
	pthread_create(&compSum[seg], &attr, rootOfSum, param);
   // joins pthreads
	for(int i=0; i<18; i++) {
		pthread_join(compGeomAvg[i], NULL);
		pthread_join(compArithAvg[i], NULL);
		pthread_join(compSum[i], NULL);
	}
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_attr_t attr;
    pthread_attr_init(&attr);
	pthread_t workers[18];
    InpArray = calloc (sizeof (int), 18000);  // allocate memory for the global array
    FILE *write;
    write = fopen(argv[2],"w");     // create output file
	FILE *read;
    read  = fopen(argv[1], "r" );   // opens input file
        for (int i = 0; i < 18000; i++){
            fscanf(read,"%d\n", &InpArray[i]);      // reads file into struct array
        }
    int rtn;
	// create the worker threads
	for(int i=0; i<18; i++) {
        int *arg  = malloc(sizeof(*arg));    // allocates memory space for thread numbers
		*arg = i;
		rtn = pthread_create(&workers[i], &attr, runner, arg);
        if (rtn) {      // if error exit
        printf( "Error:unable to create thread," );
        exit(-1);
     }
	}
	for(int i=0; i<18; i++) {
		pthread_join(workers[i], NULL);     // joins the worker threads
	}
    int min = 0, min2 = 0, min3 = 0;
	for(int i=0; i<18; i++) {       // Print the results
		fprintf(write, "Worker Child Pthread Number = %d : \t Arithmetic Average = %d \t Sum = %d \t Root of the Sum of Squares = %d \n", i, arithAvg[i], sum[i], rOfSum[i]);
         // finds the minimums 
         if (arithAvg[i] < arithAvg[min])
         min = i;
         if (sum[i] < sum[min2])
         min2 = i;
         if (rOfSum[i] < rOfSum[min3])
         min3 = i;
	}
    fprintf(write, "Main program thread: Min of the Arithmetic Averages = %d \nMain program thread: Min of the Sums = %d \nMain program thread: Min of the root of the sum of the squares = %d \nMain program thread: Terminating. \n", arithAvg[min], sum[min2], rOfSum[min3]);
    return 0;
}