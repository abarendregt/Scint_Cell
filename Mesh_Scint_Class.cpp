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
#include "PrintCommands.h"
using namespace std;


void SetMeshBoundary(int Row, int Col,vector< vector<MeshScintillator > >& S1Mesh, Scintillator s);
void AssignTriPT(int i, int j,vector< vector<MeshScintillator > >& S1Mesh, Scintillator scint1, Sphereisect sphere1);
void AssignRppPT(int i, int j,vector< vector<MeshScintillator > >& S1Mesh, Scintillator scint1, Sphereisect sphere1);
void bubbleSort(double arr[], int n);
double FindSpherePoint(double r, double x_loc, double y_loc);
void PrintFullPart(int i, int j,vector < vector<MeshScintillator > >& S1Mesh, vector <double>& z_elvs, int mesh_numb, string PartName);
void PrintIsectPart(int i, int j,vector < vector<MeshScintillator > >& S1Mesh, vector <double>& z_elvs, int mesh_numb, string PartName);
void DefineIsects(int i, int j,vector< vector<MeshScintillator > >& S1Mesh, Sphereisect sphere1, double z_isect[]);
double FindTetraVol(double sorted_isect[],double z_isect[],int i, int j,vector < vector<MeshScintillator > >& S1Mesh,Sphereisect \
							 sphere1,double temp, double testHeight);
double FindFrustumTetra(double sorted_isect[],double z_isect[],int i, int j,vector < vector<MeshScintillator > >& S1Mesh,Sphereisect \
								 sphere1,double temp, double testHeight);

