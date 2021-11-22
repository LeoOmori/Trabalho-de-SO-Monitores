/* ---
Alunos: Leonardo omori, Jean Miguel
Data: 20/11/2021
Professor: Rodrigo Campiolo
Trabalho de Sistemas Operacionais
Projeto 1. Resolvendo problemas clássicos de concorrência e sincronização usando
Semáforos e Monitores
--- */

#ifndef MONITOR_H
#define MONITOR_H

#include <pthread.h>

//* --- Controle de concorrência --- */
pthread_mutex_t the_mutex;
/* --- 
filas: 
    . Professor
    . Alunos de So querem entrar na sala
    . Alunos de comp querem entrar na sala
    . Alunos de So que vao apresentar
    . Alunos de So que terminaram a apresentacao
    . Alunos de comp que estao assistindo a apresentacao

 --- */
pthread_cond_t condProfessor, condSO, condComp, condPresentation, condExit, condspectators;

// /* --- Recursos --- */

// lugares abertos na sala
int availablePlaces = 10;
// numeros de estudante de SO na sala
int osStudents = 0;
// flag de lock para a sala
int lockedRoom = 1;
// numero de estudante de So na fila de apresentacao
int queuePresentationCount = 0;
// numero de apresentacoes terminadas
int presentationNumber = 0;


/* --- monitor operations --- */
void initMonitor();
void destroyMonitor();

/* --- Métodos do professor--- */
void iniciar_apresentacoes();
void liberar_entrada();
void atribuir_nota();
void fechar_porta();


/* --- Métodos do Aluno de SO--- */
void SO_entrar_sala();
void assinar_lista_entrada();
void aguardar_apresentacoes();
void apresentar();
void assinar_lista_saida();


/* --- Métodos dos alunos de comp--- */
void COMP_entrar_sala();
void assistir_apresentacao();
void sair_apresentacao();

#endif
