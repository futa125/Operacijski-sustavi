#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>

int isPaused = 0;
int currNum = 1000000001;
int lastPrime = 1000000001;

void periodicWrite()
{
    printf("Last prime number = %d\n", lastPrime);
}

void setPaused()
{
    if(isPaused) {
        printf("[Resuming...]\n");
    } else {
        printf("[Pausing...]\n");
    }
    isPaused = 1 - isPaused;
}

void exitProgram()
{
    printf("[Exiting...] Last prime number = %d\n", lastPrime);
    exit(0);
}

int isPrime(unsigned long n)
{
    unsigned long i, max;

    if ((n & 1) == 0)
        return 0;

    max = sqrt(n);
    for (i = 3; i <= max; i += 2)
        if ((n % i) == 0)
            return 0;

    return 1;
}

int main()
{
    sigset(SIGTERM, exitProgram);
    sigset(SIGINT, setPaused);

    struct itimerval t;
    sigset(SIGALRM, periodicWrite);

    t.it_value.tv_sec = 5;
	t.it_value.tv_usec = 0;

	t.it_interval.tv_sec = 5;
	t.it_interval.tv_usec = 0;

    setitimer(ITIMER_REAL, &t, NULL);

    while (1) {
        if (isPrime(currNum)) {
            lastPrime = currNum;
        }
        currNum++;
      
        while (isPaused == 1) {
            pause();
        }
    }
}