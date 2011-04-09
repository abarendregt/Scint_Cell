#include <string>
#include <sstream>
#include <iostream>
#include "MeshRegion.h"

using namespace std;

class RectangularParallelpiped: public MeshRegion {
  double xMin, xMax, yMin, yMax, elevation, height;
  
  int xLoc, yLoc;
  
  string PART_NAME;
  string PART_TYPE;
  
  public:
  
  //This section defines each member of the class
  void setxLoc(int x) {this->xLoc = x;}
  void setyLoc(int y) {this->yLoc = y;}
  void setxMin(double xMn) {this->xMin = xMn;}
  void setxMax(double xMx) {this->xMax = xMx;} 
  void setyMin(double yMn) {this->yMin = yMn;}
  void setyMax(double yMx) {this->yMax = yMx;}
  void setHeight(double h) {this->height = h;}
  void setElevation(double e) {this->elevation = elevation;}
    
  
  //This section calls each member of the class for use
  int getxLoc(void) {return this->xLoc;}
  int getyLoc(void) {return this->yLoc;}
  double getxMin(void) {return this->xMin;}
  double getxMax(void) {return this->xMax;}
  double getyMin(void) {return this->yMin;}
  double getyMax(void) {return this->yMax;}
  double getHeight(void) {return this->height;}
  double getElevation(void) {return this->elevation;}
  
  RectangularParallelpiped::RectangularParallelpiped(int xLoc, int yLoc, double xMin, double xMax, double yMin, double yMax, double height, double elevation) {
    this->xLoc = xLoc;
    this->yLoc = yLoc;
    this->xMin = xMin;
    this->xMax = xMax;
    this->yMin = yMin;
    this->yMax = yMax;
    this->height = height;
    this->elevation = elevation;
    this->PART_NAME = "Scint";
    this->PART_TYPE = "ag_rpp10";
  }


  string toGeo(void) {
    ostringstream strs;
    strs << "'" << PART_NAME << "_" << xLoc << yLoc << "'  '" << PART_TYPE << "' /" << endl;
    strs << xMin << ", " << xMax << ", " << yMin << ", " << yMax<< ", " << elevation << ", " << height<<" /" << endl;
    strs << "'NULL' /" << endl;
    return strs.str();
  }
};