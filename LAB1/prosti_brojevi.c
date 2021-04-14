#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>

int pauza = 0;
int broj = 1000000001;
int zadnji = 1000000001;

void periodicni_ispis()
{
    printf("Zadnji prosti broj = %d\n", zadnji);
}

void postavi_pauzu()
{
    if (pauza)
    {
        printf("[Nastavljam...]\n");
    }
    else
    {
        printf("[Pauziram...]\n");
    }
    pauza = 1 - pauza;
}

void zaustavi_program()
{
    printf("[Zaustavljam...]\n");
    printf("Zadnji prosti broj = %d\n", zadnji);
    exit(0);
}

int prost(unsigned long n)
{
    unsigned long i, max;

    if ((n & 1) == 0)
    {
        return 0;
    }

    max = sqrt(n);

    for (i = 3; i <= max; i += 2)
    {
        if ((n % i) == 0)
        {
            return 0;
        }
    }

    return 1;
}

int main()
{
    sigset(SIGTERM, zaustavi_program);
    sigset(SIGINT, postavi_pauzu);

    struct itimerval t;
    sigset(SIGALRM, periodicni_ispis);

    t.it_value.tv_sec = 5;
    t.it_value.tv_usec = 0;

    t.it_interval.tv_sec = 5;
    t.it_interval.tv_usec = 0;

    setitimer(ITIMER_REAL, &t, NULL);

    while (1)
    {
        if (prost(broj))
        {
            zadnji = broj;
        }
        broj++;

        while (pauza == 1)
        {
            pause();
        }
    }
}