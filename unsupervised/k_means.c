// Liberias basicas a utilizar

#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// definicion de las estructuras a utilizar para el codigo

/* estructura para el elemento del punto concreto, sus coordenadas y su grupo
asignado */
typedef struct point {
  double x;  // coordenada x del punto
  double y;  // coordenada y del punto
  int group; // grupo al que pertenece el punto
} point;

// estructura para definir el cluster
typedef struct cluster {
  double x;     // coordenada x del centroide
  double y;     // coordenada y del centroide
  size_t count; // cantidad de elementos que contiene este cluster
} cluster;

/* funcion para calcular el centroide mas cercano al punto de observacion en
 * cuestion */
int calculate_nearest(point *p, cluster clusters[], int k) {
  double minD = DBL_MAX;
  double dist = 0;
  int index = -1;
  int i = 0;

  for (; i < k; i++) {

    dist =
        (clusters[i].x - p->x) * (clusters[i].x - p->x) +
        (clusters[i].y - p->y) *
            (clusters[i].y - p->y); // distancia cuadrada del punto al centroide

    if (dist < minD) {
      minD = dist;
      index = i;
    }
  }
  return index;
}

void calculate_centroid(point points[], cluster *c, size_t size) {
  if (size == 0) { /* validacion para evitar que se produzca el caso de error
                      debido a la division por cero */
    c->count = 0;
    return;
  }

  size_t i = 0; /* en este caso se usarea el size_t por encima de int para
                   evitar un overflow y posteriormente un segmentation fault */
  double sum_x = 0;
  double sum_y = 0;
  c->count = size;

  for (; i < size; i++) {
    sum_x += points[i].x;
    sum_y += points[i].y;
  }

  c->x = sum_x / size;
  c->y = sum_y / size;
}

cluster *kmeans(point points[], size_t size, int k) {

  cluster *clusters = NULL; // inicializacion del puntero en NULL

  // evaluacion de los casos de las cantidades de clusters posibles

  // primero un solo cluster
  if (k <= 1) {
    clusters =
        malloc(sizeof(cluster)); // cambio del valor NULL incicial del cluster a
                                 // un size en memoria del tamaño de un cluster
    memset(clusters, 0, sizeof(cluster)); // colocar a ceros la memoria
    calculate_centroid(points, clusters,
                       size); // calculo del centroide para un solo cluster
  }
  // caso para una cantidad de clusters mayor que 1
  else if (k < size) {
    clusters = malloc(sizeof(cluster) * k);
    memset(clusters, 0, sizeof(cluster) * k);

    // asignacion del grupo a una observacion o punto j
    for (size_t j; j < size; j++) {
      points[j].group = rand() % k;
    }
    // definicion de variables de cambio y error minimo a aceptar
    size_t changed = 0;
    size_t accepted_error = size / 10000;
    int group = 0;
    do {
      // inicio de los clusters a cero
      for (int i = 0; i < k; i++) {
        clusters[i].x = 0;
        clusters[i].y = 0;
        clusters[i].count = 0;
      }
      // sumatoria de los atributos del struct
      for (size_t j = 0; j < size; j++) {

        group = points[j].group;
        clusters[group].x += points[j].x;
        clusters[group].y += points[j].y;
        clusters[group].count++;
      }

      // division de la sumatoria de las componentes x de los puntos del cluster
      // entre su tamaño
      for (int i = 0; i < k; i++) {
        clusters[i].x /= clusters[i].count;
        clusters[i].y /= clusters[i].count;
      }

      /* TODO : iterar sobre los puntos, calcular los centroides mas cercanos a
         los puntos y verificar si el cercano calculado es diferente al grupo
         evaluado para la iteracion,actualizar la variable de cambios "changed"
         y reasignar el grupo del punto/observacion evaluado al valor cercano
         calculado */

      for (size_t j = 0; j < size; j++) {

        int c = calculate_nearest(points + j, clusters, k);
        if (c != points[j].group) {
          changed++;
          (points + j)->group = c;
        }
      }

    } while (changed > accepted_error);
    // evaluar la condicion cuando se presente entonces el caso de que el
    // cluster solo comprenda a cada elemento individualmente
  } else {
    for (size_t j = 0; j < size; j++) {
      /*la siguiente linea no tiene alta legibilidad pero se hizo para poder
       * entender que es la forma final luego de que el compilador haga la
       * traduccion del azucar sintactico*/
      (*(clusters + j)).x = (*(points + j)).x;
    }
  }
  return clusters;
}

int main(int argc, char *argv[]) {
  printf("hola a todos ustedes");
  return EXIT_SUCCESS;
}
