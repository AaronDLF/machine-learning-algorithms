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

// funcion para la liberacion de memoria e inicializacion en ceros

static void list_free(index_list *list) {
  free(list->data);
  list->data =
      NULL; // aqui no es cero porque toca recordar que es un puntero a
            // size_t, no es el valor directamente, colocar cero produce el
            // mismo resultado pero no declara las intenciones directas
  list->size =
      0; // aqui si hablo de valor directamente, por tanto puedo colocar el
         // valor en ceros, accedo por -> ya que es un puntero al struct de
         // index_list y luego se accede al atributo concreto size
  list->capacity = 0;
}

static bool push(index_list *list, size_t index) {
  if (list->size == list->capacity) { // caso para evaluar que el tamaño de lo
                                      // almacenado en la lista coincida con la
                                      // capcidad designada para la misma
    const size_t limit = SIZE_MAX / sizeof(*list->data);
    size_t next; // siguiente tamaño a asignar de limite

    // se evaluan las capacidades futuras (next) a partir de las capacidades
    // actuales
    if (list->capacity == 0) {
      next = 32;
    } else {
      if (list->capacity > limit / 2) {
        next = limit;

      } else {
        next = list->capacity * 2;
      }
    }

    if (next <= list->capacity || next > limit) {
      return false;
    }

    /*TODO : 1. realocacion de memoria y validacion de que la nueva data no sea
     * NULL
     * 2.asignacion de la nueva data y de la siguiente capacidad a evaluar
     **/

    size_t *new_data = realloc(list->data, next * sizeof(*list->data));

    if (new_data == NULL) {
      return false;
    }

    list->data = new_data;
    list->capacity = next;
  }

  /* NOTE :
   *
   * El bloque de memoria dinámico se reserva mediante realloc(),
   * que también puede realizar una reserva inicial cuando data == NULL.
   *
   * El campo data del struct index_list es un único puntero a size_t,
   * que apunta al primer elemento de un arreglo dinámico.
   *
   * La memoria reservada permite almacenar 'capacity' elementos de
   * tipo size_t, no 'capacity' punteros a size_t. */

  /* asignacion del elemento pasado a la funcion para hacer push en la siguiente
  posicion libre del arreglo de tamaño dinamico */
  list->data[list->size] = index;

  // actualizacion de la cantidad de elementos del arreglo
  list->size++;

  // retorno positivo de la ejecucion del push en la funcion
  return true;
}
