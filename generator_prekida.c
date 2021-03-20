#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int pid=0;

void prekidna_rutina(int sig){
   kill(sig, SIGKILL);
   exit(0);
}

int main(int argc, char *argv[]){
   pid=atoi(argv[1]);
   sigset(SIGINT, prekidna_rutina);

   while(1){
        
        int signals[4] = {SIGUSR1, SIGUSR2, SIGILL, SIGHUP};
        int randomSignal = signals[rand() % 4];

        switch (randomSignal) {
            case SIGUSR1:
                kill(pid, SIGUSR1);
                break;
            case SIGUSR2:
                kill(pid, SIGUSR2);
                break;
            case SIGILL:
                kill(pid, SIGILL);
                break;
            case SIGHUP:
                kill(pid, SIGHUP);
                break;
        }

        sleep((rand() % (5 - 3 + 1)) + 3);
   }
   return 0;
}

