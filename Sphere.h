
#ifndef SPHEREISECT_H
#define SPHEREISECT_H
using namespace std;
#include <string>

class Sphere {
	string name;
	double Radius, xVert, yVert, zVert;
        
	public:
        Sphereisect();
        Sphereisect(string name, double Radius, double xVert, double yVert, double zVert);
	void setName(string N) {this->name=N;}
	void setRadius(double rad) {this->Radius = rad;}
	void setXVert(double Vx) {this->xVert = Vx;}
	void setYVert(double Vy) {this->yVert = Vy;}
	void setZVert(double Vz) {this->zVert = Vz;}
	//This section is the variables used in defining the scintillator cell
	string getName(void) {return this->name;}
	double getRadius(void) {return this->Radius;}
	double getXVert(void) {return this->xVert;}
	double getYVert(void) {return this->yVert;}
	double getZVert(void) {return this->zVert;}
};

Sphere::Sphere() {
	this->setName("Sphere");
        this->setRadius(0.5);
	this->setXVert(0.0);
	this->setYVert(0.0);
	this->setZVert(-0.27);
}

Sphere::Sphere(string name, double Radius, double xVert, double yVert, double zVert) {
	this->setName(name);

	this->setRadius(Radius);

	this->setXVert(xVert);
	this->setYVert(yVert);
	this->setZVert(zVert);
}

#endif
