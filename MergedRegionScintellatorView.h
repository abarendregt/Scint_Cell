class MergedRegionScintellatorView {
    vector<MeshedRegion> getMeshedRegions() {
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
        				PrintPart(i,j,S1Mesh,z_elvs,scint1.getZMesh(), scint1, sphere1); 
        			} 
        			else if ((S1Mesh[i][j].getPT() == Tri_Isect) || (S1Mesh[i][j].getPT() == RPP_Isect)) {  

        				DefineIsects(i, j, S1Mesh, sphere1, z_isect);
        				if ((z_isect[0]!=z_isect[0])||(z_isect[1]!=z_isect[1])||(z_isect[2]!=z_isect[2])||(z_isect[3]!=z_isect[3])){
        					cout<<"line 106"<<endl;
        					PrintPart(i,j,S1Mesh,z_elvs,scint1.getZMesh(), scint1, sphere1); 	
        					continue; // No voided regions. Print TRI_isect or RPP_isect
        				}  
        				if 
        				((z_isect[0]<0)||(z_isect[1]<0)||(z_isect[2]<0)||(z_isect[3]<0)){
        					cout<<"line 114"<<endl;
        					PrintPart(i,j,S1Mesh,z_elvs,scint1.getZMesh(), scint1, sphere1);
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
        					//cout<<"line 130, "<<S1Mesh[i][j].getxMin()<<", "<<S1Mesh[i][j].getyMin()<<", "<<z_numb<<", "<<top_isect<<", "<<z_top<<endl;
        					test_elvs.resize(z_numb);
        					for (int n=z_numb;n>=0;--n){
        						//cout<<"OK_n="<<n<<endl;
        						test_elvs[n]=temp;
        						temp= temp - Mesh_Height;
        					}
        					//cout<<"OK_2"<<endl;
        					test_top=test_elvs[0];
        					//cout<<"line 137"<<endl;
        					PrintPart(i,j,S1Mesh,test_elvs,z_numb, scint1, sphere1); 
        				} 
        				else{
        					//cout<<"line 141, "<<i<<", "<<j<<", "<<z_numb<<", "<<top_isect<<", "<<z_top<<endl;
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
        				double Total_Vol=abs(Mesh_Height*(S1Mesh[i][j].getyMax()-S1Mesh[i][j].getyMin())*(S1Mesh[i][j].getxMax()-S1Mesh[i][j].getxMin()));
        				double testHeight,test_Vol;

        				while (temp < test_top) {
        					cout<<temp<<", "<<S1Mesh[i][j].getxMin()<<", "<<S1Mesh[i][j].getyMin()<<", "<<test_top<<", "<<bot_isect<<endl;
        					temp = temp + Mesh_Height;
        					if (temp < sorted_isect[2]){
        						if (temp < sorted_isect[1]){
        							// test using volume of tetrahedron
        							testHeight=temp-bot_isect;
        							test_Vol=FindTetraVol(sorted_isect,z_isect,i,j,S1Mesh,sphere1,temp, testHeight); 

        							if (test_Vol < 0.05*Total_Vol){
        								cout<<"Too small of a volume. Tetra_Vol= "<<test_Vol<<", Total_Vol= "<<Total_Vol<<endl;
        								continue; //Too small of a volume
        							} else{
        								n++;
        								test_elvs[n]=temp; 
        								cout<<"line182 n="<<n<<", Tetra_Vol= "<<test_Vol<<", Total_Vol= "<<Total_Vol<<endl;
        							}
        						} else {
        							// test using frustrum of tetrahedron
        							testHeight=temp-bot_isect;
        							test_Vol=FindFrustumTetra(sorted_isect,z_isect,i,j,S1Mesh,sphere1,temp, testHeight); 

        							if (test_Vol < 0.05*Total_Vol){
        								cout<<"Too small of a volume. Frustum_Vol= "<<test_Vol<<", Total_Vol= "<<Total_Vol<<endl;
        								continue; //Too small of a volume
        							} else{
        								n++;
        								test_elvs[n]=temp; 
        								cout<<"line194 n="<<n<<". Frustum_Vol=  "<<test_Vol<<", Total_Vol= "<<Total_Vol<<endl;
        							}						
        						}
        					} else {
        						n++;
        						test_elvs[n]=temp; 
        						//cout<<"line199 n="<<n<<endl;
        					}
        				}
        				//cout<<"line 266"<<endl;
        				PrintPart(i,j,S1Mesh,test_elvs,n, scint1, sphere1); 

        			} else {	//part is null do nothing	
        			} 
        		}
        	}
        
    }
}