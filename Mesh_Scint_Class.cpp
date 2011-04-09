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


void bubbleSort(double arr[], int n);
double FindSpherePoint(double r, double x_loc, double y_loc);
void PrintPart(int i, int j,vector < vector<MeshScintillator > >& S1Mesh, vector <double>& z_elvs, int mesh_numb, Scintillator s, Sphereisect sphere1);
void DefineIsects(int i, int j,vector< vector<MeshScintillator > >& S1Mesh, Sphereisect sphere1, double z_isect[]);
double FindTetraVol(double sorted_isect[],double z_isect[],int i, int j,vector < vector<MeshScintillator > >& S1Mesh,Sphereisect \
							 sphere1,double temp, double testHeight);
double FindFrustumTetra(double sorted_isect[],double z_isect[],int i, int j,vector < vector<MeshScintillator > >& S1Mesh,Sphereisect \
								 sphere1,double temp, double testHeight);

int main () {
	Scintillator scint1;
	Sphereisect sphere1;
    /* 
        Scintillator scint = new Scintillator();
        
        ScintillatorRegionMesh mesh = new ScintillatorRegionMesh(scint);
        
        ScintellatorSphereIntersection* isect = new ScintellatorSphereIntersection(scint1, sphere1);
        
        MergedRegionScintellatorView* view =  new MergedRegionScintellatorView(isect);
        
        vector<MeshRegion> regions = view.getMeshRegions();
        for (int i = 0; i < regions.size(); i++) {
            geoFile << regions[i]->toGeo();
            gatFile << regions[i]->toGat();
            latFile << regions[i]->toLat();
        }
    */

	return 0;
}

double FindFrustumTetra(double sorted_isect[],double z_isect[],int i, int j,vector < vector<MeshScintillator > >& S1Mesh,Sphereisect  sphere1,double temp, double testHeight){

	double A1,A2,test_Xlen,test_Xlen2,test_Ylen,test_Ylen2,test_Vol;
	if(sqrt(abs(pow(sphere1.getRadius(),2)-pow(temp-sphere1.getZVert(),2)))<=abs(S1Mesh[i][j].getxMax())){
		test_Xlen=abs(S1Mesh[i][j].getxMax())-abs(FindSpherePoint(sphere1.getRadius(),temp-sphere1.getZVert()\
						,S1Mesh[i][j].getyMax()-sphere1.getYVert())+sphere1.getXVert());
		A1=0.5*test_Xlen*testHeight;
		test_Xlen2=abs(S1Mesh[i][j].getxMax())-abs(FindSpherePoint(sphere1.getRadius(),temp-sphere1.getZVert()\
						,S1Mesh[i][j].getyMin()-sphere1.getYVert())+sphere1.getXVert());
		A2=0.5*test_Xlen2*testHeight;
		test_Vol=(A1+A2+sqrt(A1*A2))/3;
		cout<<"line 264, Ylen= "<<test_Ylen<<", Ylen2= "<<test_Ylen2<<", Zlen= "<<testHeight<<", ";
	}
	else if (sqrt(abs(pow(sphere1.getRadius(),2)-pow(temp-sphere1.getZVert(),2)))<=abs(S1Mesh[i][j].getxMin())){
		test_Xlen=abs(S1Mesh[i][j].getxMin())-abs(FindSpherePoint(sphere1.getRadius(),temp-sphere1.getZVert()\
						,S1Mesh[i][j].getyMax()-sphere1.getYVert())+sphere1.getXVert());
		A1=0.5*test_Xlen*testHeight;
		test_Xlen2=abs(S1Mesh[i][j].getxMin())-abs(FindSpherePoint(sphere1.getRadius(),temp-sphere1.getZVert()\
						,S1Mesh[i][j].getyMin()-sphere1.getYVert())+sphere1.getXVert());
		A2=0.5*test_Xlen2*testHeight;
		test_Vol=(A1+A2+sqrt(A1*A2))/3;
		cout<<"line 272, Ylen= "<<test_Ylen<<", Ylen2= "<<test_Ylen2<<", Zlen= "<<testHeight<<", ";
	}
	else if (sqrt(abs(pow(sphere1.getRadius(),2)-pow(temp-sphere1.getZVert(),2)))<=abs(S1Mesh[i][j].getyMax())){
		test_Ylen=abs(S1Mesh[i][j].getyMax())-abs(FindSpherePoint(sphere1.getRadius(),temp-sphere1.getZVert()\
						,S1Mesh[i][j].getxMin()-sphere1.getXVert())+sphere1.getYVert());
		A1=0.5*test_Ylen*testHeight;
		test_Ylen2=abs(S1Mesh[i][j].getyMax())-abs(FindSpherePoint(sphere1.getRadius(),temp-sphere1.getZVert()\
						,S1Mesh[i][j].getxMax()-sphere1.getXVert())+sphere1.getYVert());
		A2=0.5*test_Ylen2*testHeight;
		test_Vol=(A1+A2+sqrt(A1*A2))/3;
		cout<<"line 280, Ylen= "<<test_Ylen<<", Ylen2= "<<test_Ylen2<<", Zlen= "<<testHeight<<", ";
	}
	else{
		test_Ylen=abs(S1Mesh[i][j].getyMin())-abs(FindSpherePoint(sphere1.getRadius(),temp-sphere1.getZVert()\
						,S1Mesh[i][j].getxMax()-sphere1.getXVert())+sphere1.getYVert());
		A1=0.5*test_Ylen*testHeight;
		test_Ylen2=abs(S1Mesh[i][j].getyMin())-abs(FindSpherePoint(sphere1.getRadius(),temp-sphere1.getZVert()\
						,S1Mesh[i][j].getxMin()-sphere1.getXVert())+sphere1.getYVert());
		A2=0.5*test_Ylen2*testHeight;
		test_Vol=(A1+A2+sqrt(A1*A2))/3;
		cout<<"line 288, Ylen= "<<test_Ylen<<", Ylen2= "<<test_Ylen2<<", Zlen= "<<testHeight<<", ";
	}
	return test_Vol;
} 


