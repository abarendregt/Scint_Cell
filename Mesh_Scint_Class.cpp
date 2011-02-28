#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <math.h>
#include <vector>
#include "MeshScintillator.h"
#include "Scintillator.h"
#include "Sphereisect.h"
using namespace std;


void SetMeshBoundary(int Row, int Col,vector< vector<MeshScintillator > >& S1Mesh, Scintillator s);
void AssignTriPT(int i, int j,vector< vector<MeshScintillator > >& S1Mesh, Scintillator scint1, Sphereisect sphere1);
void AssignRppPT(int i, int j,vector< vector<MeshScintillator > >& S1Mesh, Scintillator scint1, Sphereisect sphere1);


int main () {
  Scintillator scint1;
  Sphereisect sphere1;


// These are the variables that I need to define
	int Col = 2*scint1.getYMesh();
	int Row = scint1.getYMesh();
	int i, j;

//Define Meshing 2Darray
	vector<vector<MeshScintillator > > S1Mesh;
	S1Mesh.resize(Row);
  	for (i = 0; i<Row;++i)
		S1Mesh[i].resize(Col);

//Define xmin, xmax, ymin, ymax
	SetMeshBoundary(Row, Col, S1Mesh, scint1);
	
//Assign proper part type to MeshScintillator
/************************************************************************** 
  Current test is the node test.  This test assumes the sphere has the same
 'x' and 'y' vertex as the scintillator. If this is not the case, then this
 test may fail sometimes.  Consider adding more tests at a later time.

 Also assumed is that the sphere vertex is lower than the scintillator.
 This should be fixed at a later time!!!! (#!#)
****************************************************************************/ 

	int BlankNumb = scint1.getYMesh()/2-1;  //This is the maximum # of null cells

	for (i=0;i<Row;++i){	
		for (j=0;j<Col;++j) {
			if (j+1 <= abs(BlankNumb) ||  j > Col-abs(BlankNumb)-1) {
				S1Mesh[i][j].setPT(null);  
			} else if (j+1 == abs(BlankNumb)+1 ||  j == Col-abs(BlankNumb)-1) {
					AssignTriPT(i,j,S1Mesh,scint1,sphere1);
			} else { 
					AssignRppPT(i,j,S1Mesh,scint1,sphere1);
			}
		}
		BlankNumb--;
		if (i==Row/2-1) {
			BlankNumb=0;
		}
	}



//This section defines the initial conditions required for making 
	double Mesh_Height=scint1.getZLength()/scint1.getZMesh();
	double z_top=scint1.getZLength()+scint1.getZVert();
	double z_isect[4];  //This defines the elevation where the four edges of the RPP or three edges of the TRI intersect 
							// the sphere
	double z_bottom;
	int z_numb;

 
	for (i=0;i<Row;++i){	
		for (j=0;j<Col;++j) {
			if ((S1Mesh[i][j].getPT() != 4) && (S1Mesh[i][j].getPT() != 2)){ 		
				// #!# Print TRI_plain or RPP_plain!!!
				// This is for 
			} 
			else {  //This section of code analyses intersecting RPPs or TRIs only

				z_isect[0]=sqrt( sphere1.getRadius()*sphere1.getRadius() - pow(S1Mesh[i][j].getxMax()-sphere1.getXVert(),2) \
								- pow(S1Mesh[i][j].getyMax()-sphere1.getYVert(),2) ) +sphere1.getZVert();
				z_isect[1]=sqrt( sphere1.getRadius()*sphere1.getRadius() - pow(S1Mesh[i][j].getxMin()-sphere1.getXVert(),2) \
								- pow(S1Mesh[i][j].getyMax()-sphere1.getYVert(),2) ) +sphere1.getZVert();
				z_isect[2]=sqrt( sphere1.getRadius()*sphere1.getRadius() - pow(S1Mesh[i][j].getxMax()-sphere1.getXVert(),2) \
								- pow(S1Mesh[i][j].getyMin()-sphere1.getYVert(),2) ) +sphere1.getZVert();
				z_isect[3]=sqrt( sphere1.getRadius()*sphere1.getRadius() - pow(S1Mesh[i][j].getxMin()-sphere1.getXVert(),2) \
								- pow(S1Mesh[i][j].getyMin()-sphere1.getYVert(),2) ) +sphere1.getZVert();

				// End initializing this part, This equation is verified and works properly


				cout<<z_isect[0]<<", "<<z_isect[1]<<", "<<z_isect[2]<<", "<<z_isect[3]<<endl;
				double z=z_top;
				z_numb=0;
				while ((z > z_isect[0]) && (z > z_isect[1]) && (z > z_isect[2]) && (z > z_isect[3])){		
					z=z-Mesh_Height;
					z_numb++;
				}

				//Here is the starting elevation where z needs to be tested.  All the RPP parts above this elevation 
				// should be sent to the Print_RPP_Plain function/object 

				// Testing function 1, this function is in place to test whether or not the volume of the RPP or TRI 
            //intersected with a sphere is too small to model.  If this is the case then the volume is merged with a 
				//larger RPP or TRI so the shape can be preserved.

				double z_numb_INT=0;
				double z_Sph=z;
				int count;
				while ("TRUE"){		
					//Test if the sphere is interescting 1,2,3 or all four edges
					z_bottom=z_Sph;
					z_numb_INT++;
					count=0;  //This is the number of corners that are below the current elevation 
					if(z_bottom >= z_isect[0]){
						count++;
					} if(z_bottom >= z_isect[1]){
						count++;
					} if(z_bottom >= z_isect[2]){
						count++;
					} if(z_bottom >= z_isect[3]){
						count++;
					}
				
					if(count == 1){
					//Add Calculate Volume function.  Calculate volume of tetrahedron
						/*if(Volume too small){
					  find lowest point.  
						  z = lowest point
		
					  break;
						  } else 
						z_numb_INT--;
						continue;
					*/
						cout<<"count=1"<<endl;
						break;
					} if (count == 2){
						//Add Calculate Volume function.  Calculate volume of frustrum of tetrahedron
						/*if(Volume too small){
						  find lowest point.  
						  z = lowest point
						  break;
						  } else 
							z_numb_INT--;
							continue;
						*/
						cout<<"count=2"<<endl;
						break;
					} if (count == 4){
						cout<<"count=4"<<endl;
						break; //part of testing delete after verification
					} if (count == 3){
						cout<<"count=3"<<endl; //This is a test. delete after verification
					} if (z_Sph<=scint1.getZVert()){
						break;
					}
					z_Sph=z_Sph-Mesh_Height; 
				}
			//End Test Function 1


// **************** Start of printing function  ********************************************8
// This section does two things.
// 1) Is calculates the z starting, middle and ending elevations
// 2) It prints the part definition as understood by RAMA.  
//  Current assumptions:  This assumes that the vertex is at the center of the scintillator cell
//                        this must be modified later.	

	
				//Test output (used for printing function)
				cout<<i<<", "<<j<<", "<<S1Mesh[i][j].getxMin()<<", "<<S1Mesh[i][j].getxMax()<<", ";
				cout<<S1Mesh[i][j].getyMin()<<", "<<S1Mesh[i][j].getyMax()<<", "<<z_top<<", "<<z_Sph;
				cout<<", "<<z_numb_INT<<", "<<S1Mesh[i][j].getPT()<<endl<<endl;


			}	
		}
	}
	return 0;
}




