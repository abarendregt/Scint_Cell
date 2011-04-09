#ifndef SPHEREISECT_H
#define SPHEREISECT_H

class ScintillatorSphereIntersection {
    Scintillator scintillator;
    Sphere spehere;
    
    Scintillator::Scintillator(Schintillator scint, Sphere sph) {
        this->scint = scint;
    	this->sphere = sph;
    }
    

    vector<vector<MeshRegion>> Scintillator::getMeshRegions() {
        int maxRowSize = scintillator.getMaxRowSize();
        int maxColSize = scintillator.getMaxColumnSize();
        
        vector<vector<MeshRegion>> meshRegions = scintillator.getMeshRegions();;
    	
    	Trangloid[] ts = scintellator.getTriangloids()
        for(i = 0; i <= ts.size(); i++) {
            Trangloid t = ts[i];
            if (isIntersected(t.getXLoc, t.getYLoc)) {
				meshRegions[i][j] = new IntersectedTrianguloid();
            }
        }
        
        RectangularParallelPiped[] rpps = scintellator.getRectangularParallelpiped()
        for(i = 0; i <= rpps.size(); i++) {
            RectangularParallelPiped r = rpps[i];
            if (isIntersected(r.getXLoc, r.getYLoc)) {
				meshRegions[i][j] = new IntersectedRectangularParallelpiped();
            }
        }
        
        return meshRegions;
    }
    
    
    void isIntersected(int i, int j){
    	if (sphere1.getRadius() > sqrt( pow(S1Mesh[i][j].getxMin()-sphere1.getXVert(),2) + pow(S1Mesh[i][j].getyMin()  \
    			-sphere1.getYVert(),2) + pow(scint1.getZVert()-sphere1.getZVert(),2) ) ) {
    		return true;
    	} else if (sphere1.getRadius() > sqrt( pow(S1Mesh[i][j].getxMax()-sphere1.getXVert(),2) + pow(S1Mesh[i][j].getyMin() \
    			-sphere1.getYVert(),2)  + pow(scint1.getZVert()-sphere1.getZVert(),2) ) ) {
    		return true;
    	} else if (sphere1.getRadius() > sqrt( pow(S1Mesh[i][j].getxMin()-sphere1.getXVert(),2) + pow(S1Mesh[i][j].getyMax() \
    			-sphere1.getYVert(),2) + pow(scint1.getZVert()-sphere1.getZVert(),2) ) ) {
    		return true;
    	} else if (sphere1.getRadius() > sqrt( pow(S1Mesh[i][j].getxMax()-sphere1.getXVert(),2) + pow(S1Mesh[i][j].getyMax() \
    			-sphere1.getYVert(),2) + pow(scint1.getZVert()-sphere1.getZVert(),2) ) ) {
    		return true;
    	}
        return false;
    }
    
};
#endif
