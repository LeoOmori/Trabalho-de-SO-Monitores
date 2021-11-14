#ifndef MONITOR_H
#define MONITOR_H


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "monitor.h"
#include <unistd.h>

void *estudanteSo(void *ptr){
    int* i = (int *)ptr;
    entrar_sala(*(i));
}

int main(){
    
    initMonitor();

    pthread_t estudanteC[20];
    int aux = 1;
    for(int i = 0; i < 20; i++){
        pthread_create(&estudanteC[i], NULL, estudanteSo, &i);
    }
    for(int i = 0; i < 20; i++){
        pthread_join(estudanteC[i], NULL);
    }

    //Destroy monitor
    destroyMonitor();

    return 0;

}


#endif