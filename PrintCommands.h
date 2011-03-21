#ifndef PRINTCMD_H
#define PRINTCMD_H
using namespace std;


class PrintCmd {
	bool GEOPrinted,RPP10Printed,LATPrinted,GATPrinted;
        
	public:
	PrintCmd();
	PrintCmd(bool GEOPrinted,bool RPP10Printed,bool LATPrinted,bool GATPrinted);
	void setRPP10(bool RPP) {this->RPP10Printed=RPP;}
	void setGEO(bool GEO) {this->GEOPrinted=GEO;}
	void setLAT(bool LAT) {this->LATPrinted=LAT;}
	void setGAT(bool GAT) {this->GATPrinted=GAT;}	
	
	bool getRPP10(void) {return this->RPP10Printed;}
	bool getGEO(void) {return this->GEOPrinted;}
	bool getLAT(void) {return this->LATPrinted;}
	bool getGAT(void) {return this->GATPrinted;}
};

PrintCmd::PrintCmd(){
	
	this->setGEO(0);
	this->setRPP10(0);
	this->setLAT(0);
	this->setGAT(0);
}

PrintCmd::PrintCmd(bool GEOPrinted,bool RPP10Printed,bool LATPrinted,bool GATPrinted){
	
	this->setGEO(GEOPrinted);
	this->setRPP10(RPP10Printed);
	this->setLAT(LATPrinted);
	this->setGAT(GATPrinted);

}




#endif
