#ifndef PROJECTSCENE_H
#define PROJECTSCENE_H

#include "CGFscene.h"
#include "CGFappearance.h"
#include "XMLSceneMod.h"

class ProjectScene : public CGFscene
{
public:
	void init();
	void display();

	//unsigned int numberOfLights;
	CGFlight* lights[8];

	Graph sceneGraph;

	CGFappearance* materialA;
	CGFappearance* materialB;
	
	~ProjectScene();

	vector<bool> lightStatus;
	
	 void toggleLight(CGFlight* light, int i){
		 if (lightStatus[i]){
			 light->disable();
			 lightStatus[i] = false;
		 }
		 else{
			 light->enable();
			 lightStatus[i] = true;
		 }
	 }



	///////////alternar entre wireframe e texturas////////////
	int visualizationMode;
	void setVisualizationMode(int mode){
		visualizationMode = mode;
		printf("mode = %d", mode);
	}

	
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
	///////////////////////////////////////////////////////////


	void drawAux(Node* node);
	};




#endif