#ifndef _XMLSCENEMOD_H_
#define _XMLSCENEMOD_H_

#include "tinyxml.h"
#include "Graph.h"
#include "Lights.h"
#include "Textures.h"
#include "Appearances.h"
#include "Cameras.h"

class XMLSceneMod
{
public:
	XMLSceneMod(char *filename, Graph* gr, Light** lig, vector<Texture >* textures, vector<Appearance >*appearances, vector<Camera >* cameras);
	~XMLSceneMod();

	static TiXmlElement *findChildByAttribute(TiXmlElement *parent,const char * attr, const char *val);
	
	bool readRGBcomponents (char* rawString, float &R, float &G, float &B, float &A);
	bool readXYcoord (char* rawString, float &x, float &y);
	bool readXYZcoord (char* rawString, float &x, float &y, float &z);
	bool readFloatArray(char* rawString,  float (&a)[4]);

	bool readGlobals(TiXmlElement* dgxElement);
	bool readCameras(TiXmlElement* dgxElement);
	bool readLights(TiXmlElement* dgxElement);
	bool readTextures(TiXmlElement* dgxElement);
	bool readAppearances(TiXmlElement* dgxElement);
	bool readGraph(TiXmlElement* dgxElement);

protected:

	TiXmlDocument* doc;
	Graph* destinationGraph;
	Light** destinationLights;
	vector<Texture >* textures;
	vector<Appearance >*appearances;
	vector<Camera >*cameras;
	//vector<CGFlight *> luzes;
	/////////////Projecto LAIG////////////
	TiXmlElement* globalsElement;
	TiXmlElement* camerasElement;
	TiXmlElement* lightsElement;
	TiXmlElement* graphElement;
	TiXmlElement* appearancesElement;
	TiXmlElement* texturesElement;
	//////////////////////////////////////




};

#endif