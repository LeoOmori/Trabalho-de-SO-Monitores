/* ---
Alunos: Leonardo omori, Jean Miguel
Data: 20/11/2021
Professor: Rodrigo Campiolo
Trabalho de Sistemas Operacionais
Projeto 1. Resolvendo problemas clássicos de concorrência e sincronização usando
Semáforos e Monitores
--- */


#ifndef SEMAFORO_H
#define SEMAFORO_H

#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include "semaforo.h"
#include <unistd.h>

// numero de alunos de SO
#define N 20

// thread de alunos de SO chamando todos os metodos
void *estudanteSo(void *Iname){
    SO_entrar_sala(Iname);
    assinar_lista_entrada(Iname);
    aguardar_apresentacoes(Iname);
    apresentar(Iname);
    assinar_lista_saida(Iname);
    pthread_exit(NULL);
}

// thread de alunos de comp chamando todos os metodos
void *estudanteComp(void *Iname){

    COMP_entrar_sala(Iname);
    // assistir_apresentacao(Iname);
    // sair_apresentacao(Iname);
    pthread_exit(NULL);
}

// thread do professor chamando todos os metodos
void *professorThread(){
    //contador
    int j = 0;
    /* ---
        laco para repetir os metodos do professor 4 vezes, pois serao 20 alunos de SO e 5 por 
        rodada de apresentacao
    ---  */ 
    while(j<4){
        liberar_entrada();
        iniciar_apresentacoes();
        atribuir_nota();
        fechar_porta();
        j++;
    }
    pthread_exit(NULL);

}

int main(){

    // nomes de todos os alunos
    char *namesSO[20] =  { 
    "Lynnell Bortolussi",
    "Robyn Revett",
    "Kalil Sarra",
    "Jessalyn Hargroves",
    "Angeline Yakunikov",
    "Amalee Duval",
    "Riccardo Aitken",
    "Robinet Wither",
    "Minnie Ianno",
    "Flemming De Stoop",
    "Mervin Goundsy",
    "Vanny Banks",
    "Gaylene Pudan",
    "Ezechiel Cleynaert",
    "Elonore Darrach",
    "Robbie Severs",
    "Ardelle Joysey",
    "Eleanor Romanetti",
    "Dante Varley",
    "Montgomery Desport"
    };

    char* namesComp[20] = {
        "Jose Lima","Fabio Carlos",
        "Souza lemes","Patricia Karla","Suzana Santos",
        "Joao Carlos","Ronaldo Henrique","Ricardo Filho",
        "Alana Katrine","Pedro Henrike","Jaime Olimpio","Janaina Gonçalves",
        "Caio Akamura","Eduarda Carolina","Ana Paula",
        "Elizabethe Coelho","Talles Silva","Marko Henrique",
        "André Pereira", "Thalia Aline"
    };
    // inicializar semáforos
    
    initSemaphore();


    // threads de estudantes de SO, estudantes de comp e professor
    pthread_t estudanteC[N], estudanteSO[2], professor;

    //criar threads
   
    // N thread de estudantes de comp
    for(int i = 0; i < N; i++){
        pthread_create(&estudanteC[i], NULL, estudanteComp, namesComp[i]);
    }

    // N thread de estudantes de SO
    for(int i = 0; i < N; i++){
        pthread_create(&estudanteSO[i], NULL, estudanteSo, namesSO[i]);
    }

    // uma thread de professor
    pthread_create(&professor, NULL, professorThread, NULL);

    //inicializar threads

    // inicializar estudantesSO
    for(int i = 0; i < N; i++){
        pthread_join(estudanteSO[i], NULL);
    }
    // inicializar estudantes de comp 
    for(int i = 0; i < N; i++){
        pthread_join(estudanteC[i], NULL);
    }
    // inicizalizar thread do professor
    pthread_join(professor, NULL);

    //destruir semáforos
    destroySemaphore();
    
    return 0;

}


#endif