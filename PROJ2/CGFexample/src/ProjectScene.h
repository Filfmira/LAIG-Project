#ifndef PROJECTSCENE_H
#define PROJECTSCENE_H

#include "CGFscene.h"
#include "XMLSceneMod.h"
#include "Lights.h"
#include <vector>
#include "Appearances.h"
#include "Cameras.h"
#include "CGFapplication.h"
#include <stack>
#include "Globals.h"
#include "Flag.h"
class ProjectScene : public CGFscene
{
public:
	void init();
	void display();

	Light* lights[8];

	Graph sceneGraph;

	vector<Texture *> textures;
	vector<Appearance* > appearances;
	vector<Camera> cameras;
	Camera* activeCamera;
	stack<Appearance*> appearancesStack;
	Global globals;
	
	vector<FlagShader*> flagShaders;

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
	

	void drawAux(Node* node);


	void processDisplayLists(Node* n, Node* graphRoot);

	void updateFlagsSpeed(float s);
	
};




#endif