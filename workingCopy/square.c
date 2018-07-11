/*
 * Authour	:	Ben Haubrich
 * File		:	square.c
 * Synopsis	:	prints data about a multithreaded operation that sqares
 *			N integers
 */

/*
 * Standard Headers
 */
/*#include <windows.h>*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * Control variable that lets child threads continue to run
 */
bool keepRunning = true;

/*
 * square is a recursive algorithm that calculates the sum of n squares. It's
 * purpose is to create a lot of stack space and take a lot of time.
 * @param n The number up to which the consecutive integers will be squared
 * @return The sum of the squares
 * @precond n must be positive
 */
int square(int n) {

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

	int n = 6;
	printf("%d\n", square(n));

return EXIT_SUCCESS;
}
