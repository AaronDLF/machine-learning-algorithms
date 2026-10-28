// Librerias basicas a utilizar

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
    for (size_t j = 0; j < size; j++) {
      points[j].group = rand() % k;
    }
    // definicion de variables de cambio y error minimo a aceptar
    size_t changed = 0;
    size_t accepted_error = size / 10000;
    int group = 0;
    do {
      // reinicio de changed a cero, debe contar cambios para cada pasada
      changed = 0;
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
         calculado*/

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
    clusters = malloc(sizeof(cluster) * size);
    for (size_t j = 0; j < size; j++) {
      /*la siguiente linea no tiene alta legibilidad pero se hizo para poder
       * entender que es la forma final luego de que el compilador haga la
       * traduccion del azucar sintactico
        forma final del compilador : (*(clusters + j)).x = (*(points + j)).x;*/

      clusters[j].x = points[j].x;
      clusters[j].y = points[j].y;
      clusters[j].count = 1;
      points[j].group = j;
    }
  }
  return clusters;
}

/*la funcion de printEPS() no fue desarrollada por mi persona, es prestamo
 * de un codigo de referencia, a pesar de esto,  pude aprender logicas
 * interesantes asociadas a la alocacion de memoria para el RGB, asignando los 3
 * canales como un arreglo que contempla 3 espacios(R, G, B) para cada canal de
 * color a cada cluster
 * */

void printEPS(point pts[], size_t len, cluster cent[], int k) {
  int W = 400, H = 400;
  double min_x = DBL_MAX, max_x = DBL_MIN, min_y = DBL_MAX, max_y = DBL_MIN;
  double scale = 0, cx = 0, cy = 0;
  double *colors = (double *)malloc(sizeof(double) * (k * 3));
  int i;
  size_t j;
  double kd = k * 1.0;
  for (i = 0; i < k; i++) {
    *(colors + 3 * i) = (3 * (i + 1) % k) / kd;
    *(colors + 3 * i + 1) = (7 * i % k) / kd;
    *(colors + 3 * i + 2) = (9 * i % k) / kd;
  }

  for (j = 0; j < len; j++) {
    if (max_x < pts[j].x) {
      max_x = pts[j].x;
    }
    if (min_x > pts[j].x) {
      min_x = pts[j].x;
    }
    if (max_y < pts[j].y) {
      max_y = pts[j].y;
    }
    if (min_y > pts[j].y) {
      min_y = pts[j].y;
    }
  }
  scale = W / (max_x - min_x);
  if (scale > (H / (max_y - min_y))) {
    scale = H / (max_y - min_y);
  };
  cx = (max_x + min_x) / 2;
  cy = (max_y + min_y) / 2;

  printf("%%!PS-Adobe-3.0 EPSF-3.0\n%%%%BoundingBox: -5 -5 %d %d\n", W + 10,
         H + 10);
  printf("/l {rlineto} def /m {rmoveto} def\n"
         "/c { .25 sub exch .25 sub exch .5 0 360 arc fill } def\n"
         "/s { moveto -2 0 m 2 2 l 2 -2 l -2 -2 l closepath "
         "	gsave 1 setgray fill grestore gsave 3 setlinewidth"
         " 1 setgray stroke grestore 0 setgray stroke }def\n");
  for (int i = 0; i < k; i++) {
    printf("%g %g %g setrgbcolor\n", *(colors + 3 * i), *(colors + 3 * i + 1),
           *(colors + 3 * i + 2));
    for (j = 0; j < len; j++) {
      if (pts[j].group != i) {
        continue;
      }
      printf("%.3f %.3f c\n", (pts[j].x - cx) * scale + W / 2,
             (pts[j].y - cy) * scale + H / 2);
    }
    printf("\n0 setgray %g %g s\n", (cent[i].x - cx) * scale + W / 2,
           (cent[i].y - cy) * scale + H / 2);
  }
  printf("\n%%%%EOF");

  // free accquired memory
  free(colors);
}

static void test() {
  // se hace la alocacion en memoria de las observaciones (puntos)
  size_t size = 100000L;
  point *points = malloc(sizeof(point) * size);

  // definicion de un maximo radio a 20 y de las variables de radio y angulo
  double maxRadius = 20;
  double radius = 0;
  double angle = 0;
  // iterar para definir los angulos y los radios de cada uno de los puntos

  size_t i = 0;
  for (; i < size; i++) {
    // acotado al rango disponible conversion de las coordenadas polares a
    // coordendas geometricas
    radius = maxRadius * ((double)rand() / RAND_MAX);
    angle = 2 * M_PI * ((double)rand() / RAND_MAX);

    (points + i)->x = radius * cos(angle);
    (points + i)->y = radius * sin(angle);
  }

  // aplicar la funcion de k means sobre el conjunto de puntos
  int k = 5;
  cluster *clusters = kmeans(points, size, k);
  // aplicar la funcion de impresion del formato de EPS
  printEPS(points, size, clusters, k);

  // liberacion de la memoria
  free(points);
  free(clusters);
}

static void test2() {
  size_t size = 100000L;
  point *points = malloc(sizeof(point) * size);

  double maxRadius = 20;
  double radius = 0;
  double angle = 0;

  size_t i = 0;

  for (; i < size; i++) {
    radius = maxRadius * ((double)rand() / RAND_MAX);
    angle = 2 * M_PI * ((double)rand() / RAND_MAX);

    points[i].x = radius * cos(angle);
    points[i].y = radius * sin(angle);
  }

  int k = 15;
  cluster *clusters = kmeans(points, size, k);
  printEPS(points, size, clusters, k);
  // liberando la memoria, casi lo olvido ...
  free(points);
  free(clusters);
}

int main(int argc, char *argv[]) {
  srand(time(NULL));
  test();

  return 0;
}
