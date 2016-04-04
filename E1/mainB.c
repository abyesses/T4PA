//
//  main.c
//  T4E1
//
//  Created by Abraham Esses on 3/30/16.
//  Copyright © 2016 Abraham Esses. All rights reserved.
//
//----------------------------------------------Instrucciones-------------------------
// Dado el siguiente problema:

// Blancanieves y los siete enanitos viven en una casa donde solo existen cuatro sillas, que los enanitos utilizan para comer.
// Cuando un enanito vuelve de trabajar en la mina comprueba si hay una silla libre para sentarse.
//Si existe una silla libre, entonces indica a Blancanieves que ya está sentado, y espera pacientemente su turno a que le sirvan.
// Cuando le ha servido, Blancanieves le indica que puede empezar a comer.
// El enanito come y cuando acaba, deja la silla libre y vuelve a la mina.
//Por su parte, Blancanieves cuando no tiene ningún enanito pendiente de servirle, se va a dar una vuelta.



// a)Realice un análisis exhaustivo de los problemas de concurrencia que se pueden dar en un sistema de este tipo, planteando cada uno de ellos y explicándolo.
//R:--Los problemas que tiene el ejercicio es exclusión  mutua, carrera para reservar los recursos

// b)Seleccione una técnica de concurrencia para solucionar el problema, explicando los motivos de su selección.
//R:-- Se utilizan semáforos y mutex para solucionar la exclusión mutua y la escritura al recurso
// c)Partiendo del análisis realizado y la técnica de concurrencia seleccionada, programe una solución que resuelva el problema descrito.
//Utilizar únicamente dos tipos de procesos: Enanito y Blancanieves.



#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#define N 7
void * enanitos(void *);
void * blancanieves(void *);
sem_t sillas;
pthread_mutex_t comer =  PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t comiendo =  PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t puede_comer = PTHREAD_COND_INITIALIZER;
void * enanitos(void * p){
    int id = (int)p;
    printf("El enanito %d está en la mina\n",id+1);
    sleep(5);
    sem_wait(&sillas);
    printf("El enanito %d esta sentado, esperando a Blancanieves...\n",id+1);
    pthread_cond_wait(&puede_comer,&comiendo);
    printf("El enanito %d esta comiendo\n",id+1);
    sleep(10);
    sem_post(&sillas);
    pthread_exit(NULL);
}
void * blancanieves(void * p){
    int valor_sem;
    while (1) {
        sem_getvalue(&sillas, &valor_sem);
        printf("Valor de semáforo %d\n",valor_sem);
        if (valor_sem < 4) {
            printf("Atendiendo enanos...\n");
            pthread_cond_signal(&puede_comer);
        }
        else
        {
            pthread_mutex_lock(&comiendo);
            printf("Estoy de paseo por 5 segundos...\n");
            sleep(15);
            pthread_mutex_unlock(&comiendo);
        }
    }
    pthread_exit(NULL);
}
int main (int argc, char* argv[])
{
    sem_init(&sillas, 0, 4);
    pthread_t * enanos = (pthread_t *)malloc(sizeof(pthread_t)*N);
    
    pthread_t blanca;
    pthread_create(&blanca,NULL, blancanieves, (void *)1);
    
    for (int i = 0; i < N ; i++) {
        pthread_create(&enanos[i], NULL, enanitos, (void *)i);
    }
    for (int j = 0; j < N; j++) {
        pthread_join(enanos[j], NULL);
    }
    sem_destroy(&sillas);
    pthread_mutex_destroy(&comer);
    pthread_cond_destroy(&puede_comer);
    free(enanos);
    return 0;
}



