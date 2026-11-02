// librerias basicas a utilizar en el codigo

#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// definicion de macros para asignacion numerica a no clasificados y a
// mediciones definidas como ruido
#define UNCLASSIFIED (-2)
#define NOISE (-1)

// definicion de los structs base para trabajar

// struct para representar un punto
typedef struct point {
  double x;     // coordenada x del punto
  double y;     // coordenada y del punto
  int group;    // cluster o agrupacion asociada al punto
  bool visited; // valor booleano para indicar si fue visitado o no este punto
} point;

// almacen para el indice de los puntos
typedef struct index_list {
  size_t *data; // puntero a un bloque de memoria dinamica
  size_t size;  // el tamaño definido dados los puntos que se encuentren en la
               // index_list
  size_t capacity; // capacidad maxima de la index_list, no debe ser superada
                   // por size
} index_list;