int main () {
  Scintillator scint1;
  Sphereisect sphere1;

// Analysis variables to define
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
	****************************************************************************/ 
	int BlankNumb = scint1.getYMesh()/2-1;  //This is the maximum number of null cells
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

// set up variables to analyze meshes
	double Mesh_Height=scint1.getZLength()/scint1.getZMesh();
	double z_top=scint1.getZLength()+scint1.getZVert();
	double z_isect[4], sorted_isect[4];  //This defines the elevation where the four edges of the RPP or three edges of the TRI intersect 
													 // the sphere
	double z_bottom=scint1.getZVert();
	int z_numb;
	vector <double> z_elvs;
 	z_elvs.resize(scint1.getZMesh()+1);
	
	z_elvs[0]=scint1.getZVert();
	for (i=1;i<scint1.getZMesh()+1;++i){
		z_elvs[i]=z_elvs[i-1]+Mesh_Height;
	}

// Test and print sectors
	for (i=0;i<Row;++i){	
		for (j=0;j<Col;++j) {
			if ((S1Mesh[i][j].getPT() == Tri_Plain) || (S1Mesh[i][j].getPT() == RPP_Plain)){ 		
				//Print TRI_plain or RPP_plain!!!
				cout<<"line 87"<<endl;
				PrintFullPart(i,j,S1Mesh, z_elvs, scint1.getZMesh(), scint1.getName()); 
			} 
			else if ((S1Mesh[i][j].getPT() == Tri_Isect) || (S1Mesh[i][j].getPT() == RPP_Isect)) {  

				DefineIsects(i, j, S1Mesh, sphere1, z_isect);
				if ((z_isect[0]!=z_isect[0])||(z_isect[1]!=z_isect[1])||(z_isect[2]!=z_isect[2])||(z_isect[3]!=z_isect[3])){
					cout<<"line 106"<<endl;
					PrintIsectPart(i,j,S1Mesh,z_elvs,scint1.getZMesh(), scint1.getName()); 	
					continue; // No voided regions. Print TRI_isect or RPP_isect
				}  
				if 
				((z_isect[0]<0)||(z_isect[1]<0)||(z_isect[2]<0)||(z_isect[3]<0)){
					cout<<"line 114"<<endl;
					PrintIsectPart(i,j,S1Mesh,z_elvs,scint1.getZMesh(), scint1.getName());
					continue; // No voided regions. Print TRI_isect or RPP_isect
				}  
			
				for (int k=0;k<4;++k){
					sorted_isect[k]=z_isect[k];
				}
				bubbleSort(sorted_isect, 4);
				//cout<<sorted_isect[0]<<", "<<sorted_isect[1]<<", "<<sorted_isect[2]<<", "<<sorted_isect[3]<<endl;


				double top_isect=sorted_isect[3];
				double bot_isect=sorted_isect[0];
				int z_numb=0;
				vector <double> test_elvs;
				test_elvs.resize(scint1.getZMesh()+1);
				test_elvs[0]=bot_isect;
				//number of full RPPs
				double temp=z_top;
				while (temp > top_isect){		
					temp=temp - Mesh_Height;
					z_numb++;
				}
				double test_top;
				temp=z_top;
				if (z_numb > 1) {
					cout<<"line 130, "<<S1Mesh[i][j].getxMin()<<", "<<S1Mesh[i][j].getyMin()<<", "<<z_numb<<", "<<top_isect<<", "<<z_top<<endl;
					test_elvs.resize(z_numb);
					for (int n=z_numb;n>=0;--n){
						cout<<"OK_n="<<n<<endl;
						test_elvs[n]=temp;
						temp= temp - Mesh_Height;
					}
					cout<<"OK_2"<<endl;
					test_top=test_elvs[0];
					cout<<"line 137"<<endl;
					PrintFullPart(i,j,S1Mesh, test_elvs, z_numb, scint1.getName());
				} 
				else{
					cout<<"line 141, "<<i<<", "<<j<<", "<<z_numb<<", "<<top_isect<<", "<<z_top<<endl;
					test_top=z_top;
				}
				
				//find upper height of voided region
				double test_bottom = z_bottom;
				while (test_bottom < bot_isect-Mesh_Height) {
					test_bottom = test_bottom + Mesh_Height;
				}
				
				z_numb=0;
				temp=test_bottom;
				while (temp < test_top){		
					temp=temp + Mesh_Height;
					z_numb++;
				}
				test_elvs.resize(z_numb);
				// Test intersecting region
				temp=test_bottom;
				test_elvs[0]=test_bottom;
				int n=0;
				double Total_Vol=Mesh_Height*(scint1.getYLength()/scint1.getYMesh())*(scint1.getXLength()/(2*scint1.getYMesh()));
				double testHeight,test_Vol;

				while (temp < test_top) {
					cout<<temp<<", "<<S1Mesh[i][j].getxMin()<<", "<<S1Mesh[i][j].getyMin()<<", "<<test_top<<endl;
					temp = temp + Mesh_Height;
					if (temp < sorted_isect[2]){
						if (temp < sorted_isect[1]){
							// test using volume of tetrahedron
							testHeight=temp-bot_isect;
							test_Vol=FindTetraVol(sorted_isect,z_isect,i,j,S1Mesh,sphere1,temp, testHeight); 
							
							if (test_Vol < 0.05*Total_Vol){
								cout<<"Too small of a volume"<<endl;
								continue; //Too small of a volume
							} else{
								n++;
								test_elvs[n]=temp; 
								cout<<"line182 n="<<n<<endl;
							}
						} else {
							// test using frustrum of tetrahedron
							testHeight=temp-bot_isect;
							test_Vol=FindFrustumTetra(sorted_isect,z_isect,i,j,S1Mesh,sphere1,temp, testHeight); 

							if (test_Vol < 0.05*Total_Vol){
								cout<<"Too small of a volume"<<endl;
								continue; //Too small of a volume
							} else{
								n++;
								test_elvs[n]=temp; 
								cout<<"line194 n="<<n<<endl;
							}						
						}
					} else {
						n++;
						test_elvs[n]=temp; 
						cout<<"line199 n="<<n<<endl;
					}
				}
				cout<<"line 266"<<endl;
				PrintIsectPart(i,j,S1Mesh,test_elvs,n, scint1.getName()); 

			} else {	//part is null do nothing	
			} 
		}
	}

	return 0;
}

double FindFrustumTetra(double sorted_isect[],double z_isect[],int i, int j,vector < vector<MeshScintillator > >& S1Mesh,Sphereisect  sphere1,double temp, double testHeight){

	double A1,A2,test_Xlen,test_Xlen2,test_Ylen,test_Ylen2,test_Vol;
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
	return test_Vol;
} 


double FindTetraVol(double sorted_isect[],double z_isect[],int i, int j,vector < vector<MeshScintillator > >& S1Mesh,Sphereisect  sphere1,double temp, double testHeight){
	double test_Xlen,test_Ylen, test_Vol;
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
	return test_Vol;
}

