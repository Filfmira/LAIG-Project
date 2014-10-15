#ifndef __PROJECTSCENE_H_
#define __PROJECTSCENE_H_

#include "CGFscene.h"
//#include "CGFappearance.h"
#include "XMLSceneMod.h"
#include "Lights.h"
#include <vector>
//#include "Textures.h"
#include "Appearances.h"
#include "Cameras.h"
#include "CGFapplication.h"


class ProjectScene : public CGFscene
{
public:
	void init();
	void display();

	CGFlight* lightTest;
	Light* lights[8];
	unsigned int numberOfLights;

	Graph sceneGraph;

	vector<Texture *> textures;
	vector<Appearance* > appearances;
	vector<Camera> cameras;
	Camera* activeCamera;

	CGFappearance* materialA;
	CGFappearance* materialB;
	
	bool wireFrame;

	void setWireFrameMode()
	{
		glPolygonMode(GL_FRONT,GL_LINE);
		glPolygonMode(GL_BACK,GL_LINE);

	}

	void setTextureMode()
	{
		glPolygonMode(GL_FRONT,GL_FILL);
		glPolygonMode(GL_BACK,GL_FILL);

	}

	~ProjectScene();
	


	///////////alternar entre wireframe e texturas////////////
	int visualizationMode;
	void setVisualizationMode(int mode){
		visualizationMode = mode;
		printf("mode = %d", mode);
	}


	///////////////////////////////////////////////////////////


	void drawAux(Node* node);
	};




#endif