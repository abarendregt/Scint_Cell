#include <iostream>
#include <string>
using namespace std;

class Scintillator {
  string name;
  double xLength, yLength, yMesh, xVert, yVert, zVert;
  public:
  void setName(string N) {
	this->name=N;
  }
  void setXLength(double X) {
	this->xLength = X;
  }
  void setYLength(double Y) {
	this->yLength = Y;
  }
  void setYMesh(int msh) {
	this->yMesh = msh;
  }
  void setXVert(double Vx) {
	this->xVert = Vx;
  }
  void setYVert(double Vy) {
	this->yVert = Vy;
  }
  void setZVert(double Vz) {
	this->yVert = Vz;
  }
  string getName(string) {
	return this->name;
  }
  double getXLength(void) {
	return this->xLength;
  }
  double getYLength(void) {
	return this->yLength;
  }
  double getYMesh(void) {
	return this->yMesh;
  }
  double getXVert(void) {
	return this->xVert;
  }
  double getYVert(void) {
	return this->yVert;
  }
  double getZVert(void) {
	return this->zVert;
  }
};

int main () {
  Scintillator scint1;
  scint1.setXLength(3.72);
  scint1.setYLength(3.22);
  scint1.setYMesh(2);
  scint1.setXVert(0.0);
  scint1.setYVert(0.0);
  scint1.setZVert(0.0);
  scint1.getName("Scint1");
  scint1.getXLength(0.5);
  scint1.getYLength(0.5);
  scint1.getYMesh(0.5);
  scint1.getXVert(0.6);
  scint1.getYVert(0.8);
  scint1.getZVert(0.5);
  return 0;
}	

// Enumeration example
// http://www.cplusplus.com/doc/tutorial/other_data_types/#enum
