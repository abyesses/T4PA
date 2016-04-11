//
//  main.c
//  T4E4
//
//  Created by Abraham Esses on 4/4/16.
//  Copyright © 2016 Abraham Esses. All rights reserved.
//
//Un sistema distribuido de supervisión y registro de datos se compone de varias unidades de adquisición de datos (UADs) y una unidad central de registro, como se indica en la siguiente figura:
//
//En cada unidad de adquisición de datos se realiza continuamente un bucle de exploración de un sensor y se anota el valor actual de esa medida. Cuando se detecten ciertos cambios críticos se enviará un registro de alarma a la unidad de registro.
//
//Las unidades de adquisición de datos atenderán también a órdenes periódicas de muestreo desde la unidad de registro, a las que responderán con los valores actuales anotados de las medidas.
//
//Se asume que las unidades de adquisición de datos pueden tomar las medidas a una cadencia mucho más rápida que las solicitudes de muestreo, es decir, pueden repetir varias veces la lectura de los sensores entre dos órdenes de muestreo sucesivas.
//
//La unidad de registro recibirá las alarmas enviadas espontáneamente por las unidades de adquisición de datos, y las almacenará en disco. Periódicamente enviará órdenes de muestreo a esas mismas unidades de adquisición y almacenará en disco las colecciones de medidas recibidas. Solo se dispone de un disco, por lo que las operaciones de registro en él solo se podrán hacer de una en una, sean alarmas o medidas originadas por los muestreos. El registro de las alarmas debe tener prioridad sobre el de las medidas. Si el registro de medidas se retrasa, se puede suprimir el registro de las más antiguas. Por ejemplo, la nueva lectura de un sensor reemplaza a la antigua, si no había sido grabada todavía (ej. solo se guarda una medida por unidad de adquisición).
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#define num_sensores 10
#define YES 1
#define  NO 0
pthread_mutex_t askForReadings = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t writing_readings = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t alarma = PTHREAD_COND_INITIALIZER;
int ** lecturas;
int goAndRead = 1;
void * uad_action(void *);
void * ur_action(void *);
void cleanArray(int *);
void readings_init(int **);
void printArraySent(int *,int);
void printArraySent2(int **);
void canRead();
void * uad_action(void * p){
    int id = (int)p;
    int lectura_x = 0;
    int acum = 0;
    int indice = 0;
    int * l = (int *)malloc(sizeof(int)*100);
    cleanArray(l);
    while(1){
        while (goAndRead == YES && indice < 99) {
            printf("UID[%d]: Collecting data...\n",id);
            lectura_x = rand()%100;
            l[indice] = lectura_x;
            acum += lectura_x;
            indice++;
            if (acum - lectura_x < -15 ) {
                printf("UID[%d]: Sending critical info...\n",id);
                pthread_cond_signal(&alarma);
            }
            sleep(10);//Evita muchas lecturas
            
        }
        if(goAndRead == NO){
            //printf("UID[%d]: Writing into disk readings...\n",id);
            pthread_mutex_lock(&writing_readings);
            for (int j = indice-1; j >= 0 ; j--) {
                lecturas[id][j]=l[j];
            }
            pthread_mutex_unlock(&writing_readings);
        }
        if (indice == 100) {
            indice = 0;
            cleanArray(l);
        }
    }
    free(l);
    pthread_exit(NULL);
}
void * ur_action(void * p){
    int a = 0;
    while (1) {
        a = (rand() % (1+1-0))+0;
        printf("a: %d\n",a);
        if(pthread_mutex_trylock(&askForReadings) == 0){
            pthread_mutex_unlock(&askForReadings);
            printf("Solicitando Lecturas aleatoriamente\n");
            canRead();
           
        }
        else{
        //pthread_cond_wait(&alarma, &askForReadings);
        printf("Se recibió alarma\n");
        canRead();
        pthread_mutex_unlock(&askForReadings);
        }
    }
}
void canRead(){
    pthread_mutex_lock(&m);
    printf("Solicitando lecturas a unidades...\n");
    goAndRead = NO;
    pthread_mutex_unlock(&m);
    sleep(5);
    pthread_mutex_lock(&m);
   // printf("Generando lecturas...\n");
    printArraySent2(lecturas);
    goAndRead = YES;
    pthread_mutex_unlock(&m);
}
void cleanArray(int * array){
    for (int i = 0; i < 100; i++) {
        array[i] = 0;
    }
}
void printArraySent(int * array,int id){
    for (int i = 0; i < 100; i++) {
        if (array[i] != 0)
        printf("UAD[%d,%d]: %d\n",id,i,array[i]);
    }
}
void printArraySent2(int ** array){
    printf("Imprimiendo lecturas\n");
    for (int i = 0; i < num_sensores; i++) {
        for (int j = 0; j < 100; j++) {
            if(lecturas[i][j] != 0){
                printf("Lectura[%d,%d]:  %d,",i,j,lecturas[i][j]);
            }
        }
        printf("\n");
    }
}
void readings_init(int ** array){
    for (int i = 0; i < num_sensores ; i++) {
        lecturas[i] = (int *)malloc(sizeof(int)*100);
    }
}
int main(int argc, const char * argv[]) {
    pthread_t * uads = (pthread_t *)malloc(sizeof(pthread_t)*num_sensores);
    pthread_t ud;
    lecturas = (int **)malloc(sizeof(int*) * num_sensores);
    readings_init(lecturas);
    for (int i = 0; i < num_sensores; ++i) {
        pthread_create(&uads[i], NULL, uad_action,(void *)i);
    }
    pthread_create(&ud, NULL, ur_action, NULL);
    for (int j = 0; j < num_sensores; ++j){
        pthread_join(uads[j], NULL);
    }
    pthread_join(ud, NULL);
    free(lecturas);
    free(uads);
    return 0;
}
