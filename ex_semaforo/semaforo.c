#include <string.h>
#include <stdio.h>
#include "semaforo.h"
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <pthread.h>

// metodo para inicar os semaforos
void initSemaphore(){
    sem_init(&mutex,0,1); // mutex para regiao critica
    sem_init(&semaphoreProfessor,0,0); // semafoor para o professor
    sem_init(&semaphoreSO,0,0); // semaforo para os alunos de SO que querem entrar na sala
    sem_init(&semaphoreComp,0,0);//  semaforo para os alunos de Comp que querem entrar na sala
    sem_init(&semaphoreRoomLock,0,1); // semaforo para a regiao critica do lock da sala
    sem_init(&semaphoreSpectators,0,0); // semaforo para os espectadores
    // loop para iniciar os semaforos das apresentacoes
    for(int i = 0; i < 5; i++)
        sem_init(&semaphoresPresentation[i],0,0); // semaforo do aluno que vai apresentar

    sem_init(&semaphoreSpectators,0,0); // semaforo do espectador
}

// metodo para destuir os semaforos
void destroySemaphore(){
    sem_destroy(&mutex);
    sem_destroy(&semaphoreProfessor);
    sem_destroy(&semaphoreSO);
    sem_destroy(&semaphoreComp);
    sem_destroy(&semaphoreRoomLock);
    sem_destroy(&semaphoreSpectators);
    for(int i = 0; i < 5; i++)
        sem_destroy(&semaphoresPresentation[i]);
}

/* --- Métodos do professor--- */

// metodo para liberas a sala
void liberar_entrada(){

    //regiao critica
    sem_wait(&mutex);
    // setar os contadores em 0 
    osStudent = 0;
    studentPresenting = 0;
    idPresenting = 0;
    //sair da regiao critica
    sem_post(&mutex);
    printf("--Professor Campiolo liberou Entrada\n");
    // chamar os estudantes de SO
    sem_post(&semaphoreSO);
    // fazer o professor esperar
    sem_wait(&semaphoreProfessor);
}

// metodo para atribuir nota para o aluno
void atribuir_nota(){
    // gerar um numero randomico
    int r = rand() % 11; 
    // atribuir a nota com esse numero
    printf("NOTA:%d\n",r);
}

// metodo para inicar as apresentacoes
void iniciar_apresentacoes(){
    // entrar na regia critica
    sem_wait(&semaphoreRoomLock);
    printf("--Professor Campiolo iniciou as apresentaçoes\n");
    // setar a flag lock da sala em false
    roomLock = 0;
    // sair da regiao critica
    sem_post(&semaphoreRoomLock);
    // loop para todos os alunos de so na sala apresentarem
    for(int i = 0; i < 5; i++){
        // entrar na regia critica
        sem_wait(&mutex);
        // setar qual aluno esta apresentando
        studentPresenting = i;
        //sair da regiao critica
        sem_post(&mutex);
        // chamar o proximo aluno que ira apresentar
        sem_post(&semaphoresPresentation[i]);
        // esperar o aluno apresentar
        sem_wait(&semaphoreProfessor);
        // dar uma nota 
        atribuir_nota();
        // chamar novamente o aluno para assinar a lista de saida
        sem_post(&semaphoresPresentation[i]);
        // esperar o aluno assinar a lista de saida
        sem_wait(&semaphoreProfessor);
        // sleeep de 1 seg
        sleep(1);
    }
    // entrar na regiao critica
    sem_wait(&semaphoreRoomLock);
    // setar a flag lock da sala em true
    roomLock = 1;
    // sair da regiao critica
    sem_post(&semaphoreRoomLock);
}

// metodo para o professor fechar as portas 
void fechar_porta(){
    // se o numero de apresentacoes chegar no maximo, terminar o processo
    if(presentationNumber == 20){
        printf("--Pofessor Campiolo fechou a sala!\n");
        exit(0);
    }
}

/* --- Métodos de aluno de so --- */

