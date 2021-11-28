/* ---
Alunos: Leonardo omori, Jean Miguel
Data: 20/11/2021
Professor: Rodrigo Campiolo
Trabalho de Sistemas Operacionais
Projeto 1. Resolvendo problemas clássicos de concorrência e sincronização usando
Semáforos e Monitores
--- */

#include <string.h>
#include <stdio.h>
#include "monitor.h"
#include <stdlib.h>
#include <unistd.h>


void initMonitor()
{
    // Inicializar mutex e variáveis de condição
    pthread_mutex_init(&the_mutex, NULL); // mutex para regiao critica
    pthread_cond_init(&condSO, NULL); // fila de alunos de SO para entrar na sala
    pthread_cond_init(&condProfessor, NULL); // fila do professor
    pthread_cond_init(&condComp, NULL); // fila de alunos de computacaoo para entrar na sala
    pthread_cond_init(&condPresentation, NULL); // fila de alunos que estao apresentando
    pthread_cond_init(&condspectators, NULL); // fila dos alunos de compiutacao que estao assistindo
    
}

void destroyMonitor()
{
    // Destroir o mutex e as variáveis de condição inicializados na funcao initMonitor()
    pthread_mutex_destroy(&the_mutex); 
    pthread_cond_destroy(&condSO); 
    pthread_cond_destroy(&condComp); 
    pthread_cond_destroy(&condProfessor);
    pthread_cond_destroy(&condPresentation);
    pthread_cond_destroy(&condspectators);
}


/* --- Métodos do professor--- */

// metodo para liberar a entrada de alunos na sala
void liberar_entrada(){
    pthread_mutex_lock(&the_mutex);
    // setar o numero de estudantes de SO e o numero da fila de apresentacao em 0
    osStudents = 0;
    queuePresentationCount = 0;
    printf("--Professor Campiolo liberou entrada da sala!\n");
    // abrir a flag do lock da sala em false
    lockedRoom = 0;
    //broadcast para a fila de alunos de so que querem entrar na sala
    pthread_cond_broadcast(&condSO);
    // esperar na fila de professor
    pthread_cond_wait(&condProfessor, &the_mutex);
    pthread_mutex_unlock(&the_mutex);
}


// metodo para liberar as apresentacoes e dar lock na sala
void iniciar_apresentacoes(){
    pthread_mutex_lock(&the_mutex);
    // setar flag de lock da sala em true
    lockedRoom = 1;
    printf("--Professor Campiolo iniciou as apresentações\n");
    // enquanto estiver alunos para apresentar continuar no while
    while(queuePresentationCount > 0){
        // sinal para a fila de apresentacao
        pthread_cond_signal(&condPresentation);
        // mandar o professor para a fila de espera para esperar o termino a apresentacao
        pthread_cond_wait(&condProfessor, &the_mutex);
        // sleep de 1 seg
        sleep(1);
        // chamar metodo de atribuir nota
        atribuir_nota();
        // sinal para a thread de alunos que vao assinar a lista de chamanda
        pthread_cond_signal(&condExit);
        // mandar o professor para a fila de espera para esperar o termino da assinatura da lista de chamada
        pthread_cond_wait(&condProfessor, &the_mutex);
        //sleep de 1 seg
        sleep(1);
    }
    pthread_mutex_unlock(&the_mutex);
}


// metodo para atribuir nota para os alunos apos uma apresentacao
void atribuir_nota(){
    // nota randomica entre 0 e 10
    int r = rand() % 11;
    printf("nota do trabalho: %d\n", r);
}


// metodo para fechar as portas da sala apos todas as apresentacoes
void fechar_porta(){
    pthread_mutex_lock(&the_mutex);
    // se o numero de apresentacoes chegar no maximo, terminar o processo
    if(presentationNumber == 20){
        printf("--Pofessor Campiolo fechou a sala!\n");
        exit(0);
    }
    pthread_mutex_unlock(&the_mutex);
}

/* --- Métodos do Aluno de SO--- */

