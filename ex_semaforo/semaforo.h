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

#include <sys/types.h>  /* Primitive System Data Types */ 
#include <semaphore.h>

sem_t 
mutex,
semaphoreSO, 
semaphoreProfessor,
semaphoreComp, 
semaphoresPresentation[5], 
semaphoreSpectators;

// Numero de lugares disponiveis
int osStudent = 0;
int studentPresenting = 0;
int idPresenting = 0;
int idSpectationg = 0;
int presentationNumber = 0;
int spectatorsNumber = 0;
int spectatorIndex = 0;

/* --- monitor operations --- */
void initSemaphore();
void destroySemaphore();

/* --- Métodos do professor--- */
void liberar_entrada();
void iniciar_apresentacoes();
void atribuir_nota();
void fechar_porta();

/* --- Métodos do Aluno de SO--- */
void SO_entrar_sala();
void assinar_lista_entrada();
void aguardar_apresentacoes();
void apresentar();
void assinar_lista_saida();


/* --- Métodos dos alunos de c--- */
void COMP_entrar_sala();
void assistir_apresentacao();
void sair_apresentacao();

#endif