// metodo par ao aluno de so entrar na sala
void SO_entrar_sala(char* i){
    // esperar o professo liberar a sala
    sem_wait(&semaphoreSO);
    printf("Estudante_de_SO_%s entrou na sala\n", i);
}

void assinar_lista_entrada(char *i){
    // entrar na regiao critica
    sem_wait(&mutex);
    // incrementar o contador de estudante de SO
    osStudent++;
    // sair da regiao critica 
    sem_post(&mutex);
    printf("Estudante_de_SO_%s Assinou a lista de entrada!\n", i);
}

void aguardar_apresentacoes(char *i){ 
    // entrar na regiao critica
    sem_wait(&mutex);
    // incrementar o id do estudante que vai apresnetar para o correto
    idPresenting++;
    // sair da regiao critica
    sem_post(&mutex);
    printf("Estudante_de_SO_%s está aguardando sua apresentação!\n", i);
    // entrar na regiao critica
    sem_wait(&mutex);
    // incrementar os estudantes que estao apresentando
    studentPresenting++;
    // sair da regiao critica
    sem_post(&mutex);
    // caso o numero de estudante de so na sala for menor que 5
    if(osStudent < 5 ){
        // chamar o proximo aluno de so
        sem_post(&semaphoreSO);
    }else{
        // caso o numero de estudante de so for maior ou igual a 5
        // chamar um estudante de comp
        sem_post(&semaphoreComp);
    }
    // esperar na fila de estundate que vai apresentar
    sem_wait(&semaphoresPresentation[idPresenting-1]);
}

// metodo para o aluno de So fazer a apresentacao
void apresentar(char *i){
    printf("----Estudante_de_SO_%s está apresentando agora!\n",i);
    // entrar na regiao critica
    sem_wait(&mutex);
    // incrementar o numero de apresentacao
    presentationNumber++;
    // sair da regiao critica
    sem_post(&mutex);
    // chamar o professor
    sem_post(&semaphoreProfessor);
    // esperar o professor chamar 
    sem_wait(&semaphoresPresentation[studentPresenting]);

}

// metodo para assinar a lista de saida
void assinar_lista_saida(char *i){
    printf("Estudante_de_SO_%s Assinou a lista de saída!\n",i);
    // entrar na regiao critica
    sem_wait(&mutex);
    // decrementar o numero de estudanter de SO na sala
    osStudent--;
    // sair da regiao critica
    sem_post(&mutex);
    // chamar o professor
    sem_post(&semaphoreProfessor);
}

/* --- Métodos de aluno de comp --- */
void COMP_entrar_sala(char *i){
    // esperar os estudante de So entrarem todos na sala
    sem_wait(&semaphoreComp);
    //entrar na regia critica
    sem_wait(&mutex);
    spectatorsNumber++;
    // sair da regiao critica
    sem_post(&mutex);
    printf("Estudante_de_COMP_%s entrou na sala %d\n", i,spectatorsNumber);
    // caso o numero de espectadores for menor de 4
    if(spectatorsNumber < 4){
        // chamar outro aluno de comp
        sem_post(&semaphoreComp);
    }else{
        // caso o numero de espectador for maior ou igual a 4
        // chamar o professor
        sem_post(&semaphoreProfessor);
    }
}

// metodo para o espectador assistir a apresentacao
void assistir_apresentacao(char *i){
    int r;// variavel para receber um numero randomico
    // o aluno esperando sair da sala em um loop
    while(1){
        // sleep de 1 seg
        sleep(1);
        // caso o flag de lock da sala for igual a 0
        if(!roomLock){
            // r recebe um numnero entre 0 - 10
            r = rand() % 11;
            // caso o numero seja menor que 2
            if(r<2){
                // o espectador sai da sala
                break;
            }
        }
    }
}

// metodo para o espectador sair da sala
void sair_apresentacao(char *i){
    // entrar na regia critica
    sem_wait(&semaphoreRoomLock);
        printf("Estudante_de_COMP_%s saiu da apresentação\n", i);
        // decrementar o contador de espectadores
        spectatorsNumber--;
    // sair da regiao critica
    sem_post(&semaphoreRoomLock);
}