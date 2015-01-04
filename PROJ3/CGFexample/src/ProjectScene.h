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
#include "MarcadorPontuacao.h"
#include "Clock.h"
#include "Evaluator.h"

class ProjectScene : public CGFscene
{
public:
	void init();
	void display();

	CGFlight* light0;
	CGFlight* light1;
	CGFlight* light2;

	std::vector<Texture *> textures;
	std::vector<Texture *> texturesLabeling;
	std::vector<Appearance* > appearances;
	std::vector<Camera> cameras;

	int animacoes;
	bool jogadorActivo;
	
	int aparenciaActiva;
	vector<Appearance*> appDefault;
	vector<Appearance*> appBoard;
	vector<Appearance*> background;



	Appearance* appValues;
	Appearance* appDefaultValues;
	
	Plane *plane;
	std::vector<PieceData> pecasLixo;
	Cylinder* cylinder;
	Camera* activeCamera;
	Clock* clock;
	Cube* cubeTest;
	Piece* pieceTest;
	Board* board;
	Box* box;
	PieceData* selectedPiece; // default (none selected) is x = 100; y = 100
	Perspective* perspective;
	string selectedType; //tipo de peça a jogar selecionado
	bool computadorAjogar; //true se a proxima iteração corresponder a jogada do computador
	string modoDeJogo; //JvJ, JvC, CvC ou none (J -> Jogador, C -> Computador, none -> modo de jogo ainda por selecionar)
	bool corActiva; //cor actualmente a jogar
	string gameOver; //winPreto, winBranco, draw ou NOT , ou restart (recomeçar o jogo com o modo definido)
	bool jogadaSimples; //se um jogador jogar uma peça simples, isto passa a true, para o jogador poder jogar uma segunda peça simples
	float timeSpan;
	int tamanhoTabuleiro;
	int tamanhoTabuleiroActual;
	bool camera;
	
	MarcadorPontuacao* marcadorBranco;
	MarcadorPontuacao* marcadorPreto;
	
	void noneSelected(); //set selectedPiece to default
	void setSelectedPiece(int x, int y);
	void setTypePiece(int n);
	void switchJogador();
	void jogar();
	void restartJogo(string modo);
	void undo();
	Animation* getAnimation(float x1,float y1,float z1,float x2,float y2,float z2,float time);
	void drawPecasLaterais(bool cor);
	void drawPecasBox();
	void changeTextures(int i);

	//////FILME //////
	bool filmeEmCurso;
	int jogadaFilmeActual;
	void playJogadaFilme();
	void initFilme();
	//////////////////

	Animation* generateAnimation(int x, int y,bool color,string tipo,bool insert);

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
		//if(!clock->getBool())
		clock->update(t);

	animacoes=0;
		for(int i=0;i<pecasLixo.size();i++)
		{
			if(pecasLixo[i].getAnimation()->isEnd())
			pecasLixo.erase(pecasLixo.begin(),pecasLixo.begin()+i);
			else
			{
			pecasLixo[i].getAnimation()->update(t);
			animacoes++;
			}
		}

		for(unsigned int k=0;k<this->board->getBoard().size();k++)
			for(unsigned int i=0;i<this->board->getBoard().at(k).size();i++){
				if(this->board->getBoard()[k][i].hasAnimation())
				{
					this->board->getBoard()[k][i].getAnimation()->update(t);
					animacoes++;
				}
			}
	}
	
	
};




#endif