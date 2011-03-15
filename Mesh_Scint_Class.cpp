#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "MeshScintillator.h"
#include "Scintillator.h"
#include "Sphereisect.h"
using namespace std;


void SetMeshBoundary(int Row, int Col,vector< vector<MeshScintillator > >& S1Mesh, Scintillator s);
void AssignTriPT(int i, int j,vector< vector<MeshScintillator > >& S1Mesh, Scintillator scint1, Sphereisect sphere1);
void AssignRppPT(int i, int j,vector< vector<MeshScintillator > >& S1Mesh, Scintillator scint1, Sphereisect sphere1);
void bubbleSort(double arr[], int n);
double FindSpherePoint(double r, double x_loc, double y_loc);

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
 This function assumes the sphere has the same 'x' and 'y' vertex as the scintillator. 
 If this is not the case, then this test may fail.  Consider adding more tests at a later time.

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
	double z_isect[4], sorted_isect[4];  //This defines the elevation where the four edges of the RPP or three edges of the TRI intersect 
							// the sphere
	double z_bottom=scint1.getZVert();
	int z_numb;
	vector <double> z_elvs;
 	z_elvs.resize(scint1.getZMesh()+1);


	for (i=0;i<Row;++i){	
		for (j=0;j<Col;++j) {
			if ((S1Mesh[i][j].getPT() == Tri_Plain) || (S1Mesh[i][j].getPT() == RPP_Plain)){ 		
				// #!# Print TRI_plain or RPP_plain!!!
				// This is for 
			} 
			else if ((S1Mesh[i][j].getPT() == Tri_Isect) || (S1Mesh[i][j].getPT() == RPP_Isect)) {  

				z_isect[0]=FindSpherePoint(sphere1.getRadius(),S1Mesh[i][j].getxMax()-sphere1.getXVert(), \
								S1Mesh[i][j].getyMax()-sphere1.getYVert()) +sphere1.getZVert();  //Fix for arrays 1,2 and 3
				z_isect[1]=sqrt( sphere1.getRadius()*sphere1.getRadius() - pow(S1Mesh[i][j].getxMin()-sphere1.getXVert(),2) \
								- pow(S1Mesh[i][j].getyMax()-sphere1.getYVert(),2) ) +sphere1.getZVert();
				z_isect[2]=sqrt( sphere1.getRadius()*sphere1.getRadius() - pow(S1Mesh[i][j].getxMax()-sphere1.getXVert(),2) \
								- pow(S1Mesh[i][j].getyMin()-sphere1.getYVert(),2) ) +sphere1.getZVert();
				z_isect[3]=sqrt( sphere1.getRadius()*sphere1.getRadius() - pow(S1Mesh[i][j].getxMin()-sphere1.getXVert(),2) \
								- pow(S1Mesh[i][j].getyMin()-sphere1.getYVert(),2) ) +sphere1.getZVert();

				cout<<z_isect[0]<<", "<<z_isect[1]<<", "<<z_isect[2]<<", "<<z_isect[3]<<endl;

				if ((z_isect[0]!=z_isect[0])||(z_isect[1]!=z_isect[1])||(z_isect[2]!=z_isect[2])||(z_isect[3]!=z_isect[3])){
					// No voided regions. Print TRI_isect or RPP_isect
					cout<<"Break"<<endl;					
					continue;
				}  
				if 
				((z_isect[0]<0)||(z_isect[1]<0)||(z_isect[2]<0)||(z_isect[3]<0)){
					// No voided regions. Print TRI_isect or RPP_isect
					cout<<"Break"<<endl;					
					continue;
				}  
				for (int k=0;k<4;++k){
					sorted_isect[k]=z_isect[k];
				}
				bubbleSort(sorted_isect, 4);
				cout<<sorted_isect[0]<<", "<<sorted_isect[1]<<", "<<sorted_isect[2]<<", "<<sorted_isect[3]<<endl;

				double top_isect=sorted_isect[3];
				double bot_isect=sorted_isect[0];
				int z_numb=0;

				//number of full RPPs
				double temp=z_top;
				while (z_top < top_isect){		
					temp=temp - Mesh_Height;
					z_numb++;
				}
				
				temp=z_top;
				if (z_numb > 1) {
					z_elvs.resize(z_numb+1);
					for (int n=0;i<=z_numb;n++){
						z_elvs[n]=temp;
						temp= temp - Mesh_Height;
					}
					//#!# Add Print RPP_FULL function
				}
				
				//find upper height of voided region
				while (z_bottom > bot_isect-Mesh_Height) {
					z_bottom = z_bottom + Mesh_Height;
				}
				
				// Test intersecting region
				z_top=z_elvs[z_numb+1];
				temp=z_bottom;
				vector <double> test_elvs;
				test_elvs.resize(scint1.getZMesh()+1);
				test_elvs[0]=z_bottom;
				int n=0;
				double Total_Vol=Mesh_Height*(scint1.getYLength()/scint1.getYMesh())*(scint1.getXLength()/(2*scint1.getYMesh()));
				double testHeight,test_Xlen,test_Ylen,test_Vol;
				while (temp > z_top) {
					temp = temp + Mesh_Height;
					if (temp < z_isect[2]){
						if (temp < z_isect[1]){
							// test using volume of tetrahedron
							testHeight=temp-bot_isect;
							if(sorted_isect[0]==z_isect[0]){
								test_Xlen=FindSpherePoint(sphere1.getRadius(),temp,S1Mesh[i][j].getyMax()-sphere1.getYVert())/
													 -S1Mesh[i][j].getxMax();
								test_Ylen=FindSpherePoint(sphere1.getRadius(),temp,S1Mesh[i][j].getxMax()-sphere1.getXVert())/
													 -S1Mesh[i][j].getyMax();
								test_Vol=testHeight*test_Xlen*test_Ylen/6;
							}
							else if (sorted_isect[0]==z_isect[1]){
								test_Xlen=FindSpherePoint(sphere1.getRadius(),temp,S1Mesh[i][j].getyMax()-sphere1.getYVert())/
													 -S1Mesh[i][j].getxMin();
								test_Ylen=FindSpherePoint(sphere1.getRadius(),temp,S1Mesh[i][j].getxMin()-sphere1.getXVert())/
													 -S1Mesh[i][j].getyMax();
								test_Vol=testHeight*test_Xlen*test_Ylen/6;
							}
							else if (sorted_isect[0]==z_isect[2]){
								test_Xlen=FindSpherePoint(sphere1.getRadius(),temp,S1Mesh[i][j].getyMin()-sphere1.getYVert())/
													 -S1Mesh[i][j].getxMax();
								test_Ylen=FindSpherePoint(sphere1.getRadius(),temp,S1Mesh[i][j].getxMax()-sphere1.getXVert())/
													 -S1Mesh[i][j].getyMin();
								test_Vol=testHeight*test_Xlen*test_Ylen/6;
							}
							else{
								test_Xlen=FindSpherePoint(sphere1.getRadius(),temp,S1Mesh[i][j].getyMin()-sphere1.getYVert())/
													 -S1Mesh[i][j].getxMin();
								test_Ylen=FindSpherePoint(sphere1.getRadius(),temp,S1Mesh[i][j].getxMin()-sphere1.getXVert())/
													 -S1Mesh[i][j].getyMin();
								test_Vol=testHeight*test_Xlen*test_Ylen/6;
							}
							
							
							if (test_Vol < 0.05*Total_Vol){
								//Too small of a volume
								continue;
							} else{
								n++;
								test_elvs[n]=temp; 
							}
						}
						else {
							// test using frustrum of tetrahedron
							testHeight=temp-bot_isect;
							double A1,A2,test_Xlen2,test_Ylen2;
							if(sqrt(abs(pow(sphere1.getRadius(),2)-temp*temp))<=S1Mesh[i][j].getxMax()){
								test_Ylen=FindSpherePoint(sphere1.getRadius(),temp,S1Mesh[i][j].getxMax()-sphere1.getXVert())/
												-S1Mesh[i][j].getyMin();
								A1=0.5*test_Ylen*testHeight;
								test_Ylen2=FindSpherePoint(sphere1.getRadius(),temp,S1Mesh[i][j].getxMax()-sphere1.getXVert())/
												-S1Mesh[i][j].getyMax();
								A2=0.5*test_Ylen2*testHeight;
								test_Vol=(A1+A2+sqrt(A1*A2))/3;
							}
							else if (sqrt(abs(pow(sphere1.getRadius(),2)-temp*temp))<=S1Mesh[i][j].getxMin()){
								test_Ylen=FindSpherePoint(sphere1.getRadius(),temp,S1Mesh[i][j].getxMin()-sphere1.getXVert())/
												-S1Mesh[i][j].getyMin();
								A1=0.5*test_Ylen*testHeight;
								test_Ylen2=FindSpherePoint(sphere1.getRadius(),temp,S1Mesh[i][j].getxMin()-sphere1.getXVert())/
												-S1Mesh[i][j].getyMax();
								A2=0.5*test_Ylen2*testHeight;
								test_Vol=(A1+A2+sqrt(A1*A2))/3;
							}
							else if (sqrt(abs(pow(sphere1.getRadius(),2)-temp*temp))<=S1Mesh[i][j].getyMax()){
								test_Xlen=FindSpherePoint(sphere1.getRadius(),temp,S1Mesh[i][j].getyMax()-sphere1.getYVert())/
												-S1Mesh[i][j].getxMin();
								A1=0.5*test_Xlen*testHeight;
								test_Xlen2=FindSpherePoint(sphere1.getRadius(),temp,S1Mesh[i][j].getyMax()-sphere1.getYVert())/
												-S1Mesh[i][j].getxMax();
								A2=0.5*test_Xlen2*testHeight;
								test_Vol=(A1+A2+sqrt(A1*A2))/3;
							}
							else{
								test_Xlen=FindSpherePoint(sphere1.getRadius(),temp,S1Mesh[i][j].getyMin()-sphere1.getYVert())/
												-S1Mesh[i][j].getxMin();
								A1=0.5*test_Xlen*testHeight;
								test_Xlen2=FindSpherePoint(sphere1.getRadius(),temp,S1Mesh[i][j].getyMin()-sphere1.getYVert())/
												-S1Mesh[i][j].getxMax();
								A2=0.5*test_Xlen2*testHeight;
								test_Vol=(A1+A2+sqrt(A1*A2))/3;
							}

							if (test_Vol < 0.05*Total_Vol){
								//Too small of a volume
								continue;
							} else{
								n++;
								test_elvs[n]=temp; 
							}						
						}
					} else {
						n++;
						test_elvs[n]=temp; 
					}
				}
				



// **************** Start of printing function  ********************************************8
// This section does two things.
// 1) Is calculates the z starting, middle and ending elevations
// 2) It prints the part definition as understood by RAMA.  
//  Current assumptions:  This assumes that the vertex is at the center of the scintillator cell
//                        this must be modified later.	

	
				//Test output (used for printing function)
				//cout<<i<<", "<<j<<", "<<S1Mesh[i][j].getxMin()<<", "<<S1Mesh[i][j].getxMax()<<", ";
				//cout<<S1Mesh[i][j].getyMin()<<", "<<S1Mesh[i][j].getyMax()<<", "<<z_top<<", "<<z_Sph;
				//cout<<", "<<z_numb_INT<<", "<<S1Mesh[i][j].getPT()<<endl<<endl;


			} else {} //part is null do nothing	
		}
	}
	return 0;
}


double FindSpherePoint(double r, double x_loc, double y_loc) {
	double Point=sqrt( pow(r,2) - pow(x_loc,2) - pow(y_loc,2) );
	return Point;
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


void bubbleSort(double arr[], int n) {
      bool swapped = true;
      int j = 0;
      double tmp;
      while (swapped) {
            swapped = false;
            j++;
            for (int i = 0; i < n - j; i++) {
                  if (arr[i] > arr[i + 1]) {
                        tmp = arr[i];
                        arr[i] = arr[i + 1];
                        arr[i + 1] = tmp;
                        swapped = true;
                  }
            }
      }
}

