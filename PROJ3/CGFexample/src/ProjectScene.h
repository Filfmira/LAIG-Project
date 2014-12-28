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
	std::vector<Animation*> animationsPieces;
	Camera* activeCamera;

	Piece* pieceTest;
	Board* board;
	PieceData* selectedPiece; // default (none selected) is x = 100; y = 100
	
	string selectedType; //tipo de peça a jogar selecionado
	bool computadorAjogar; //true se a proxima iteração corresponder a jogada do computador
	string modoDeJogo; //JvJ, JvC, CvC ou none (J -> Jogador, C -> Computador, none -> modo de jogo ainda por selecionar)
	bool corActiva; //cor actualmente a jogar
	string gameOver; //winPreto, winBranco, draw ou NOT , ou restart (recomeçar o jogo com o modo definido)
	bool jogadaSimples; //se um jogador jogar uma peça simples, isto passa a true, para o jogador poder jogar uma segunda peça simples
	
	void noneSelected(); //set selectedPiece to default
	void setSelectedPiece(int x, int y);
	void setTypePiece(int n);
	void switchJogador();
	void jogar();
	void restartJogo(string modo);

	Animation* getAnimation(float x1,float y1,float z1,float x2,float y2,float z2);
	Animation* generateAnimation(int x, int y);

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
		for(unsigned int k=0;k<animationsPieces.size();k++)
		{
			if(animationsPieces.at(k)->isEnd())
				animationsPieces.erase (animationsPieces.begin()+k);
		}
		
		for(unsigned int k=0;k<animationsPieces.size();k++)
		{
			animationsPieces.at(k)->update(t);
		}
	}

	
};




#endif