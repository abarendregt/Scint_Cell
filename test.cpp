#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
using namespace std;

enum PT {null, Tri_Plain, Tri_Isect, RPP_Plain, RPP_Isect};
class MeshScintillator {
	double xMin, xMax, yMin, yMax;
	PT PartType; 
	public:
//This section defines each member of the class
	void setPT(PT pt) {this->PartType=pt;}
	void setxMin(double xMn) {this->xMin = xMn;}
	void setxMax(double xMx) {this->xMax = xMx;}	
	void setyMin(double yMn) {this->yMin = yMn;}
	void setyMax(double yMx) {this->yMax = yMx;}	
//This section calls each member of the class for use
	PT getPT(void) {return this->PartType;}
	double getxMin(void) {return this->xMin;}
	double getxMax(void) {return this->xMax;}
	double getyMin(void) {return this->yMin;}
	double getyMax(void) {return this->yMax;}
};
class Scintillator {
  string name;
  double xLength, yLength, yMesh, xVert, yVert, zVert;
  public:
  void setName(string N) {this->name=N;}
  void setXLength(double X) { this->xLength = X;}
  void setYLength(double Y) {this->yLength = Y;}
  void setYMesh(int msh) {this->yMesh = msh;}
  void setXVert(double Vx) {this->xVert = Vx;}
  void setYVert(double Vy) {this->yVert = Vy;}
  void setZVert(double Vz) {this->yVert = Vz;}
//This section is the variables used in defining the scintillator cell
  string getName(void) {return this->name;}
  double getXLength(void) {return this->xLength;}
  double getYLength(void) {return this->yLength;}
  int getYMesh(void) {return this->yMesh;}
  double getXVert(void) {return this->xVert;}
  double getYVert(void) {return this->yVert;}
  double getZVert(void) {return this->zVert;}
};

int main () {
  Scintillator scint1;
  scint1.setName("Scint1");
  scint1.setXLength(3.72);
  scint1.setYLength(3.22);
  scint1.setYMesh(2);
  scint1.setXVert(0.0);
  scint1.setYVert(0.0);
  scint1.setZVert(0.0);
  scint1.getName();
  scint1.getXLength();
  scint1.getYLength();
  scint1.getYMesh();
  scint1.getXVert();
  scint1.getYVert();
  scint1.getZVert();

//These are inputs from the Sphere Coordinates
	string Sph_name;
	double Sph_Rad=20.0;
   double Sph_VertX=0;
   double Sph_VertY=0; 
	double Sph_VertZ=-19.0;

// These are the variables that I need to define
	double Mesh_lenx= scint1.getXLength()/(2*scint1.getYMesh());
	double Mesh_leny= scint1.getYLength()/scint1.getYMesh();
	int Col = 2*scint1.getYMesh();
	int Row = scint1.getYMesh();

//Define Meshing array
	MeshScintillator S1Mesh[Row][Col];

//This portion of the code finds where the Radius intersects (if is does at all)
// Note: it is assumed that the Radial location of the sphere intersects below the
//       scintillator.  This assumption may have to be modified at a later time.


	int i, j, BlankNumb;
	// Define the rows
	for (i=0; i<Row;Row++){
		for (j=0;j<Col;j++) {
				S1Mesh[i][j].setPT(RPP_Isect);
				S1Mesh[i][j].getPT();
				cout<<i<<", "<<j<<", "<<S1Mesh[i][j].getPT()<<endl;
		}
	}
	return 0;
}

