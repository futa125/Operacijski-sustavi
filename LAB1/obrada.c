#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED

#define N 6

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int OZNAKA_CEKANJA[N];
int PRIORITET[N];
int TEKUCI_PRIORITET;

int sig[] = {SIGUSR1, SIGUSR2, SIGILL, SIGHUP, SIGINT};

void ispisi_podatke() {
    printf("OZNAKA_CEKANJA: [ ");

    for (int i = 0; i < N; i++) {
        printf("%d ", OZNAKA_CEKANJA[i]);
    }

    printf("] ");

    printf("PRIORITET: [ ");
    
    for (int i = 0; i < N; i++) {
        printf("%d ", PRIORITET[i]);
    }

    printf("] ");

    printf("TEKUCI_PRIORITET: %d", TEKUCI_PRIORITET);
}

void zabrani_prekidanje() {
    int i;
    for(i = 0; i < 5; i++) {
        sighold(sig[i]);
    }
}

void dozvoli_prekidanje() {
    int i;
    for(i = 0; i < 5; i++) {
        sigrelse(sig[i]);
    }
}

void obrada_signala(int x) 
{
    switch (x) {
        case 1:
            printf(" -  P  -  -  -  - ");
            ispisi_podatke();
            printf("\n");
            for(int i = 1; i <= 5; i++) {
                sleep(1);
                printf(" -  %d  -  -  -  - ", i);
                ispisi_podatke();
                printf("\n");
            }
            printf(" -  K  -  -  -  - ");
            ispisi_podatke();
            printf("\n");
            break;
        case 2:
            printf(" -  -  P  -  -  - ");
            ispisi_podatke();
            printf("\n");
            for(int i = 1; i <= 5; i++) {
                sleep(1);
                printf(" -  -  %d  -  -  - ", i);
                ispisi_podatke();
                printf("\n");
            }
            printf(" -  -  K  -  -  - ");
            ispisi_podatke();
            printf("\n");
            break;
        case 3:
            printf(" -  -  -  P  -  - ");
            ispisi_podatke();
            printf("\n");
            for(int i = 1; i <= 5; i++) {
                sleep(1);
                printf(" -  -  -  %d  -  - ", i);
                ispisi_podatke();
                printf("\n");
            }
            printf(" -  -  -  K  -  - ");
            ispisi_podatke();
            printf("\n");
            break;
        case 4:
            printf(" -  -  -  -  P  - ");
            ispisi_podatke();
            printf("\n");
            for(int i = 1; i <= 5; i++) {
                sleep(1);
                printf(" -  -  -  -  %d  - ", i);
                ispisi_podatke();
                printf("\n");
            }
            printf(" -  -  -  -  K  - ");
            ispisi_podatke();
            printf("\n");
            break;
        case 5:
            sleep(1);
            printf(" -  -  -  -  -  P ");
            ispisi_podatke();
            printf("\n");
            for(int i = 1; i <= 5; i++) {
                sleep(1);
                printf(" -  -  -  -  -  %d ", i);
                ispisi_podatke();
                printf("\n");
            }
            printf(" -  -  -  -  -  K ");
            ispisi_podatke();
            printf("\n");
            break;
    }
}

void prekidna_rutina(int sig)
{
    int n = -1;
    zabrani_prekidanje();

    switch(sig) {
        case SIGUSR1: 
            n = 1; 
            printf(" -  X  -  -  -  - ");
            ispisi_podatke();
            printf("\n");
            break;
        case SIGUSR2: 
            n = 2; 
            printf(" -  -  X  -  -  - ");
            ispisi_podatke();
            printf("\n");
            break;
        case SIGILL:
            n = 3;
            printf(" -  -  -  X  -  - ");
            ispisi_podatke();
            printf("\n");
            break;
        case SIGHUP:
            n = 4;
            printf(" -  -  -  -  X  - ");
            ispisi_podatke();
            printf("\n");
            break;
        case SIGINT:
            n = 5;
            printf(" -  -  -  -  -  X ");
            ispisi_podatke();
            printf("\n");
            break;
    }

    OZNAKA_CEKANJA[n] += 1;
    int x;
 
    do {
        x = 0;

        for(int i = TEKUCI_PRIORITET + 1; i < N; i++) {
            if(OZNAKA_CEKANJA[i] != 0) {
                x = i;
            }
        }
        
        if(x > 0) {
            OZNAKA_CEKANJA[x] -= 1;
            PRIORITET[x] = TEKUCI_PRIORITET;
            TEKUCI_PRIORITET = x;

            dozvoli_prekidanje();
            obrada_signala(x);
            zabrani_prekidanje();

            TEKUCI_PRIORITET = PRIORITET[x];
            PRIORITET[x] = 0;
        }

    } while(x > 0);

    dozvoli_prekidanje();
}
 
int main() 
{
    sigset(SIGUSR1, prekidna_rutina);
    sigset(SIGUSR2, prekidna_rutina);
    sigset(SIGILL, prekidna_rutina);
    sigset(SIGHUP, prekidna_rutina);
    sigset(SIGINT, prekidna_rutina);
 
    printf("Proces obrade prekida, PID=%ld\n", getpid());
    printf("GP S1 S2 S3 S4 S5\n");
    printf("-----------------\n");
    
    for(int i = 1; i <= 99; i++) {
        sleep(1);
        printf("%2d  -  -  -  -  - ", i);
        ispisi_podatke();
        printf("\n");
    }
 
    printf("Zavrsio osnovni program\n");
 
    return 0;
}
