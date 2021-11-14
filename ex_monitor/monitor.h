#ifndef MONITOR_H
#define MONITOR_H

#include <pthread.h>

// /* --- Controle de concorrência --- */
pthread_mutex_t the_mutex;
pthread_cond_t condProfessor, condSO, condComp;

// /* --- Recursos --- */
int availablePlaces = 10;

/* --- monitor operations --- */
void initMonitor();
void destroyMonitor();

/* --- Métodos do professor--- */
void iniciar_apresentacoes();
void liberar_entrada();
void atribuir_nota();
void fechar_porta();


/* --- Métodos do Aluno de SO--- */
void entrar_sala();
void assinar_lista_entrada();
void aguardar_apresentacoes();
void apresentar();
void assinar_lista_saida();

/* --- Métodos dos alunos de c--- */
void entrar_sala();
void assistir_apresentacao();
void sair_apresentacao();

#endif
