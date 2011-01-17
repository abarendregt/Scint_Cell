#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <math.h>
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
  double xLength, yLength, yMesh, zLength, zmesh, xVert, yVert, zVert;
  public:
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



int main () {
  Scintillator scint1;
  scint1.setName("Scint1");
  scint1.setXLength(3.72);
  scint1.setYLength(3.22);
  scint1.setYMesh(4);  //This number must be even (#!# add test later)
  scint1.setZLength(0.3);
  scint1.setZMesh(8);
  scint1.setXVert(0.0);
  scint1.setYVert(0.0);
  scint1.setZVert(0.0);
  scint1.getName();
  scint1.getXLength();
  scint1.getYLength();
  scint1.getYMesh();
  scint1.getZLength();
  scint1.getZMesh();
  scint1.getXVert();
  scint1.getYVert();
  scint1.getZVert();

//These are inputs from the Sphere Coordinates
	string Sph_name;
	double Sph_Rad=5.7;
   double Sph_VertX=0;
   double Sph_VertY=0; 
	double Sph_VertZ=-5.47;

// These are the variables that I need to define
	int Col = 2*scint1.getYMesh();
	int Row = scint1.getYMesh();

//Define Meshing array
	MeshScintillator S1Mesh[Row][Col];

	double yMeshLth=scint1.getYLength()/scint1.getYMesh();
	double xMeshLth=scint1.getXLength()/(2*scint1.getYMesh());


//**************************Start of function to Define xmin, xmax, ymin, ymax***************
//****************************************************************************************
//****************************************************************************************
	int i, j;
	double xmin, xmax, ymin, ymax;
	//define xmin, xmax, ymin and ymax based on starting location (0,0)
	// Define the rows


	for (i=0;i<Row;++i){	 
		ymax= scint1.getYVert() + scint1.getYLength()/2 -i*yMeshLth;
		ymin=ymax-yMeshLth;

		if (i==Row-1) {  //this is in place on the chance rounding error occurs
			ymin = scint1.getYVert() - scint1.getYLength()/2;
		}

		for (j=0;j<Col;++j) {
			xmin= scint1.getXVert()-scint1.getXLength()/2+j*xMeshLth;
			xmax=xmin+xMeshLth;

			if (j==Col-1) {  //this is in place on the chance rounding error occurs
			xmax = scint1.getXVert() + scint1.getXLength()/2;
			}

			S1Mesh[i][j].setxMin(xmin);
			S1Mesh[i][j].setyMin(ymin);
			S1Mesh[i][j].setxMax(xmax);
			S1Mesh[i][j].setyMax(ymax);
			//This was in place to test if this works properly			
			//cout<<i<<", "<<j<<", "<<S1Mesh[i][j].getxMin()<<", "<<S1Mesh[i][j].getxMax();
			//cout<<", "<<S1Mesh[i][j].getyMin()<<", "<<S1Mesh[i][j].getyMax()<<endl;
		}
	}

//******************************************************************************************
//******************************************************************************************
//***********************************End of function to Define Type*************************
// This function is tested and works properly 12/31/10!!!!!!!********************************



//**************************Start of function to Define type********************************
//****************************************************************************************
//****************************************************************************************
	int BlankNumb;
	double r;
	BlankNumb = scint1.getYMesh()/2-1;  //This is the maximum # of null cells
	// Define the rows
	for (i=0;i<Row;++i){	
		for (j=0;j<Col;++j) {
			if (j+1 <= abs(BlankNumb) ||  j > Col-abs(BlankNumb)-1) {
				S1Mesh[i][j].setPT(null);  
				S1Mesh[i][j].getPT();
				cout<<i<<", "<<j<<", "<<S1Mesh[i][j].getPT()<<", null"<<endl;
			} else if (j+1 == abs(BlankNumb)+1 ||  j == Col-abs(BlankNumb)-1) {
			  	// set up new function NodealTest here!!!
				/************************************************************************** 
				  Current test is the node test.  This test assumes the sphere has the same
				 'x' and 'y' vertex as the scintillator. If this is not the case, then this
				 test may fail sometimes.  Consider adding more tests at a later time.

				  Also assumed is that the sphere vertex is lower than the scintillator.
				  This should be fixed at a later time!!!! (#!#)
				****************************************************************************/ 
					if (Sph_Rad > sqrt( pow(S1Mesh[i][j].getxMin()-Sph_VertX,2) + pow(S1Mesh[i][j].getyMin()-Sph_VertY,2) \
						+ pow(scint1.getZVert()-Sph_VertZ,2) ) ) {
						S1Mesh[i][j].setPT(Tri_Isect);
						S1Mesh[i][j].getPT();
						cout<<i<<", "<<j<<", "<<S1Mesh[i][j].getPT()<<r<<", TRI_Isect"<<endl;
					} else if (Sph_Rad > sqrt( pow(S1Mesh[i][j].getxMax()-Sph_VertX,2) + \
						pow(S1Mesh[i][j].getyMin()-Sph_VertY,2) + pow(scint1.getZVert()-Sph_VertZ,2) ) ) {
						S1Mesh[i][j].setPT(Tri_Isect);
						S1Mesh[i][j].getPT();
						cout<<i<<", "<<j<<", "<<S1Mesh[i][j].getPT()<<", TRI_Isect"<<endl;
					} else if (Sph_Rad > sqrt( pow(S1Mesh[i][j].getxMin()-Sph_VertX,2) + \
						pow(S1Mesh[i][j].getyMax()-Sph_VertY,2) + pow(scint1.getZVert()-Sph_VertZ,2) ) ) {
						S1Mesh[i][j].setPT(Tri_Isect);
						S1Mesh[i][j].getPT();
						cout<<i<<", "<<j<<", "<<S1Mesh[i][j].getPT()<<", TRI_Isect"<<endl;
					} else if (Sph_Rad > sqrt( pow(S1Mesh[i][j].getxMax()-Sph_VertX,2) + \
						pow(S1Mesh[i][j].getyMax()-Sph_VertY,2) + pow(scint1.getZVert()-Sph_VertZ,2) ) ) {
						S1Mesh[i][j].setPT(Tri_Isect);
						S1Mesh[i][j].getPT();
						cout<<i<<", "<<j<<", "<<S1Mesh[i][j].getPT()<<", TRI_Isect"<<endl;
					} else {
						S1Mesh[i][j].setPT(Tri_Plain);
						S1Mesh[i][j].getPT();
						cout<<i<<", "<<j<<", "<<S1Mesh[i][j].getPT()<<", TRI_Plain"<<endl;
					}
			// End new function NodalTest here!!!
			// Syntax is correct
			} else { 
			// (Note: This section is the same as the NodalTest Function that needs to get implemented eventually)
					if (Sph_Rad > sqrt( pow(S1Mesh[i][j].getxMin()-Sph_VertX,2) + pow(S1Mesh[i][j].getyMin()-Sph_VertY,2) \
						+ pow(scint1.getZVert()-Sph_VertZ,2) ) ) {
						S1Mesh[i][j].setPT(RPP_Isect);
						S1Mesh[i][j].getPT();
						r=sqrt( pow(S1Mesh[i][j].getxMin()-Sph_VertX,2) + pow(S1Mesh[i][j].getyMin()-Sph_VertY,2) + \
							pow(scint1.getZVert()-Sph_VertZ,2) );
						cout<<i<<", "<<j<<", "<<S1Mesh[i][j].getPT()<<", "<<r<<", RPP_Isect"<<endl;
					} else if (Sph_Rad > sqrt( pow(S1Mesh[i][j].getxMax()-Sph_VertX,2) + \
						pow(S1Mesh[i][j].getyMin()-Sph_VertY,2)  + pow(scint1.getZVert()-Sph_VertZ,2) ) ) {
						S1Mesh[i][j].setPT(RPP_Isect);
						S1Mesh[i][j].getPT();
						r=sqrt( pow(S1Mesh[i][j].getxMax()-Sph_VertX,2) + pow(S1Mesh[i][j].getyMin()-Sph_VertY,2) \
							+ pow(scint1.getZVert()-Sph_VertZ,2) );
						cout<<i<<", "<<j<<", "<<S1Mesh[i][j].getPT()<<", "<<r<<", RPP_Isect"<<endl;
					} else if (Sph_Rad > sqrt( pow(S1Mesh[i][j].getxMin()-Sph_VertX,2) + \
						pow(S1Mesh[i][j].getyMax()-Sph_VertY,2) + pow(scint1.getZVert()-Sph_VertZ,2) ) ) {
						S1Mesh[i][j].setPT(RPP_Isect);
						S1Mesh[i][j].getPT();
						r=sqrt( pow(S1Mesh[i][j].getxMin()-Sph_VertX,2) + pow(S1Mesh[i][j].getyMax()-Sph_VertY,2) \
							+ pow(scint1.getZVert()-Sph_VertZ,2) );
						cout<<i<<", "<<j<<", "<<r<<S1Mesh[i][j].getPT()<<", "<<r<<", RPP_Isect"<<endl;
					} else if (Sph_Rad > sqrt( pow(S1Mesh[i][j].getxMax()-Sph_VertX,2) + \
						pow(S1Mesh[i][j].getyMax()-Sph_VertY,2) + pow(scint1.getZVert()-Sph_VertZ,2) ) ) {
						S1Mesh[i][j].setPT(RPP_Isect);
						S1Mesh[i][j].getPT();
						r=sqrt( pow(S1Mesh[i][j].getxMax()-Sph_VertX,2) + pow(S1Mesh[i][j].getyMax()-Sph_VertY,2) \
							+ pow(scint1.getZVert()-Sph_VertZ,2) );
						cout<<i<<", "<<j<<", "<<S1Mesh[i][j].getPT()<<", "<<r<<", RPP_Isect"<<endl;
					} else {
						S1Mesh[i][j].setPT(RPP_Plain);
						S1Mesh[i][j].getPT();
						r=sqrt( pow(S1Mesh[i][j].getxMax()-Sph_VertX,2) + pow(S1Mesh[i][j].getyMax()-Sph_VertY,2) \
						+ pow(scint1.getZVert()-Sph_VertZ,2) );
						cout<<i<<", "<<j<<", "<<S1Mesh[i][j].getPT()<<", "<<r<<", RPP_Plain"<<endl;
					}
				// Syntax is correct.  Requires more testing to spreadsheet to verify 100% correctness
			// End NodalTest function
			}
		}
		BlankNumb=BlankNumb-1;
		if (i==Row/2-1) {
			BlankNumb=0;
		}
	}
//******************************************************************************************
//******************************************************************************************
//***********************************End of function to Define Type*************************


// **************** Start of printing function  ********************************************8
// This section does two things.
// 1) Is calculates the z starting, middle and ending elevations
// 2) It prints the part definition as understood by RAMA.  
//  Current assumptions:  This assumes that the vertex is at the center of the scintillator cell
//                        this must be modified later.	


//This may be the method used, however I am leaning towards what is provided below
/*	double z_elv[scint1.getZMesh()];
	// This array will be used to initialize the elevations for the scintillator. 
	double Mesh_Height=scint1.getZLength()/scint1.getZMesh();

	for (i=0;i<=scint1.getZMesh();++i){
		//Starting with the lower elevation
		if (i == 0){			
			z_elv[i]=scint1.getZVert();
		} else if (i == scint1.getZMesh()){
			z_elv[i]=scint1.getZVert()+scint1.getZLength();
		} else {
			z_elv[i]=z_elv[i-1]+Mesh_Height;
		}
	}
*/

	

//This section defines the initial conditions required for making 
	double z_int[4];  //This defines the elevation where the four edges of the RPP or three edges of the TRI intersect 
							// the sphere
	z_int[0]=sqrt( Sph_Rad*Sph_Rad - pow(S1Mesh[i][j].getxMax()-Sph_VertX,2) + pow(S1Mesh[i][j].getyMax()-Sph_VertY,2) ) \ 					+scint1.getZVert();
	z_int[1]=sqrt( Sph_Rad*Sph_Rad - pow(S1Mesh[i][j].getxMin()-Sph_VertX,2) + pow(S1Mesh[i][j].getyMax()-Sph_VertY,2) ) \ 					+scint1.getZVert();
	z_int[2]=sqrt( Sph_Rad*Sph_Rad - pow(S1Mesh[i][j].getxMax()-Sph_VertX,2) + pow(S1Mesh[i][j].getyMin()-Sph_VertY,2) ) \ 					+scint1.getZVert();
	z_int[3]=sqrt( Sph_Rad*Sph_Rad - pow(S1Mesh[i][j].getxMin()-Sph_VertX,2) + pow(S1Mesh[i][j].getyMin()-Sph_VertY,2) ) \ 					+scint1.getZVert();
// End initializing this part		
	double z=scint1.getZLength()+scint1.getZVert();
	while (z > z_int[0]) || (z > z_int[1]) || (z > z_int[2]) || (z > z_int[3]){		
		z=z-Mesh_Height;
	}
	z=z+Mesh_Height;
//Here is the starting elevation where z needs to be tested.  All the RPP parts above this elevation should be sent to 
//The Print_RPP_Plain function/object 

// Testing function 1, this function is in place to test whether or not the volume of the RPP or TRI intersected with
// a sphere is too small to model.  If this is the case then the volume is merged with a larger RPP or TRI so the
// shape can be preserved.
	double z_bottom;
	while (TRUE){		
		//Test if the sphere is interesction 1,2,3 or all four
		zbottom=z-Mesh_Height;
		int count=0;
		if(zbottom >= z_int[0]){
			count++;
		}
		if(zbottom >= z_int[1]){
			count++;
		}
		if(zbottom >= z_int[2]){
			count++;
		}
		if(zbottom >= z_int[3]){
			count++;
		}
		
		if(count == 3){
			//Add Calculate Volume function.  Calculate volume of tetrahedron
			/*if(Volume too small){
			  find lowest point.  
			  z = lowest point
			  exit;
			  } else continue;
			*/
		} 
		if (count == 2){
			//Add Calculate Volume function.  Calculate volume of frustrum of tetrahedron
			/*if(Volume too small){
			  find lowest point.  
			  z = lowest point
			  exit;
			  } else continue;
			*/
		}  
		if (count == 4){
			exit;
		}
		//#!#Set up intersected printing function here from zbottom to z
		z=z-Mesh_Height; 			
	}



	return 0;
}