double FindTetraVol(double sorted_isect[],double z_isect[],int i, int j,vector < vector<MeshScintillator > >& S1Mesh,Sphereisect  sphere1,double temp, double testHeight){
	double test_Xlen,test_Ylen, test_Vol;
	if(sorted_isect[0]==z_isect[0]){
		test_Xlen=abs(S1Mesh[i][j].getxMax())-abs(FindSpherePoint(sphere1.getRadius(),temp-sphere1.getZVert()\
						,S1Mesh[i][j].getyMax()-sphere1.getYVert())+sphere1.getXVert());
		test_Ylen=abs(S1Mesh[i][j].getyMax())-abs(FindSpherePoint(sphere1.getRadius(),temp-sphere1.getZVert()\
						,S1Mesh[i][j].getxMax()-sphere1.getXVert())+sphere1.getYVert());		 
		test_Vol=testHeight*test_Xlen*test_Ylen/6;
	}
	else if (sorted_isect[0]==z_isect[1]){
		test_Xlen=abs(S1Mesh[i][j].getxMin())-abs(FindSpherePoint(sphere1.getRadius(),temp-sphere1.getZVert()\
						,S1Mesh[i][j].getyMax()-sphere1.getYVert())+sphere1.getXVert());
		test_Ylen=abs(S1Mesh[i][j].getyMax())-abs(FindSpherePoint(sphere1.getRadius(),temp-sphere1.getZVert()\
						,S1Mesh[i][j].getxMin()-sphere1.getXVert())+sphere1.getYVert());
		test_Vol=testHeight*test_Xlen*test_Ylen/6;
	}
	else if (sorted_isect[0]==z_isect[2]){
		test_Xlen=abs(S1Mesh[i][j].getxMax())-abs(FindSpherePoint(sphere1.getRadius(),temp-sphere1.getZVert()\
						,S1Mesh[i][j].getyMin()-sphere1.getYVert())+sphere1.getXVert());
		test_Ylen=abs(S1Mesh[i][j].getyMin())-abs(FindSpherePoint(sphere1.getRadius(),temp-sphere1.getZVert()\
						,S1Mesh[i][j].getxMax()-sphere1.getXVert())+sphere1.getYVert());
		test_Vol=testHeight*test_Xlen*test_Ylen/6;
	}
	else{
		test_Xlen=abs(S1Mesh[i][j].getxMin())-abs(FindSpherePoint(sphere1.getRadius(),temp-sphere1.getZVert()\
						,S1Mesh[i][j].getyMin()-sphere1.getYVert())+sphere1.getXVert());					 
		test_Ylen=abs(S1Mesh[i][j].getyMin())-abs(FindSpherePoint(sphere1.getRadius(),temp-sphere1.getZVert()\
						,S1Mesh[i][j].getxMin()-sphere1.getXVert())+sphere1.getYVert());
		test_Vol=testHeight*test_Xlen*test_Ylen/6;
	}
	return test_Vol;
}

