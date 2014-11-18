#ifndef XMLSCENEMOD_H
#define XMLSCENEMOD_H

#include "tinyxml.h"
#include "Graph.h"
#include "Lights.h"
#include "Textures.h"
#include "Appearances.h"
#include "Cameras.h"
#include "Animation.h"
#include  "Globals.h"
#include <stack>
#include "Flag.h"
#include "Vehicle.h"
#include "Patch.h"

class XMLSceneMod
{
public:
	XMLSceneMod(char *filename, Graph* gr, Light** lig, std::vector<Texture* > &textures, std::vector<Appearance* > &appearances,  std::vector<Animation* > &animations,vector<Camera >* cameras, Camera* &activeCamera, Global *globals, vector<FlagShader*> &flagShaders);
	~XMLSceneMod();

	static TiXmlElement *findChildByAttribute(TiXmlElement *parent,const char * attr, const char *val);
	
	bool readRGBcomponents (char* rawString, float &R, float &G, float &B, float &A);
	bool readXYcoord (char* rawString, float &x, float &y);
	bool readXYZcoord (char* rawString, float &x, float &y, float &z);
	bool readFloatArray(char* rawString,  float (&a)[4]);

	bool readGlobals(TiXmlElement* dgxElement);
	bool readCameras(TiXmlElement* dgxElement);
	bool readLights(TiXmlElement* dgxElement);
	bool readTextures(TiXmlElement* dgxElement, std::vector<Texture*> &text);
	bool readAppearances(TiXmlElement* dgxElement, std::vector<Appearance* > &appearances, std::vector<Texture*> &text);
	bool readGraph(TiXmlElement* dgxElement, std::vector<Appearance* > &appearances, std::vector<Animation*> &animationsVector,vector<FlagShader*> &flagShaders);

	bool readAnimations(TiXmlElement* dgxElement, std::vector<Animation*> &animationsVector);
	
	/*void processDisplayLists(Node* n);
	void processDisplayListsAux(Node* n);*/

	//stack<Appearance*> appearancesStack;

protected:

	TiXmlDocument* doc;
	Graph* destinationGraph;
	Light** destinationLights;
	std::vector<Camera >*cameras;
	Camera* initialCamera;
	Global* globals;
	TiXmlElement* globalsElement;
	TiXmlElement* camerasElement;
	TiXmlElement* lightsElement;
	TiXmlElement* graphElement;
	TiXmlElement* appearancesElement;
	TiXmlElement* texturesElement;
	TiXmlElement* animationElement;

};

#endif