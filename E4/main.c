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

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    return 0;
}
