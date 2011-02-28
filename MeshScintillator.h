
#ifndef MESHSCINTILLATOR_H
#define MESHSCINTILLATOR_H

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <math.h>

enum PT {null,Tri_Plain,Tri_Isect,RPP_Plain,RPP_Isect};

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


#endif