void SetMeshBoundary(int Row, int Col,vector< vector<MeshScintillator > >& S1Mesh, Scintillator s) {
	double yMeshLth=s.getYLength()/s.getYMesh();
	double xMeshLth=s.getXLength()/(2*s.getYMesh());
	double xmin, xmax, ymin, ymax;

	//define xmin, xmax, ymin and ymax based on starting location (0,0)
	// Define the rows


	for (int i=0;i<Row;++i){	 
		ymax= s.getYVert() + s.getYLength()/2 -i*yMeshLth;
		ymin=ymax-yMeshLth;

		if (i==Row-1) {  //this is in place on to eliminate any rounding errors
			ymin = s.getYVert() - s.getYLength()/2;
		}

		for (int j=0;j<Col;++j) {
			xmin= s.getXVert()-s.getXLength()/2+j*xMeshLth;
			xmax=xmin+xMeshLth;

			if (j==Col-1) {  //this is in place on to eliminate any rounding errors
			xmax = s.getXVert() + s.getXLength()/2;
			}

			S1Mesh[i][j].setxMin(xmin);
			S1Mesh[i][j].setyMin(ymin);
			S1Mesh[i][j].setxMax(xmax);
			S1Mesh[i][j].setyMax(ymax);
		}
	}
	//return S1Mesh[Row][Col];
}


