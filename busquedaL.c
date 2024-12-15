/*
Flores Anzurez Marco Antonio 
4CV1 

Arreglo de potencias
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <math.h>

// Variable global para controlar el turno de los hilos
int turno = 1;

// Semáforos para sincronizar los hilos
sem_t mutex;
sem_t semf;

// Estructura para almacenar los parámetros necesarios para la función 'elevarPotencia'
struct cosas {
    int x;
    int tamano;
    int* arr;
};

// Función que será ejecutada por cada hilo
void *elevarPotencia(void *arg) {
    // Convertir el argumento a la estructura 'cosas'
    struct cosas *todojunto = (struct cosas *)arg;
    int x = todojunto->x;
    int *arr = todojunto->arr;
    int tamano = todojunto->tamano;
    int mitad = tamano / 2;
    int i = 0;

    // Iterar a través del arreglo de potencias
    while (i < tamano) {
        // Si es el turno del primer hilo
        if (turno == 1) {
            sem_wait(&mutex);
            printf("Índice: %d, x = %d, potencia = %d, resultado = %.2lf\n", i, x, arr[i], pow(x, arr[i]));
            i++;
            turno = 0;
            sem_post(&semf);
        } 
        // Si es el turno del segundo hilo
        else if (turno == 0 && i + mitad < tamano) {
            sem_wait(&semf);
            printf("Índice: %d, x = %d, potencia = %d, resultado = %.2lf\n", i + mitad, x, arr[i + mitad], pow(x, arr[i + mitad]));
            i++;
            turno = 1;
            sem_post(&mutex);
        }
    }

    return NULL;
}

int main() {
    int x, tamano;

    // Inicializar semáforos
    sem_init(&mutex, 0, 1);
    sem_init(&semf, 0, 0);

    // Leer el número 'x' a elevar y el tamaño del arreglo
    printf("Escribe el número x a elevar:\n");
    scanf("%d", &x);

    printf("Escribe el tamaño del arreglo:\n");
    scanf("%d", &tamano);

    // Crear e inicializar la estructura 'params'
    struct cosas params;
    int *arr = malloc(tamano * sizeof(int));

    for (int i = 0; i < tamano; i++) {
        arr[i] = rand() % 3 + 2; // Genera números aleatorios entre 2 y 4
    }

    // Mostrar el arreglo generado
    printf("Arreglo generado: [");
    for (int i = 0; i < tamano; i++) {
        printf("%d", arr[i]);
        if (i < tamano - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    params.x = x;
    params.tamano = tamano;
    params.arr = arr;

    // Crear dos hilos y ejecutar la función 'elevarPotencia'

    pthread_t hilo1, hilo2;
    pthread_create(&hilo1, NULL, elevarPotencia, &params);
    pthread_create(&hilo2, NULL, elevarPotencia, &params);

    // Esperar a que los hilos terminen
    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);

    // Liberar memoria y destruir semáforos
    free(arr);
    sem_destroy(&mutex);
    sem_destroy(&semf);

    return 0;
}