void PrintIsectPart(int i, int j,vector < vector<MeshScintillator > >& S1Mesh, vector <double>& z_elvs, int mesh_numb, string PartName){
	
	ofstream GEOblock, LATblock, GATblock;
	
	if (S1Mesh[i][j].getPT()==RPP_Isect){
		//GEO block	
		GEOblock.open ("GEOinp.txt", ios::out | ios::app );
		if (GEOblock.is_open()) { 
			GEOblock<<"'"<<PartName<<"_"<<i+1<<j+1<<"'  'ag_sphrpp10' /"<<endl;
			GEOblock<<S1Mesh[i][j].getxMin()<<", "<<S1Mesh[i][j].getxMax();
			GEOblock<<", "<<S1Mesh[i][j].getyMin()<<", "<<S1Mesh[i][j].getyMax()<<", ";
			for (int tmp=mesh_numb; tmp>=1; tmp--){
				GEOblock<<z_elvs[tmp]<<", ";
			}
			GEOblock<<z_elvs[0]<<" /"<<endl;
			GEOblock<<"'NULL' /"<<endl;
		} else {
			cout<<"Error in opening GEOinp.txt file"<<endl;
		}
		GEOblock.close();
	
		//LAT block
		LATblock.open ("LATinp.txt", ios::out | ios::app );
		if (LATblock.is_open()) { 
			LATblock<<"&"<<PartName<<"-"<<i+1<<"x"<<j+1<<"_N01:1"<<" '"<<PartName<<"_"<<i+1<<j+1<<"' /"<<endl;
			LATblock<<"0,0,0,0 /"<<endl; //#!# Add in sphere element
		} else { 		
			cout<<"Error in opening LATinp.txt file"<<endl;
		}
		LATblock.close();

		//GAT block
		GATblock.open ("GATinp.txt", ios::out | ios::app );
		if (GATblock.is_open()) { 
			for (int tmp=0; tmp>=mesh_numb; ++tmp){
				GATblock<<"'"<<PartName<<"-"<<i+1<<"x"<<j+1<<"_N01:1."<<tmp<<"'  1=scint 2=2.0 10=0 /"<<endl;
			}
		} else { 		
			cout<<"Error in opening GATinp.txt file"<<endl;
		}
		GATblock.close();
	}
}

void PrintFullPart(int i, int j,vector < vector<MeshScintillator > >& S1Mesh, vector <double>& z_elvs, int mesh_numb, string PartName){
	
//	ofstream GEOblock, LATblock, GATblock;
//	GEOblock.open ("GEOinp.txt", ios::out | ios::app );
//	if (GEOblock.is_open()) { 
		cout<<"'"<<PartName<<"_"<<i+1<<j+1<<"'  'ag_rpp10' /"<<endl;
		cout<<S1Mesh[i][j].getxMin()<<", "<<S1Mesh[i][j].getxMax();
		cout<<", "<<S1Mesh[i][j].getyMin()<<", "<<S1Mesh[i][j].getyMax()<<", ";
		for (mesh_numb; mesh_numb>=1; mesh_numb--){
			cout<<z_elvs[mesh_numb]<<", ";
		}
		cout<<z_elvs[0]<<" /"<<endl;
		cout<<"'NULL' /"<<endl;
//	} else {
//		cout<<"Error in opening GEOinp.txt file"<<endl;
//	}
//	GEOblock.close();

	cout<<"LAT"<<endl;
	cout<<"&"<<PartName<<"-"<<i+1<<"x"<<j+1<<"_N01:1"<<" '"<<PartName<<"_"<<i+1<<j+1<<"' /"<<endl<<endl;
}

void DefineIsects(int i, int j,vector< vector<MeshScintillator > >& S1Mesh, Sphereisect sphere1, double z_isect[]){
	z_isect[0]=FindSpherePoint(sphere1.getRadius(),S1Mesh[i][j].getxMax()-sphere1.getXVert(), \
					S1Mesh[i][j].getyMax()-sphere1.getYVert()) +sphere1.getZVert();  //Fix for arrays 1,2 and 3
	z_isect[1]=sqrt( sphere1.getRadius()*sphere1.getRadius() - pow(S1Mesh[i][j].getxMin()-sphere1.getXVert(),2) \
					- pow(S1Mesh[i][j].getyMax()-sphere1.getYVert(),2) ) +sphere1.getZVert();
	z_isect[2]=sqrt( sphere1.getRadius()*sphere1.getRadius() - pow(S1Mesh[i][j].getxMax()-sphere1.getXVert(),2) \
					- pow(S1Mesh[i][j].getyMin()-sphere1.getYVert(),2) ) +sphere1.getZVert();
	z_isect[3]=sqrt( sphere1.getRadius()*sphere1.getRadius() - pow(S1Mesh[i][j].getxMin()-sphere1.getXVert(),2) \
					- pow(S1Mesh[i][j].getyMin()-sphere1.getYVert(),2) ) +sphere1.getZVert();

}

double FindSpherePoint(double r, double dim1, double dim2) {
	double Point=sqrt( pow(r,2) - pow(dim1,2) - pow(dim2,2) );
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

