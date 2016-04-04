//
//  main.c
//  T4E2
//
//  Created by Abraham Esses on 4/4/16.
//  Copyright © 2016 Abraham Esses. All rights reserved.
//
//Suponga que una universidad quiere presumir de lo políticamente correcta que es, aplicando la doctrina de la Suprema Corte de EU “separados pero iguales implica desigualdad” no sólo en cuestiones de raza sino también de género, terminando su añeja práctica de tener sanitarios segregados por género en el campus. No obstante, como concesión a la tradición, se decreta que cuando una mujer está en un sanitario, pueden entrar otras mujeres, pero ningún hombre, y viceversa. Un letrero deslizante en la puerta de cada sanitario indica en cuál de los tres posibles estados se encuentra:

//oVacío
//
//oHay mujeres
//
//oHay hombres

//Utilizando como lenguaje de programación C, escriba los siguientes procedimientos: mujer_quiere_entrar, hombre_quiere_entrar, mujer_sale y hombre_sale. Puede usar los contadores y las técnicas de sincronización que desee.
//
//Demuestre el funcionamiento de la aplicación, mostrando en cada momento un mensaje con el estado del sanitario, quién quiere entrar, cuántos hombre y mujeres hay en espera, cuando entra un hombre, cuando entra una mujer, cuando sale un hombre y cuando sale una mujer. Por ejemplo, la salida debe ser algo como lo siguiente:

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#define MUJERES 1
#define HOMBRES 2
#define VACIO 0
#define nhilos 30
void * hombres(void *);
void * mujeres(void *);
sem_t bano;
sem_t espacios;
pthread_mutex_t mutex;
int status = VACIO;
int esperaH = 0;
int esperaM = 0;
void * hombres(void * p){
    int entro = 0;
    int valor_sem;
     esperaH++;
    while (!entro) {
       
        if(status == VACIO){
            printf("Sanitario vacío \n");
            sem_wait(&bano);
            pthread_mutex_lock(&mutex);
            status = HOMBRES;
            printf("G: Hombres\n");
            
            pthread_mutex_unlock(&mutex);
        }
        if (status==HOMBRES) {
            sem_getvalue(&espacios, &valor_sem);
            if(valor_sem != 0){
                sem_wait(&espacios);
                entro = 1;
                printf("El hombre con id %d está en el baño (%d hombres en espera) (%d mujeres en espera)\n",(int)p,esperaH,esperaM);
                sleep(3);
                pthread_mutex_lock(&mutex);
                esperaH--;
                pthread_mutex_unlock(&mutex);
                sem_post(&espacios);
                printf("El hombre con id %d sale del baño (%d hombres en espera) (%d mujeres en espera)\n",(int)p,esperaH,esperaM);
            }
            else{
                pthread_mutex_lock(&mutex);
                status = VACIO;
                printf("G: Vacío\n");
                pthread_mutex_unlock(&mutex);
                sem_post(&bano);
                
            }
        }
        else
        {
           printf("El hombre con id %d no pudo entrar al baño \n",(int)p);
        }
    }
    pthread_exit(NULL);
}
void * mujeres(void * p){
    int entro = 0;
    int valor_sem;
    esperaM++;
    while (!entro) {
        if(status == VACIO){
            printf("Sanitario vacío \n");
            sem_wait(&bano);
            pthread_mutex_lock(&mutex);
            status = MUJERES;
            printf("G: Mujeres\n");
            
            pthread_mutex_unlock(&mutex);
        }
        if (status==MUJERES) {
            sem_getvalue(&espacios, &valor_sem);
            if(valor_sem != 0){
                sem_wait(&espacios);
                entro = 1;
                printf("La mujer con id %d está en el baño (%d mujeres en espera) (%d hombres en espera) \n",(int)p,esperaM,esperaH);
                sleep(3);
                pthread_mutex_lock(&mutex);
                esperaM--;
                pthread_mutex_unlock(&mutex);
                sem_post(&espacios);
                printf("La mujer con id %d sale del baño (%d mujeres en espera) (%d hombres en espera)\n",(int)p,esperaM,esperaH);
            }
            else{
                pthread_mutex_lock(&mutex);
                status = VACIO;
                printf("G: Vacío\n");
                pthread_mutex_unlock(&mutex);
                sem_post(&bano);
                
            }
        }
        else
        {
            printf("La mujer con id %d no pudo entrar al baño \n",(int)p);
        }
    }
    pthread_exit(NULL);
}
int main(int argc, const char * argv[]) {
    sem_init(&bano, 0, 1);
    sem_init(&espacios,0,10);
    pthread_mutex_init(&mutex, NULL);
    pthread_t * threadsH = (pthread_t *)malloc(nhilos*sizeof(pthread_t));
    pthread_t * threadsM = (pthread_t *)malloc(nhilos *sizeof(pthread_t));
    for(int i = 0;i < nhilos; i++)
    {
        pthread_create(&threadsH[i], NULL, hombres, (void *)i);
        pthread_create(&threadsM[i], NULL, mujeres, (void *)i);
    }
    for (int k=0; k<nhilos; k++) {
        pthread_join(threadsM[k], NULL);
        pthread_join(threadsH[k], NULL);
    }
    sem_destroy(&bano);
    sem_destroy(&espacios);
    free(threadsH);
    free(threadsM);
    return 0;
}
