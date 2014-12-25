#ifndef PROJECTSCENE_H
#define PROJECTSCENE_H

#include "CGFscene.h"
#include "Lights.h"
#include <vector>
#include "Appearances.h"
#include "Cameras.h"
#include "CGFapplication.h"
#include "Animation.h"
#include "Piece.h"
#include "PieceData.h"
#include "Board.h"

class ProjectScene : public CGFscene
{
public:
	void init();
	void display();

	CGFlight* light0;

	std::vector<Texture *> textures;
	std::vector<Appearance* > appearances;
	std::vector<Animation* > animations;
	std::vector<Camera> cameras;
	Camera* activeCamera;

	Piece* pieceTest;
	Board* board;
	PieceData* selectedPiece; // default (none selected) is x = 100; y = 100

	void noneSelected(); //set selectedPiece to default
	void setSelectedPiece(int x, int y);

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

	
};




#endif