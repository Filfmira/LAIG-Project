#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>
#include <sstream>
class Global{

private:
	//drawing
	char* mode;
	char* shading;
    float background[4];
	//culling
	char* face;
	char* order;

	//lighting
	bool doublesided;
	bool local;
	bool enabled;
	float ambient[4];

public:
	Global(){};
	Global(char* mode, char*shading,float background[4], char* face, char* order, bool doublesided, bool local, bool enabled, float ambient[4])
	{
		this->mode=mode;
		this->shading=shading;
		this->background[0]=background[0];
		this->background[1]=background[1];
		this->background[2]=background[2];
		this->background[3]=background[3];
		this->face=face;
		this->order=order;
		this->doublesided=doublesided;
		this->local=local;
		this->enabled=enabled;
		this->ambient[0]=ambient[0];
		this->ambient[1]=ambient[1];
		this->ambient[2]=ambient[2];
		this->ambient[3]=ambient[3];
	}

	char* getMode(){return this->mode;}
	void setMode(char* mode){ this->mode=mode;}

	char* getShading(){return this->shading;}
	void setShading(char* shading){ this->shading=shading;}

	float* getBackground(){return this->background;}
	void setBackground(float background[4])
		{ this->background[0]=background[0];
		this->background[1]=background[1];
		this->background[2]=background[2];
		this->background[3]=background[3];}

	char* getFace(){return this->face;}
	void setFace(char* face){ this->face=face;}

	char* getOrder(){return this->order;}
	void setOrder(char* order){ this->order=order;}

	bool getDoublesided(){return this->doublesided;}
	void setDoublesided(bool doublesided){ this->doublesided=doublesided;}

	bool getLocal(){return this->local;}
	void setLocal(bool local){ this->local=local;}

	bool getEnabled(){return this->enabled;}
	void setEnabled(bool enabled){ this->enabled=enabled;}

	float* getAmbient(){return this->ambient;}
	void setAmbient(float ambient[4])
	{
		this->ambient[0]=ambient[0];
		this->ambient[1]=ambient[1];
		this->ambient[2]=ambient[2];
		this->ambient[3]=ambient[3];
	}


	string printGlobals(){
		std::ostringstream s;
		s<<"Globals:: ";
		s<<mode<<" "<<endl;
		return s.str();
	}
};



#endif