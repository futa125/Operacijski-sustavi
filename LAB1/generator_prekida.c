#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int pid=0;

void prekidna_rutina(int sig)
{
   kill(sig, SIGKILL);
   exit(0);
}

int main(int argc, char *argv[]) 
{
   pid = atoi(argv[1]);
   sigset(SIGINT, prekidna_rutina);
   srand(time(0));

   while (1) {
        int sig[4] = {SIGUSR1, SIGUSR2, SIGILL, SIGHUP};
        int rand_sig = sig[rand() % 4];

        kill(pid, rand_sig);

        sleep((rand() % (5 - 3 + 1)) + 3);
   }

   return 0;
}

