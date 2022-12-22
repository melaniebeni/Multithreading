# Multithreading
POSIX pthreads multithreaded program that, when executed, has a main program thread, 18 worker child pthreads, and each of the 18 worker child pthreads has its own 3 worker computation child (that is, grandchild of the main) pthreads.    

Overall, 1 main program thread, 18 worker child pthreads, and 54 worker computation pthreads.

The input file has 18,000 numbers that the main program thread will open and read the input file into a 18,000 element integer array called InpArray. Each of the 18 worker child pthreads is responsible for processing its own 1,000 number segment of this integer array. (Data Parallelism)

The 18 worker child pthreads compute the arithmetic average, the sum, and root of the squares for its 1,000 number segment (of the InpArray) to the main program thread. Note that the worker child pthreads do not actually do the computations themselves. 
Instead, the 3 worker computation child  pthreads (grandchildren) will do the computations while the worker child pthread (the parent) just simply waits for the computations to be completed.

   When each worker child pthread has received its 3 results from the worker computation child pthreads, the worker child pthread (parent) makes these results available to the main program thread using global array(s) and then terminates.

After all of the 18 worker pthreads have terminated, the main program thread will compute and provide output for:
   a) The minimum of the 18 arithmetic average numbers
   b) The minimum of the 18 sum numbers
   c) The minimum of the 18 product over the sum numbers

./filename.exe INPUT_FILE OUTPUT_FILE

Uses the creat(), write(), and close() system calls for output and uses the POSIX pthread multithreaded library. 



   