void AssignTriPT(int i, int j,vector< vector<MeshScintillator > >& S1Mesh, Scintillator scint1, Sphereisect sphere1){
	if (sphere1.getRadius() > sqrt( pow(S1Mesh[i][j].getxMin()-sphere1.getXVert(),2) + pow(S1Mesh[i][j].getyMin() \
	    -sphere1.getYVert(),2) + pow(scint1.getZVert()-sphere1.getZVert(),2) ) ) {
   	S1Mesh[i][j].setPT(Tri_Isect);
   } else if (sphere1.getRadius() > sqrt( pow(S1Mesh[i][j].getxMax()-sphere1.getXVert(),2) + pow(S1Mesh[i][j].getyMin() \
				-sphere1.getYVert(),2) + pow(scint1.getZVert()-sphere1.getZVert(),2) ) ) {
      S1Mesh[i][j].setPT(Tri_Isect);
   } else if (sphere1.getRadius() > sqrt( pow(S1Mesh[i][j].getxMin()-sphere1.getXVert(),2) + pow(S1Mesh[i][j].getyMax() \
				-sphere1.getYVert(),2) + pow(scint1.getZVert()-sphere1.getZVert(),2) ) ) {
      S1Mesh[i][j].setPT(Tri_Isect);
   } else if (sphere1.getRadius() > sqrt( pow(S1Mesh[i][j].getxMax()-sphere1.getXVert(),2) + pow(S1Mesh[i][j].getyMax() \
				  -sphere1.getYVert(),2) + pow(scint1.getZVert()-sphere1.getZVert(),2) ) ) {
      S1Mesh[i][j].setPT(Tri_Isect);
   } else {
      S1Mesh[i][j].setPT(Tri_Plain);
   }
}


void AssignRppPT(int i, int j,vector< vector<MeshScintillator > >& S1Mesh, Scintillator scint1, Sphereisect sphere1){
	if (sphere1.getRadius() > sqrt( pow(S1Mesh[i][j].getxMin()-sphere1.getXVert(),2) + pow(S1Mesh[i][j].getyMin()  \
			-sphere1.getYVert(),2) + pow(scint1.getZVert()-sphere1.getZVert(),2) ) ) {
		S1Mesh[i][j].setPT(RPP_Isect);
	} else if (sphere1.getRadius() > sqrt( pow(S1Mesh[i][j].getxMax()-sphere1.getXVert(),2) + pow(S1Mesh[i][j].getyMin() \
			-sphere1.getYVert(),2)  + pow(scint1.getZVert()-sphere1.getZVert(),2) ) ) {
		S1Mesh[i][j].setPT(RPP_Isect);
	} else if (sphere1.getRadius() > sqrt( pow(S1Mesh[i][j].getxMin()-sphere1.getXVert(),2) + pow(S1Mesh[i][j].getyMax() \
			-sphere1.getYVert(),2) + pow(scint1.getZVert()-sphere1.getZVert(),2) ) ) {
		S1Mesh[i][j].setPT(RPP_Isect);
	} else if (sphere1.getRadius() > sqrt( pow(S1Mesh[i][j].getxMax()-sphere1.getXVert(),2) + pow(S1Mesh[i][j].getyMax() \
			-sphere1.getYVert(),2) + pow(scint1.getZVert()-sphere1.getZVert(),2) ) ) {
		S1Mesh[i][j].setPT(RPP_Isect);
	} else {
		S1Mesh[i][j].setPT(RPP_Plain);
	}
}


