#include "RectangularParallelpiped.h"

int main () {
  /* 
    MeshRegion[] regions = scintelator.getMeshRegions();
    for (int i = 0; i < regions.length; i++) {
      regions[i]->toGeo();
      regions[i]->toGat();
      regions[i]->toLat();
    }
  */
  
  MeshRegion* m = new RectangularParallelpiped(1, 2, 1.1, 1.2, 2.1, 2.2, 3.0, 4.0);
  cout << m->toGeo();
  return 0;
}