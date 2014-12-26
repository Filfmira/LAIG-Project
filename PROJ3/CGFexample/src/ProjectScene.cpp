#include "ProjectScene.h"
#include "CGFaxis.h"
#include "CGFapplication.h"
#include <iostream>
#include "Socket.h"
#include "ParseProlog.h"

float Appearance::texlength_s = 0;
float Appearance::texlength_t = 0;

float globalAmbientLight[4]= {0.8,0.8,0.8,1.0};

void ProjectScene::init() 
{
	pieceTest = new Piece();
	type="simples";
	glPolygonMode(GL_FILL,GL_TRUE);
	glShadeModel(GL_SMOOTH);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	
	glFrontFace(GL_CCW);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);  
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, CGFlight::background_ambient);  
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientLight);  

	glEnable(GL_LIGHTING);

	glEnable (GL_NORMALIZE);

	float light0_pos[4] = {4.0, 6.0, 5.0, 1.0};
	light0 = new CGFlight(GL_LIGHT0, light0_pos);
	light0->enable();

	this->pieceTest = new Piece();

	// Animation-related code
	unsigned long updatePeriod=50;
	setUpdatePeriod(updatePeriod);

	//inicializar a peça selecionada ao default
	this->selectedPiece = new PieceData(100, 100, true, "simples");

	//socket prolog
	if (!socketConnect()){
		printf("Error connecting to PROLOG...\n");
		cin.get();
		exit(1);
	}

	char *s = "novo-tabuleiro.\n";
	envia(s, strlen(s));
	char ans[2048];
	recebe(ans);
	vector<vector <PieceData> > tempBoard;
	parseAnswerJogada((string) ans,tempBoard);
	this->board = new Board(tempBoard);
	
	/*char *t = "jogada.\n";
	envia(t, strlen(t));
	recebe(ans);*/

	/*
	
	PieceData jogada(-3, -1, false, "simples"); //está manualmente a fazer uma jogada no x = -3, y = -1;
	string j = jogadaToString(jogada, this->board->getBoard()); //transforma a jogada num comando a enviar ao prolog
	char jogadaEtabuleiro[2048];
	strcpy(jogadaEtabuleiro, j.c_str());
	envia(jogadaEtabuleiro, strlen(jogadaEtabuleiro)); //envia a jogada
	ans[0] = '\0'; recebe(ans); // recebe resposta (ok ou not-ok)
	
	vector<vector<PieceData> > newBoard;
	if (parseAnswerJogada((string)ans, newBoard)){ //se ok, faz a jogada no tabuleiro local
		this->board->setBoard(newBoard);
	}
	*/
	
}

void ProjectScene::display() 
{

	// Clear image and depth buffer everytime we update the scene
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// Initialize Model-View matrix as identity (no transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	CGFscene::activeCamera->applyView();
	light0->draw();


	if (wireFrame)
		setWireFrameMode();
	else setTextureMode();

	axis.draw();
	
	
	//primitives
	//printf("x: %d, y: %d\n", this->selectedPiece->getX(), this->selectedPiece->getY());
	//pieceTest->draw(false, "ataque");
	glPushMatrix();
		//glPushName(-1);	
		board->draw(this->selectedPiece->getX(), this->selectedPiece->getY());
	glPopMatrix();


	glPushMatrix();

	glPushName(-1);		// Load a default name
	
	
	glTranslatef(0,3,-9);

	for (int i=0; i< 5;i++)
	{
		glPushMatrix();
		glTranslatef(i*2,0,0);
		glLoadName(i);		//replaces the value on top of the name stack
		string tipoTemp;
		switch (i){
	case 0:
		tipoTemp="simples";
		break;
	case 1:
		tipoTemp="ataque";
		break;
	case 2:
		tipoTemp="defesa";
		break;
	case 3:
		tipoTemp="expansao";
		break;
	case 4:
		tipoTemp="salto";
		break;


	}
		pieceTest->draw(true,tipoTemp,true);
		glPopMatrix();
	}
	glPopMatrix();



	// We have been drawing in a memory area that is not visible - the back buffer, 
	// while the graphics card is showing the contents of another buffer - the front buffer
	// glutSwapBuffers() will swap pointers so that the back buffer becomes the front buffer and vice-versa
	glutSwapBuffers();
}

/*
 * set selectedPiece to default (x = 100; y = 100)	
*/
void ProjectScene::noneSelected(){
	this->selectedPiece->setX(100);
	this->selectedPiece->setY(100);
}

void ProjectScene::setTypePiece(int n){

	switch (n){
	case 0:
		type="simples";
		break;
	case 1:
		type="ataque";
		break;
	case 2:
		type="defesa";
		break;
	case 3:
		type="expansao";
		break;
	case 4:
		type="salto";
		break;


	}
	
	cout<<type<<endl;

}

void ProjectScene::setSelectedPiece(int x, int y){
	printf("ENTROU EM %d . %d\n",x,y);
	if (x == this->selectedPiece->getX() && y == this->selectedPiece->getY())
		this->noneSelected();
	else{
		this->selectedPiece->setX(x);
		this->selectedPiece->setY(y);
	}

		char ans[2048];
	PieceData jogada(x, y, false, type); //está manualmente a fazer uma jogada no x = -3, y = -1;
	string j = jogadaToString(jogada, this->board->getBoard()); //transforma a jogada num comando a enviar ao prolog
	char jogadaEtabuleiro[2048];
	strcpy(jogadaEtabuleiro, j.c_str());
	envia(jogadaEtabuleiro, strlen(jogadaEtabuleiro)); //envia a jogada
	ans[0] = '\0'; recebe(ans); // recebe resposta (ok ou not-ok)
	
	vector<vector<PieceData> > newBoard;
	if (parseAnswerJogada((string)ans, newBoard)){ //se ok, faz a jogada no tabuleiro local
		this->board->setBoard(newBoard);
	}


	
	string pc=jogadaComputadorToString(this->board->getBoard());
	char jogadaEtabuleiro2[2048];
	strcpy(jogadaEtabuleiro2, pc.c_str());
	envia(jogadaEtabuleiro2, strlen(jogadaEtabuleiro2)); //envia a jogadaans[0] = '\0'; recebe(ans); // recebe resposta (ok ou not-ok)
	ans[0] = '\0'; recebe(ans); // recebe resposta (ok ou not-ok)
	vector<vector<PieceData> > newBoard2;
	if (parseAnswerJogada((string)ans, newBoard2)){ //se ok, faz a jogada no tabuleiro local
		this->board->setBoard(newBoard2);
	}
}


ProjectScene::~ProjectScene() 
{

}

