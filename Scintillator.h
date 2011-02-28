
#ifndef SCINTILLATOR_H
#define SCINTILLATOR_H
using namespace std;
#include <string>

class Scintillator {
	string name;
	int yMesh, zmesh;
	double xLength, yLength, zLength, xVert, yVert, zVert;

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

#endif
