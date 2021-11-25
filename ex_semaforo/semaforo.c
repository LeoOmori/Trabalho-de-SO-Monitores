#include <string.h>
#include <stdio.h>
#include "semaforo.h"
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <pthread.h>


void initSemaphore(){
    sem_init(&mutex,0,1);
    sem_init(&semaphoreProfessor,0,0);
    sem_init(&semaphoreSO,0,0);
    sem_init(&semaphoreComp,0,0);
    sem_init(&semaphoreRoomLock,0,1);
    sem_init(&semaphoreSpectators,0,0);
    for(int i = 0; i < 5; i++)
        sem_init(&semaphoresPresentation[i],0,0);

    sem_init(&semaphoreSpectators,0,0);
}

void destroySemaphore(){

}

/* --- Métodos do professor--- */
void liberar_entrada(){
    sem_wait(&mutex);
    osStudent = 0;
    studentPresenting = 0;
    idPresenting = 0;
    sem_post(&mutex);
    printf("--Professor Campiolo liberou Entrada\n");
    sem_post(&semaphoreSO);
    sem_wait(&semaphoreProfessor);
}

void atribuir_nota(){
    int r = rand() % 11;
    printf("NOTA:%d\n",r);
}

void iniciar_apresentacoes(){
    sem_wait(&semaphoreRoomLock);
    printf("--Professor Campiolo iniciou as apresentaçoes\n");
    roomLock = 0;
    sem_post(&semaphoreRoomLock);
    for(int i = 0; i < 5; i++){
        sem_wait(&mutex);
        studentPresenting = i;
        spectatorIndex = 0;
        sem_post(&mutex);
        sem_post(&semaphoresPresentation[i]);
        sem_wait(&semaphoreProfessor);
        atribuir_nota();
        sem_post(&semaphoresPresentation[i]);
        sem_wait(&semaphoreProfessor);
        sleep(1);
    }
    sem_wait(&semaphoreRoomLock);
    roomLock = 1;
    sem_post(&semaphoreRoomLock);
}

void fechar_porta(){
    // se o numero de apresentacoes chegar no maximo, terminar o processo
    if(presentationNumber == 20){
        printf("--Pofessor Campiolo fechou a sala!\n");
        exit(0);
    }
}

/* --- Métodos de aluno de so --- */
void SO_entrar_sala(char* i){
    sem_wait(&semaphoreSO);
    sem_wait(&mutex);
    printf("Estudante_de_SO_%s entrou na sala\n", i);
    sem_post(&mutex);
}

void assinar_lista_entrada(char *i){
    osStudent++;
    printf("Estudante_de_SO_%s Assinou a lista de entrada!\n", i);
}

void aguardar_apresentacoes(char *i){ 
    sem_wait(&mutex);
    idPresenting++;
    sem_post(&mutex);
    printf("Estudante_de_SO_%s está aguardando sua apresentação!\n", i);
    sem_wait(&mutex);
    studentPresenting++;
    sem_post(&mutex);
    if(osStudent < 5 ){
        sem_post(&semaphoreSO);
    }else{
        sem_post(&semaphoreComp);
    }
    sem_wait(&semaphoresPresentation[idPresenting-1]);
}

void apresentar(char *i){
    printf("----Estudante_de_SO_%s está apresentando agora!\n",i);
    sem_wait(&mutex);
    presentationNumber++;
    sem_post(&mutex);
    sem_post(&semaphoreProfessor);
    sem_wait(&semaphoresPresentation[studentPresenting]);

}

void assinar_lista_saida(char *i){
    printf("Estudante_de_SO_%s Assinou a lista de saída!\n",i);
    sem_wait(&mutex);
    osStudent--;
    sem_post(&mutex);
    sem_post(&semaphoreProfessor);
}

/* --- Métodos de aluno de comp --- */
void COMP_entrar_sala(char *i){
    sem_wait(&semaphoreComp);
    
    sem_wait(&mutex);
    spectatorsNumber++;
    printf("Estudante_de_COMP_%s entrou na sala %d\n", i,spectatorsNumber);
    sem_post(&mutex);
    if(spectatorsNumber < 4){
        sem_post(&semaphoreComp);
    }else{
        sem_post(&semaphoreProfessor);
    }
}

void assistir_apresentacao(char *i){
    int r;
    while(1){
        sleep(1);
        if(!roomLock){
            r = rand() % 11;
            if(r<2){
                break;
            }
        }
    }
}

void sair_apresentacao(char *i){
    sem_wait(&mutex);
        printf("Estudante_de_COMP_%s saiu da apresentação\n", i);
        spectatorsNumber--;
    sem_post(&mutex);
}