/*
 * Authour	:	Ben Haubrich
 * File		:	square.c
 * Synopsis	:	prints data about a multithreaded operation that sqares
 *			N integers
 */

/*
 * Standard Headers
 */
#include <windows.h> /* For CreateThread(), Sleep() */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_THREADS 6

/*
 * square is a recursive algorithm that calculates the sum of n squares. It's
 * purpose is to create a lot of stack space and take a lot of time.
 * @param n The number up to which the consecutive integers will be squared
 * @return The sum of the squares
 * @precond n must be positive
 * @precond keepRunning must be true
 * 
 * Furthur Notes:
 * Since this function will be run using multiple threads. it must be defined as
 * a CALLBACK function
 */
int CALLBACK square(int n) {

	/*
	* Count the number of calls. Start at 1 for the initial call
	*/
	int numCalls = 1;

	if(0 == n) {

		return 0;
	}
	else if(n < 0) {

		fprintf(stderr, "n can not be negative\n");
		exit(EXIT_FAILURE);
	}
	else {
	
		numCalls++;
		return(square(n-1) + n + n - 1);
	}
}

int main(int argc, char *argv[]) {


	/*
	* Remember, argv[0] displays the name of the program. argv[1] is the
	* first argument
	*/

	/*
	* The number of threads that will execute square. DWORD's are defined
	* in windows.h as 32 bit unsigned integers
	*/
	DWORD numThreads = atoi(argv[1]);

	if(numThreads > MAX_THREADS) {

		fprintf(stderr, "Number of threads chosen is larger than the than the maximum allowable\n");
		
		exit(EXIT_FAILURE);
	}
	/*
	* Deadline in seconds for the threads to complete
	*/
	DWORD allowedTime = atoi(argv[2]);
	/*
	* The sqaures function will calculate squares until this number
	*/
	DWORD sumSquaresTo = atoi(argv[3]);

	/*
	* Make an array to hold the threads
	*/

	/*
	* Temporary fix to get this program to compile. Won't work with argv
	* value
	*/
	HANDLE threadArray[MAX_THREADS];
 
	/*
	* time variables to keep track of elapsed time of threads
	*/
	SYSTEMTIME *time = NULL;
	WORD threadTimes[MAX_THREADS];

	/*
	* create the threads
	* Extra delimeters are to scope i, so it's only declared for this loop
	*/
	{
	DWORD i;
	for(i = 0; i < numThreads; i++) {
	
		/*
		* Notes about CreateThread()
		* You can use a struct if you need to pass more than one
		* parameter to the threaded function
		* &square and square are equivalent. They both give the
		* location in the code of the square function.
		*/
		
		threadArray[i] = CreateThread(NULL,
						0,
						(LPTHREAD_START_ROUTINE) square,
						&sumSquaresTo,
						CREATE_SUSPENDED,
						NULL);
	
		if(NULL == threadArray[i]) {

			fprintf(stderr, "CreateThread() failed, and could not create the %d'th thread", i);
		}
	}

	for(i = 0; i < numThreads; i++) {

		/*
		* Resume the threads and save the time they started
		*/
		ResumeThread(threadArray[i]);
		GetSystemTime(time);
		threadTimes[i] = time->wSecond;
	}
	}

	/*
	* Give the threads until the deadline to finish execution
	*/
	printf("wolf fence 1; numThreads = %u, allowedTime = %u\n", numThreads, allowedTime);
	DWORD waitReturn;
	waitReturn = WaitForMultipleObjects(numThreads, threadArray, true, (allowedTime * 1000));

	switch (waitReturn) {

		case (WAIT_TIMEOUT):
		printf("wait timeout\n");
		break;

		case (WAIT_FAILED):
		printf("wait failed\n");
		break;

		default:
		printf("wait error: %d\n", GetLastError());
		exit(EXIT_FAILURE);
	}

	printf("wolf fence 2\n");

	/*
	* Get the time, and subtract from start time to find total execution
	*/
	GetSystemTime(time);
	
	/*
	* Print execution times for each thread and the number of calls to
	* square that each thread made
	*/
	printf("Time for thread 1: %u\n Time for thread 2: %u\n Time for thread 3 %u\n", time->wSecond - threadTimes[0], time->wSecond - threadTimes[1], time->wSecond - threadTimes[2]);


return EXIT_SUCCESS;
}
