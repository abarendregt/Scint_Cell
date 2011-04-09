
#ifndef SCINTILLATOR_H
#define SCINTILLATOR_H
using namespace std;
#include <string>
#include <vector>
class Scintillator {
	string name;
	int yMesh, zmesh;
	double xLength, yLength, zLength, xVert, yVert, zVert;
	
	vector<vector<MeshRegion > > meshRegions;
	
	Sphereisect sphere1;

	public:
	Scintillator();
	Scintillator(string name, double xLength, double yLength, double zLength, int yMesh, int zMesh, double xVert, double yVert, double zVert);
	void setName(string N) {this->name=N;}
	void setXLength(double X) { this->xLength = X;}
	void setYLength(double Y) {this->yLength = Y;}
	void setYMesh(int msh) {this->yMesh = msh;}
	void setZLength(double Z) {this->zLength = Z;}
	void setZMesh(int zmsh) {this->zmesh = zmsh;}
	void setXVert(double Vx) {this->xVert = Vx;}
	void setYVert(double Vy) {this->yVert = Vy;}
	void setZVert(double Vz) {this->zVert = Vz;}
	//This section is the variables used in defining the scintillator cell
	string getName(void) {return this->name;}
	double getXLength(void) {return this->xLength;}
	double getYLength(void) {return this->yLength;}
	int getYMesh(void) {return this->yMesh;}
	double getZLength(void) {return this->zLength;}
	int getZMesh(void) {return this->zmesh;}
	double getXVert(void) {return this->xVert;}
	double getYVert(void) {return this->yVert;}
	double getZVert(void) {return this->zVert;}
};

Scintillator::Scintillator() {
	this->setName("Scint1");
	this->setXLength(3.72);
	this->setYLength(3.22);
	this->setYMesh(4);  //This number must be even (#!# add test later)
	this->setZLength(0.3);
	this->setZMesh(8);
	this->setXVert(0.0);
	this->setYVert(0.0);
	this->setZVert(0.0);
}

Scintillator::Scintillator(string name, double xLength, double yLength, double zLength, int yMesh, int zMesh, double xVert, double yVert, double zVert) {
	this->setName(name);
	this->setXLength(xLength);
	this->setYLength(yLength);
	this->setZLength(zLength);

	this->setYMesh(yMesh);
	this->setZMesh(zMesh);

	this->setXVert(xVert);
	this->setYVert(yVert);
	this->setZVert(zVert);
}

Scintillator::getMeshRegions() {
    // Analysis variables to define
  	int maxColSize = getMaxColumnSize();
  	int maxRowSize = getMaxRowSize();
  	int i, j;

    //Define Meshing 2Darray

  	meshRegions.resize(maxRowSize);
	for (i = 0; i<maxRowSize;++i)
  		meshRegions[i].resize(maxColSize);
  
    //Define xmin, xmax, ymin, ymax
	setMeshBoundary(maxRowSize, maxColSize);
    classifyMeshRegion();
}

Scintillator::getMaxColumnSize() {
  return 2 * yMesh;
}

Scintillator::getMaxRowSize() {
  return yMesh;
}

void setMeshBoundary(int maxRowSize, int maxColSize) {
	double yMeshLth=yLength/yMesh;
	double xMeshLth=xLength/(2*yMesh);
	double xmin, xmax, ymin, ymax;

	//define xmin, xmax, ymin and ymax based on starting location (0,0)
	// Define the rows
	for (int i=0;i<maxRowSize;++i){	 
		ymax= yVert + yLength/2 -i*yMeshLth;
		ymin=ymax-yMeshLth;

		if (i==maxRowSize-1) {  //this is in place on to eliminate any rounding errors
			ymin = yVert - yLength/2;
		}

		for (int j=0;j<maxColSize;++j) {
			xmin= xVert-s.xLength/2+j*xMeshLth;
			xmax=xmin+xMeshLth;

			if (j==maxColSize-1) {  //this is in place on to eliminate any rounding errors
			xmax = xVert + xLength/2;
			}
			
            RectangularParallelpiped* r = new RectangularParallelpiped(i, j, xmin, xmax, ymin, ymax);

			meshRegions[i][j] = r;
		}
	}
}

Scintillator::classifyMeshRegion() {
    int maxRowSize = getMaxRowSize();
    int maxColSize = getMaxColumnSize();
	
    //Assign proper part type to MeshScintillator
	/************************************************************************** 
	 This function assumes the sphere has the same 'x' and 'y' vertex as the scintillator. 
	 If this is not the case, then this test may fail.  Consider adding more tests at a later time.
	
	 Also assumed is that the sphere vertex is lower than the scintillator.
	****************************************************************************/ 
	int nullCellMax = yMesh/2-1;  //This is the maximum number of null cells
	for (i=0;i<maxRowSize;++i){	
		for (j=0;j<maxColSize;++j) {
			if (j+1 <= abs(nullCellMax) ||  j > maxColSize-abs(nullCellMax)-1) {
				meshRegions[i][j] = NULL;  
			} else if (j+1 == abs(nullCellMax)+1 ||  j == maxColSize-abs(nullCellMax)-1) {
			    if (isIntersected(i,j)) {
			        // TODO: Set xmin, xmax, ymin, ymax, etc..
                    meshRegions[i][j] = new IntersectedRectangularParallelpiped();
		        }
			} else { 
			    if (isIntersected(i,j)) {
    		        // TODO: Set xmin, xmax, ymin, ymax, etc..
					meshRegions[i][j] = new IntersectedTrianguloid();
				}
			}
		}
		nullCellMax--;
		if (i==maxRowSize/2-1) {
			nullCellMax=0;
		}
	}
}

void isIntersected(int i, int j){
	if (sphere1.getRadius() > sqrt( pow(S1Mesh[i][j].getxMin()-sphere1.getXVert(),2) + pow(S1Mesh[i][j].getyMin()  \
			-sphere1.getYVert(),2) + pow(scint1.getZVert()-sphere1.getZVert(),2) ) ) {
		return true;
	} else if (sphere1.getRadius() > sqrt( pow(S1Mesh[i][j].getxMax()-sphere1.getXVert(),2) + pow(S1Mesh[i][j].getyMin() \
			-sphere1.getYVert(),2)  + pow(scint1.getZVert()-sphere1.getZVert(),2) ) ) {
		return true;
	} else if (sphere1.getRadius() > sqrt( pow(S1Mesh[i][j].getxMin()-sphere1.getXVert(),2) + pow(S1Mesh[i][j].getyMax() \
			-sphere1.getYVert(),2) + pow(scint1.getZVert()-sphere1.getZVert(),2) ) ) {
		return true;
	} else if (sphere1.getRadius() > sqrt( pow(S1Mesh[i][j].getxMax()-sphere1.getXVert(),2) + pow(S1Mesh[i][j].getyMax() \
			-sphere1.getYVert(),2) + pow(scint1.getZVert()-sphere1.getZVert(),2) ) ) {
		return true;
	}
    return false;
}



#endif
