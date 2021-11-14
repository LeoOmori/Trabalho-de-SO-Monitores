#include<stdio.h>
#include "monitor.h"

void initMonitor()
{
    // Inicializar mutex e variáveis de condição
    pthread_mutex_init(&the_mutex, NULL);
    pthread_cond_init(&condProfessor, NULL); /* Variável de condição de professor */
    pthread_cond_init(&condSO, NULL); /* Variável de condição de estudante de SO */
    pthread_cond_init(&condComp, NULL); /* Variável de condição de estudante de Computação */
}

void destroyMonitor()
{
    // Destroir o mutex e as variáveis de condição
    pthread_mutex_destroy(&the_mutex); /* Free up the_mutex */
    pthread_cond_destroy(&condProfessor); /* Variável de condição de professor */
    pthread_cond_destroy(&condSO); /* Variável de condição de estudante de SO */
    pthread_cond_destroy(&condComp); /* Variável de condição de estudante de Computação */
}


/* --- Métodos do Aluno de SO--- */
void entrar_sala(int i){
    pthread_mutex_lock(&the_mutex); /* protect buffer */
        while (availablePlaces == 0)
            pthread_cond_wait(&condSO, &the_mutex);

    printf("Estudante_de_SO_%d entrou nao sala\n", i);
    availablePlaces--;
    pthread_mutex_unlock(&the_mutex);
}


// void assinar_lista_entrada();
// void aguardar_apresentacoes();
// void apresentar();
// void assinar_lista_saida();
