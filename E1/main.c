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
int ingresarAcola(int );
int  quitarCola();
void imprimeCola(int *);
void cola_init(int *);
sem_t sillas;
pthread_mutex_t mutex;
int * cola_enanitos;
void * enanitos(void * p)
{
    int id = (int)p;
    printf("------Soy el enano con id %d\n",id);
    int j;
    //id = malloc(sizeof(int));
    //id = (int *)p;
    printf("E:El enanito %d esta en la mina\n",id );
    sleep(5);
    printf("E:El enanito %d llegó y quiere sentarse...\n",id);
    j=ingresarAcola(id);
    printf("El enanito %d esta en la pos %d\n",id,j);
    printf("E:El enanito %d fue atendido...\n",id);
    
    pthread_exit(NULL);
}
void * blancanieves(void * p){
    int valor_sem;
    int enano;
    
    enano = quitarCola();
    sem_getvalue(&sillas, &valor_sem);
    if(cola_enanitos[0] != -1){
        sem_wait(&sillas);
        printf("B: El enanito %d está comiendo\n",enano);
        sleep(10);
        printf("B: El enanito %d terminó de comer y regresó a la mina\n",enano); 
        
    }
    else
    {
        printf("B: No hay enanitos en cola :( \nB:Salí a dar un paseo, regreso en 5 seg\n");
        sleep(5);
    }
    
    //printf("El enano que se esta atendiendo es %d\n",enano);
    /*while(1){
            }*/
    pthread_exit(NULL);
}
void imprimeCola(int * a){
    for (int i = 0; i < N; i++) {
        printf("Enanito en cola [%d]: %d\n",i,a[i]);
    }
}
int ingresarAcola(int id){
    int i;
    int pos=0;
    pthread_mutex_lock(&mutex);
    for(i = 0; i < N; i++ ){
        if(cola_enanitos[i] == -1)
        {
            cola_enanitos[i] = id;
            pos = i;
            break;
        }
    }
    pthread_mutex_unlock(&mutex);
    
    return pos;
}
void cola_init(int * cola){
    for (int i = 0; i < N; i++) {
        cola[i]=-1;
    }
}
int quitarCola(){
    int * colaTemp = (int *)malloc(sizeof(int)*N);
    int i;
    int id;
    pthread_mutex_lock(&mutex);
    id = cola_enanitos[0];
    for(i = 0;i < N;i++){
        if(i!=(N-1))
            colaTemp[i]=cola_enanitos[i+1];
        else
            colaTemp[N-1] = -1;
    }
    //Copiando temporal en cola_enanitos
    
    for (int j = 0; j < N; j++) {
        cola_enanitos[i] = colaTemp[i];
    }
    pthread_mutex_unlock(&mutex);
    free(colaTemp);
    return id;
}
int main (int argc, char* argv[])
{
    pthread_t * enanitos_array = (pthread_t *)malloc(sizeof(pthread_t) * N);
    pthread_t blancanieves_t;
    cola_enanitos = (int *) malloc(sizeof(int) * N);
    cola_init(cola_enanitos);
    int in;
    //int *id_en;
    sem_init(&sillas,0,4);
    pthread_create(&blancanieves_t,NULL,blancanieves,NULL);
    
    for(in = 0; in < N;++in){
        //id_en = (int *)malloc(sizeof(int));
        //id_en = &in;
        //printf("Esta es la i asignada a los enanos %d\n",*id_en);
        pthread_create(&enanitos_array[in], NULL, enanitos, (void *)in);
        
        //imprimeCola(cola_enanitos);
        //printf("Se ingreso el enanito a la cola en la posición %d\n",j);
        //imprimeCola(cola_enanitos);
    }
    for (int k = 0; k < N; k++) {
        pthread_join(enanitos_array[k], NULL);
    }
    pthread_join(blancanieves_t, NULL);
    //free(cola_enanitos);
    sem_destroy(&sillas);
    free(enanitos_array);
    //free(id_en);
    return 0;
}



