#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <math.h>
using namespace std;

int main(){

	/* This file is split into three sections.  One to define to GEO block, one to define the LAT block and a third to 
		define the GAT block.  These three sections should be merged into one file at the end of this.  */
	//Define initial inputs
	enum PT {null,Tri_Plain,Tri_Isect,RPP_Plain,RPP_Isect};
	double xmin=-0.465;
	double xmax=0.0;
	double ymin=0.0;
	double ymax=0.805;
	int z_mesh=8;
	double z[]={0.3,0.2625,0.225,0.1875,0.15,0.1125,0.075,0.0375};
	double z_bot=0.0;
	int z_numb=8;  //In case the number of elevations are less than 8, this number will tell this code when to stop 
						// processing the array 'z[]'
	int x_location=0;
	int y_location=3;
	PT PartType=RPP_Plain;
	string PartName="Scint1";
	string Part="ag_rpp10";
	
	//Print GEO block*****************************************************************************************
	//********************************************************************************************************
	// The enum block will define the part type being used.  The part type will decide what parameters will 
	// be printed in the GEO, LAT and GAT block. This section assumes a plain RPP part type.

	// This is a portion of the GEO block.  This is the part name and part type.
	cout<<"'"<<PartName<<"_"<<x_location+1<<y_location+1<<"'  '"<<Part<<"_"<<z_numb<<"' /"<<endl;
	cout<<xmin<<", "<<xmax<<", "<<ymin<<", "<<ymax<<", ";
	for (int i=0; i<z_numb; ++i){
		cout<<z[i]<<", ";
	}
	cout<<z_bot<<" /"<<endl;
	cout<<"'NULL' /"<<endl;
	
	return 0;
}

