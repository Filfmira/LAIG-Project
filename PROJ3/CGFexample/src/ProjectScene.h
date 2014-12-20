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
#include "Animation.h"
#include "Flag.h"
#include "Vehicle.h"
#include "Patch.h"
#include "Evaluator.h"

#include "Piece.h"

class ProjectScene : public CGFscene
{
public:
	void init();
	void display();

	Light* lights[8];

	Graph sceneGraph;

	std::vector<Texture *> textures;
	std::vector<Appearance* > appearances;
	std::vector<Animation* > animations;
	std::vector<Camera> cameras;
	Camera* activeCamera;
	std::stack<Appearance*> appearancesStack;
	Global globals;
	std::vector<FlagShader*> flagShaders;
	Vehicle vehicle;

	Piece* pieceTest;

	bool wireFrame;

	void processGlobals();


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


	void update(unsigned long t){
		//cout<<t<<endl;
		//cout<<animations.size()<<endl;
		for(unsigned int k=0;k<animations.size();k++)
					animations.at(k)->update(t);
		//animations.at(0)->show();
		//animations.at(0)->update(t);
		//animations.at(1)->show();
		//cout<<"\n|||||||||||||||||||||||||||\n";
	}

	void updateFlagsSpeed(float s);
	
};




#endif