void PrintPart(int i, int j,vector < vector<MeshScintillator > >& S1Mesh, vector <double>& z_elvs, int mesh_numb, Scintillator s, Sphereisect sphere1){
	
	ofstream GEOblock, LATblock, GATblock;
	
	if (S1Mesh[i][j].getPT()==RPP_Isect){
		//GEO block	
		GEOblock.open ("GEOinp.txt", ios::out | ios::app );
		if (GEOblock.is_open()) { 
			GEOblock<<"'"<<s.getName()<<"_"<<i+1<<j+1<<"'  'ag_RPPSPH10_"<<mesh_numb<<"' /"<<endl;
                        GEOblock<<sphere1.getXVert()<<", "<<sphere1.getYVert()<<", "<<sphere1.getZVert()<<", "<<sphere1.getRadius()<<", ";
			GEOblock<<S1Mesh[i][j].getxMin()<<", "<<S1Mesh[i][j].getxMax();
			GEOblock<<", "<<S1Mesh[i][j].getyMin()<<", "<<S1Mesh[i][j].getyMax()<<", ";
                        GEOblock<<z_elvs[0];
			for (int tmp=mesh_numb; tmp>=1; tmp--){
				GEOblock<<", "<<z_elvs[tmp]-z_elvs[0];
			}
			GEOblock<<" /"<<endl<<"'NULL' /"<<endl;
		} else {
			cout<<"Error in opening GEOinp.txt file"<<endl;
		}
		GEOblock.close();
	
		//LAT block
		LATblock.open ("LATinp.txt", ios::out | ios::app );
		if (LATblock.is_open()) { 
			LATblock<<"&"<<s.getName()<<"-"<<i+1<<"x"<<j+1<<"_N01:1"<<" '"<<s.getName()<<"_"<<i+1<<j+1<<"' /"<<endl;
			LATblock<<"0,0,0,0 /"<<endl; //#!# Add in sphere element
		} else { 		
			cout<<"Error in opening LATinp.txt file"<<endl;
		}
		LATblock.close();

		//GAT block
		GATblock.open ("GATinp.txt", ios::out | ios::app );
		if (GATblock.is_open()) { 
			for (int tmp=1; tmp<=mesh_numb; ++tmp){
				GATblock<<"'"<<s.getName()<<"-"<<i+1<<"x"<<j+1<<"_N01:1."<<tmp<<"'  1=scint 2=2.0 10=0 /"<<endl;
			}
		} else { 		
			cout<<"Error in opening GATinp.txt file"<<endl;
		}
		GATblock.close();
	} else if (S1Mesh[i][j].getPT()==RPP_Plain){
		
		//GEO block	
		GEOblock.open ("GEOinp.txt", ios::out | ios::app );
		if (GEOblock.is_open()) { 
			GEOblock<<"'"<<s.getName()<<"_"<<i+1<<j+1<<"'  'ag_rpp10' /"<<endl;
			GEOblock<<S1Mesh[i][j].getxMin()<<", "<<S1Mesh[i][j].getxMax();
			GEOblock<<", "<<S1Mesh[i][j].getyMin()<<", "<<S1Mesh[i][j].getyMax()<<", ";
			GEOblock<<z_elvs[0]<<", "<<z_elvs[1]-z_elvs[0]<<" /"<<endl;
			GEOblock<<"'NULL' /"<<endl;
		} else {
			cout<<"Error in opening GEOinp.txt file"<<endl;
		}
		GEOblock.close();
	
		//LAT block
		LATblock.open ("LATinp.txt", ios::out | ios::app );
		if (LATblock.is_open()) { 
			LATblock<<"&"<<s.getName()<<"-"<<i+1<<"x"<<j+1<<"_N01:1-"<<mesh_numb<<" '"<<s.getName()<<"_"<<i+1<<j+1<<"' /"<<endl;
			LATblock<<"0,0,0,0,"<<endl;
			for (int tmp=1;tmp<mesh_numb; ++tmp){
				LATblock<<"0,0,"<<z_elvs[tmp]<<",0";
				if (tmp+1 != mesh_numb){
					LATblock<<","<<endl; //#!# Add in sphere element
				}
			}
			LATblock<<" /"<<endl;
		} else { 		
			cout<<"Error in opening LATinp.txt file"<<endl;
		}
		LATblock.close();

		//GAT block
		GATblock.open ("GATinp.txt", ios::out | ios::app );
		if (GATblock.is_open()) { 
			for (int tmp=1; tmp<=mesh_numb; ++tmp){
				GATblock<<"'"<<s.getName()<<"-"<<i+1<<"x"<<j+1<<"_N01:"<<tmp<<".1'  1=scint 2=2.0 10=0 /"<<endl;
			}
		} else { 		
			cout<<"Error in opening GATinp.txt file"<<endl;
		}
		GATblock.close();

	} else if (S1Mesh[i][j].getPT()==Tri_Isect){
		double Xvect,Yvect;
		//GEO block	
		GEOblock.open ("GEOinp.txt", ios::out | ios::app );
		if (GEOblock.is_open()) { 
			for(int tmp=0; tmp<mesh_numb; ++tmp){			
				GEOblock<<"'"<<s.getName()<<"_"<<i+1<<j+1<<"_"<<tmp+1<<"'  'ag_sphraw10' /"<<endl;//#!# Add in sphere element
				GEOblock<<"0.,0."<<s.getZVert()<<", "<<z_elvs[tmp+1]-z_elvs[tmp]<<", ";	//#!# get mesh height issue resolved, update accordingly	
				if ( j+1 < s.getYMesh() ) {
					Xvect=-1*(S1Mesh[i][j].getxMax()-S1Mesh[i][j].getxMin());
					GEOblock<<S1Mesh[i][j].getxMax()<<", ";
				} else {
					Xvect=(S1Mesh[i][j].getxMax()-S1Mesh[i][j].getxMin());
					GEOblock<<S1Mesh[i][j].getxMin()<<", ";
				}
	
				if ( i+1 <= s.getYMesh()/2 ) {
					Yvect=(S1Mesh[i][j].getyMax()-S1Mesh[i][j].getyMin());
					GEOblock<<S1Mesh[i][j].getyMin()<<", ";
				} else {
					Yvect=-1*(S1Mesh[i][j].getyMax()-S1Mesh[i][j].getyMin());
					GEOblock<<S1Mesh[i][j].getyMax()<<", ";
				}
				GEOblock<<", "<<Xvect<<", 0., 0."<<Yvect<<" /"<<endl;
				GEOblock<<"'NULL' /"<<endl;
			}
		} else {
			cout<<"Error in opening GEOinp.txt file"<<endl;
		}
		GEOblock.close();
	
		//LAT block
		LATblock.open ("LATinp.txt", ios::out | ios::app );
		if (LATblock.is_open()) { 
			for(int tmp=0; tmp<mesh_numb; ++tmp){		
				LATblock<<"&"<<s.getName()<<"-"<<i+1<<"x"<<j+1<<"_"<<tmp+1<<"_N01:1"<<" '"<<s.getName()<<"_"<<i+1<<j+1<<"_"<<tmp+1<<"' /"<<endl;
				LATblock<<"0,0,0,0 /"<<endl; 
			}
		} else { 		
			cout<<"Error in opening LATinp.txt file"<<endl;
		}
		LATblock.close();

		//GAT block
		GATblock.open ("GATinp.txt", ios::out | ios::app );
		if (GATblock.is_open()) { 
			for (int tmp=0; tmp<mesh_numb; ++tmp){
				GATblock<<"'"<<s.getName()<<"-"<<i+1<<"x"<<j+1<<"_"<<tmp+1<<"_N01:1."<<tmp<<"'  1=scint 2=2.0 10=0 /"<<endl;
			}
		} else { 		
			cout<<"Error in opening GATinp.txt file"<<endl;
		}
		GATblock.close();  
		//cout<<"Add Tri_Isect PArt!!!!) for region "<<i+1<<j+1<<endl;
	} else{
		double Xvect,Yvect;
		//GEO block	
		GEOblock.open ("GEOinp.txt", ios::out | ios::app );
		if (GEOblock.is_open()) { 
			for(int tmp=0; tmp<mesh_numb; ++tmp){			
				GEOblock<<"'"<<s.getName()<<"_"<<i+1<<j+1<<"_"<<tmp+1<<"'  'ag_raw10' /"<<endl;
				GEOblock<<"0.,0., "<<z_elvs[tmp]+s.getZVert()<<", "<<z_elvs[tmp+1]-z_elvs[tmp]<<", ";	//#!# get mesh height issue resolved, update accordingly	
				if ( j+1 < s.getYMesh() ) {
					Xvect=-1*(S1Mesh[i][j].getxMax()-S1Mesh[i][j].getxMin());
					GEOblock<<S1Mesh[i][j].getxMax()<<", ";
				} else {
					Xvect=(S1Mesh[i][j].getxMax()-S1Mesh[i][j].getxMin());
					GEOblock<<S1Mesh[i][j].getxMin()<<", ";
				}
	
				if ( i+1 <= s.getYMesh()/2 ) {
					Yvect=(S1Mesh[i][j].getyMax()-S1Mesh[i][j].getyMin());
					GEOblock<<S1Mesh[i][j].getyMin()<<", ";
				} else {
					Yvect=-1*(S1Mesh[i][j].getyMax()-S1Mesh[i][j].getyMin());
					GEOblock<<S1Mesh[i][j].getyMax()<<", ";
				}
				GEOblock<<Xvect<<", 0., 0., "<<Yvect<<" /"<<endl;
				GEOblock<<"'NULL' /"<<endl;
			}
		} else {
			cout<<"Error in opening GEOinp.txt file"<<endl;
		}
		GEOblock.close();
	
		//LAT block
		LATblock.open ("LATinp.txt", ios::out | ios::app );
		if (LATblock.is_open()) { 
			for(int tmp=0; tmp<mesh_numb; ++tmp){		
				LATblock<<"&"<<s.getName()<<"-"<<i+1<<"x"<<j+1<<"_"<<tmp+1<<"_N01:1"<<" '"<<s.getName()<<"_"<<i+1<<j+1<<"_"<<tmp+1<<"' /"<<endl;
				LATblock<<"0,0,0,0 /"<<endl; //#!# Add in sphere element
			}
		} else { 		
			cout<<"Error in opening LATinp.txt file"<<endl;
		}
		LATblock.close();

		//GAT block
		GATblock.open ("GATinp.txt", ios::out | ios::app );
		if (GATblock.is_open()) { 
			for (int tmp=0; tmp<mesh_numb; ++tmp){
				GATblock<<"'"<<s.getName()<<"-"<<i+1<<"x"<<j+1<<"_"<<tmp+1<<"_N01:1.1'  1=scint 2=2.0 10=0 /"<<endl;
			}
		} else { 		
			cout<<"Error in opening GATinp.txt file"<<endl;
		}
		GATblock.close(); 
		//cout<<"Add Tri_Plain PArt!!!!) for region "<<i+1<<j+1<<endl;
	}
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

