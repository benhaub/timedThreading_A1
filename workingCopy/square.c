/*
 * Authour	:	Ben Haubrich
 * File		:	square.c
 * Synopsis	:	prints data about a multithreaded operation that sqares
 */

/*
 * Standard Headers
 */
#include <windows.h> /* For CreateThread(), Sleep() */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_THREADS 10

/*
* boolean that allows threads to continue execution
*/
bool keepRunning = true;
/*
* Array to store function call counts by each thread. Elements are accessed by
* thread ID's that have been divided by 2 to save space. I have never seen a
* a thread Id much over 20,000 so I went with 25000 divided by two to be safe.
*/
int squareCount[12500];
/*
* We also need to save the indexes to the array that were used
*/
DWORD squareCountIndex[MAX_THREADS];

/*
 * square is a recursive algorithm that calculates the sum of n squares. It's
 * purpose is to create a lot of stack space and take a lot of time.
 * @param n The number up to which the consecutive integers will be squared
 * @return The sum of the squares
 * @precond n must be positive
 * @precond keepRunning must be true
 * 
 * Further Notes:
 * Since this function will be run using multiple threads. it must be defined as
 * a CALLBACK function
 */
int CALLBACK square(int n) {

	/* Actual count is off by one because of the initial call*/
	squareCount[GetCurrentThreadId() / 2] += 1;

	printf("square was called by %u\n", GetCurrentThreadId());

	/* Save the index of the calling thread so we can access counts later*/
	int i;
	for(i = 0; i < MAX_THREADS; i++) {

		/* If we haven't placed a value in i'th spot, put one there*/
		if(squareCountIndex[i] == 0) {

			squareCountIndex[i] = (GetCurrentThreadId() / 2);
			break;
		}
	}
	if(false == keepRunning) {

		LPDWORD exitCode = NULL;
		GetExitCodeThread(GetCurrentThread(), exitCode);
		TerminateThread(GetCurrentThread(), (DWORD)exitCode);
	
		return(n-1 + n + n - 1);
	}

	if(0 == n) {
		
		return 0;
	}
	else if(n < 0) {

		fprintf(stderr, "n can not be negative\n");
		exit(EXIT_FAILURE);
	}
	else {
	
		return(square(n-1) + n + n - 1);
	}
}

int main(int argc, char *argv[]) {

	{
	int i;
	for(i = 0; i < 12500; i++) {

		squareCount[i] = 0;
	}
	}
	
	/*
	* time variables to keep track of elapsed time of threads and to also
	* keep track of the deadline. GetSystemTime() can only be given a
	* a pointer as a parameter using the ampersand. All other ways will
	* cause this call to fail silently and exit the program.
	*/
	SYSTEMTIME time;
	GetSystemTime(&time); 
		
	/*
	* Arrays to store the seconds and milliseconds that each individual
	* thread runs.These times will correspond to the thread stored in 
	* threadArray of the same index.
	*/
	WORD threadTimesSec[MAX_THREADS], threadTimesMsec[MAX_THREADS];

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
	* Time allowed in seconds for the threads to complete
	*/
	DWORD allowedTime = atoi(argv[2]);
	
	/*
	* The sqaure() function will calculate squares until this number
	*/
	DWORD computeSquaresTo = atoi(argv[3]);

	/*
	* Make an array to hold the threads
	*/
	HANDLE threadArray[MAX_THREADS];
 
	/*
	* create the threads
	*/
	DWORD i;
	for(i = 0; i < numThreads; i++) {
	
		/*
		* Notes about CreateThread()
		* You can use a struct if you need to pass more than one
		* parameter to the threaded function
		* &square and square are equivalent. They both give the
		* location in the code of the square function.
		*/
		
		threadArray[i] = CreateThread(
					NULL,
					0,
					(LPTHREAD_START_ROUTINE) square,
					&computeSquaresTo,
					CREATE_SUSPENDED,
					NULL);
	
		if(NULL == threadArray[i]) {

			fprintf(stderr, "CreateThread() failed, and could not create the %d'th thread", i);
		}
	}

	for(i = 0; i < numThreads; i++) {

	/*
	* Start the threads and save the time they started
	*/
	ResumeThread(threadArray[i]);
	GetSystemTime(&time);
	threadTimesSec[i] = time.wSecond;
	threadTimesMsec[i] = time.wMilliseconds;
	}

	/*
	* This thread will sleep for the allowed time. When it wakes it will
	* set keepRunning to false, thereby causing all threads to close
	*/
	Sleep(allowedTime * 1000);
	keepRunning = false;

	/*
	* close all HANDLE's
	*/
	for(i = 0; i < numThreads; i++) {

		CloseHandle(threadArray[i]);
	}

	/*
	* Get the time, and subtract from start time to find total execution
	*/
	GetSystemTime(&time);
	
	/*
	* Print execution times for each thread and the number of calls to
	* square that each thread made
	*/
	int index;
	for(i = 0; i < numThreads; i++) {
		
		index = squareCountIndex[i]; 
		printf("Time for thread %u: %u seconds and %d milliseconds\nCalls for thread %u: %d.\n", i, (time.wSecond - threadTimesSec[i]), (time.wMilliseconds - threadTimesMsec[i]), i, squareCount[index]);
		/*printf("Calls for thread %u: %d.\n", i, squareCount[index]);*/
	
	}

return EXIT_SUCCESS;
}
