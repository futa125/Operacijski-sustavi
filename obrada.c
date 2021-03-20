#include <stdio.h>
#include <signal.h>
 
#define N 6
 
int OZNAKA_CEKANJA[N];
int PRIORITET[N];
int TEKUCI_PRIORITET;

 
int sig[]={SIGUSR1, SIGUSR2, SIGILL, SIGHUP, SIGINT};


void zabrani_prekidanje(){
   int i;
   for(i=0; i<5; i++)
      sighold(sig[i]);
}
void dozvoli_prekidanje(){
   int i;
   for(i=0; i<5; i++)
      sigrelse(sig[i]);
}
 
void obrada_signala(int i){
    switch (i)
    {
    case 1:
        sleep(1);
        printf(" -  P  -  -  -  - \n");
        for(i = 1; i <= 5; i++) {
            sleep(1);
            printf(" -  %d  -  -  -  - \n", i);
        }
        printf(" -  K  -  -  -  - \n");
        break;
    case 2:
        sleep(1);
        printf(" -  -  P  -  -  - \n");
        for(i = 1; i <= 5; i++) {
            sleep(1);
            printf(" -  -  %d  -  -  - \n", i);
        }
        printf(" -  -  K  -  -  - \n");
        break;
    case 3:
        sleep(1);
        printf(" -  -  -  P  -  - \n");
        for(i = 1; i <= 5; i++) {
            sleep(1);
            printf(" -  -  -  %d  -  - \n", i);
        }
        printf(" -  -  -  K  -  - \n");
        break;
    case 4:
        sleep(1);
        printf(" -  -  -  -  P  - \n");
        for(i = 1; i <= 5; i++) {
            sleep(1);
            printf(" -  -  -  -  %d  - \n", i);
        }
        printf(" -  -  -  -  K  - \n");
        break;
    case 5:
        sleep(1);
        printf(" -  -  -  -  -  P \n");
        for(i = 1; i <= 5; i++) {
            sleep(1);
            printf(" -  -  -  -  -  %d \n", i);
        }
        printf(" -  -  -  -  -  K \n");
        break;
    }
}
void prekidna_rutina(int sig){
   int n=-1;
   zabrani_prekidanje();
   switch(sig){
        case SIGUSR1: 
            n=1; 
            printf(" -  X  -  -  -  - \n");
            break;
        case SIGUSR2: 
            n=2; 
            printf(" -  -  X  -  -  - \n");
            break;
        case SIGILL:
            n=3;
            printf(" -  -  -  X  -  - \n");
            break;
        case SIGHUP:
            n=4;
            printf(" -  -  -  -  X  - \n");
            break;
        case SIGINT:
            n=5;
            printf(" -  -  -  -  -  X \n");
            break;
    }
    OZNAKA_CEKANJA[n]=1;
    int x;
    
    do {

        x = 0;
        for (int i = TEKUCI_PRIORITET + 1; i < N; i++) {
            if (OZNAKA_CEKANJA[i] != 0) {
                x = i;
            }
        }
        
        if (x > 0) {
            OZNAKA_CEKANJA[x] = 0;
            PRIORITET[x] = TEKUCI_PRIORITET;
            TEKUCI_PRIORITET = x;
            dozvoli_prekidanje();
            obrada_signala(x);
            zabrani_prekidanje();
            TEKUCI_PRIORITET = PRIORITET[x];
        }

   } while (x > 0);

   dozvoli_prekidanje();
}
 
int main ( void )
{
   sigset (SIGUSR1, prekidna_rutina);
   sigset (SIGUSR2, prekidna_rutina);
   sigset (SIGILL, prekidna_rutina);
   sigset (SIGHUP, prekidna_rutina);
   sigset (SIGINT, prekidna_rutina);
 
   printf("Proces obrade prekida, PID=%ld\n", getpid());
   printf("GP S1 S2 S3 S4 S5\n");
   printf("-----------------\n");
   
   for (int i = 1; i < 60; i++) {
       printf("%2d  -  -  -  -  - \n", i);
       sleep(1);
   }
 
   printf ("Zavrsio osnovni program\n");
 
   return 0;
}
