//
//  main.c
//  T4E3
//
//  Created by Abraham Esses on 4/4/16.
//  Copyright © 2016 Abraham Esses. All rights reserved.
//
//En el Distrito Federal está a punto de inaugurarse el primer centro comercial en el que la compra la realizan robots en lugar de personas. El centro comercial está compuesto por N (un número fijo) secciones adyacentes, donde por problemas estructurales, cada sección solo puede soportar un peso máximo (Pi). Los compradores envían sus listas de compra a un sistema, y cada lista se asigna a un robot diferente. Cuando un robot recibe su asignación, inicia el recorrido entrando en la sección 0, avanzan a la sección adyacente (0, 1, . . .) cuando han acumulado los productos requeridos de esa sección y terminan saliendo de la sección N − 1.
//
//Debido al peso de los robots y las limitantes estructurales de centro comercial, cuando un robot con un determinado peso (pi) quiere avanzar a la siguiente sección, debe esperar hasta que el incremento de peso que provoca no ponga en peligro la estructura (si el peso actual acumulado de los robots que se encuentran en la sección adyacente es peso entonces un robot con peso pi no puede avanzar si peso + pi > Pi).
//
//Tenga en cuenta que debido a la demanda existente en el centro comercial y la poca disponibilidad de robots con que se cuenta inicialmente, se desea que si un robot  se encuentra esperando para entrar en una sección (debido a restricción de sobrepeso), en cuanto se libere peso en dicha sección se le notifique al robot en espera para que intente avanzar.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define num_secciones 10
#define nrobots 20
#define PI 3.14159265

typedef struct{
    pthread_mutex_t seccion;
    float peso;
}seccion;
seccion *s;
void * robot (void *);
void * robot (void * p){
    int id = (int)p;
    int entro = 0;
    int i = 0;
    //printf("Soy el hilo con el id %d\n",id);
        while (entro == 0 || i < num_secciones) {
            if (s[i].peso == 0) {
                pthread_mutex_lock(&s[i].seccion);
                s[i].peso = PI;
                entro = 1;
                printf("Hilo [%d]: Comprando en sección %d\n",id+1,i+1);
                sleep(3);
                printf("Hilo [%d]: Saliendo de sección %d\n",id+1,i+1);
                pthread_mutex_unlock(&s[i].seccion);
                s[i].peso = 0;
                i++;
            }
            else
            {
                entro = 0;
            }
        }
    pthread_exit(NULL);
}
int main(int argc, const char * argv[]) {
    s = (seccion *)malloc(sizeof(seccion)*num_secciones);
    pthread_t * robots = (pthread_t *)malloc(sizeof(pthread_t)*nrobots);
    for (int j = 0; j < num_secciones; j++) {
        s[j].peso = 0;
        pthread_mutex_init(&s[j].seccion, NULL);
    }
    for (int i = 0; i <nrobots; ++i) {
        pthread_create(&robots[i], NULL, robot,(void *)i);
    }
    for (int k = 0; k <nrobots; ++k) {
        pthread_join(robots[k], NULL);
    }
    free(s);
    free(robots);
    return 0;
}
