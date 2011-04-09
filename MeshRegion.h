#include <string>

using namespace std;

class MeshRegion {
  public:
  // virtual ~MeshRegion();
  virtual string toGeo (void) =0;
};