// metodo para o aluno de so entrar na sala
void SO_entrar_sala(char *i, int occupation){
    pthread_mutex_lock(&the_mutex);
    /* --- 
        bloquear entrada caso: 
        . A flag de lock da sala for true
        . O numero de estudante de SO na sala for 5
    --- */
    while (osStudents >4 || lockedRoom)
        pthread_cond_wait(&condSO, &the_mutex);
    printf("Estudante_de_SO_%s entrou na sala\n", i);
    // aumentar o contadore de alunos de SO
    osStudents++;

}

// metodo para o aluno de SO assinar a lista de entrada na sala
void assinar_lista_entrada(char *i){
    printf("Estudante_de_SO_%s Assinou a lista de entrada!\n", i);
    // aumentar o contador de alunos na fila de aprensentacao
    queuePresentationCount++;
}

// metodo para o aluno esperar a sua vez de apresentar
void aguardar_apresentacoes(char *i){
    printf("Estudante_de_SO_%s está aguardando sua apresentação!\n", i);
    // caso exista  5 alunos de computacao na sala
    if(osStudents == 5){
        // caso o numero de espectadores sejam 5
        if(spectatorsNumber == 5){
            // mandar um sinal para o professor
            pthread_cond_signal(&condProfessor);
        }else{
            // caso nao seja 5, dar broadcast nos alunos de comp
            pthread_cond_broadcast(&condComp);
        }
    }
    pthread_cond_wait(&condPresentation, &the_mutex);
    pthread_mutex_unlock(&the_mutex);
}

// o aluno realiza sua apresentacao
void apresentar(char *i){
    pthread_mutex_lock(&the_mutex);
    printf("----Estudante_de_SO_%s está apresentando agora!\n",i);
    // decrementar o numero de alunos de so para apresentar
    queuePresentationCount--;
    // incrementar o numero de apresentacoes total
    presentationNumber++;
    // mandar um sinal para o professor que a apresentacao foi realizada
    pthread_cond_signal(&condProfessor);
    // esperar na fila de saida da sala
    pthread_cond_wait(&condExit, &the_mutex);
    pthread_mutex_unlock(&the_mutex);
}


// metodo para o aluno assinar a lista de saida
void assinar_lista_saida(char *i){
    pthread_mutex_lock(&the_mutex);
    printf("Estudante_de_SO_%s Assinou a lista de saída!\n",i);
    // enviar um sinal para o professor quando temrinar de assinar a lista de saida
    pthread_cond_signal(&condProfessor);
    pthread_mutex_unlock(&the_mutex);
}


/* --- Métodos do Aluno de COMP--- */

//método para o aluno de computação entrar na sala
void COMP_entrar_sala(char *i, int occupation){
    pthread_mutex_lock(&the_mutex);
    /* ---
        Nao entrar na sala quando:
        . Numero de espectadores for maior que 4
        . Numero de estudantes de so for menor que 5
        . A flag lock for true
    --- */

    while (spectatorsNumber > 4 || osStudents < 5 || lockedRoom){    
        pthread_cond_wait(&condComp, &the_mutex);
    }  
    printf("Estudante_de_COMP_%s entrou na sala\n", i);
    // aumentar o contadar de espectadores
    spectatorsNumber++;
    // caso o numero de espectadores for igual a 5
    if(spectatorsNumber == 5){
        // mandar um sinal para o professor
        pthread_cond_signal(&condProfessor);
    }
    pthread_mutex_unlock(&the_mutex);
}

// método para o aluno de computação assistir a apresentação
void assistir_apresentacao(char *i){
    int r; // variavel para receber valor randomico
    while(1){
        // esperar 1 seg
        sleep(1);
        // caso a flag de lock da sala for true
        if(lockedRoom){
            // gerar um numero randomico de 0 a 10
            r = rand() % 11;
            // caso o numero for menor que 2, sair do loop
            if(r<2){
                break;
            }
        }
    }
}
//método para o aluno de computação sair da apresentação
void sair_apresentacao(char *i){
    pthread_mutex_lock(&the_mutex);
    printf("Estudante_de_COMP_%s saiu da apresentação\n", i);
    // diminuir o contador de espectadores
    spectatorsNumber--;
    pthread_mutex_unlock(&the_mutex);
}
