#include "RectangularParallelpiped.h"

int main () {
  RectangularParallelpiped* rect = new RectangularParallelpiped(1, 2, 1.1, 1.2, 2.1, 2.2, 3.0, 4.0);
  cout << rect->toGeo();
  return 0;
}