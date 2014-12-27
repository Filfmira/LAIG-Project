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

	

	// Animation-related code
	unsigned long updatePeriod=50;
	setUpdatePeriod(updatePeriod);

	//inicializações
	this->pieceTest = new Piece();
	this->selectedPiece = new PieceData(100, 100, true, "simples");

	//socket prolog
	if (!socketConnect()){
		printf("Error connecting to PROLOG...\n");
		cin.get();
		exit(1);
	}



	this->restartJogo("CvC");

	
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


	glPushMatrix();	
		board->draw(this->selectedPiece->getX(), this->selectedPiece->getY());
	glPopMatrix();


	glPushMatrix();
		glPushName(-1);		// Load a default name
		
		glTranslatef(0,3,-9);
		string tipos[5] = {"simples","ataque","defesa","expansao","salto"};
		for (int i=0; i< 5;i++)
		{
			glPushMatrix();
			glTranslatef(i*2,0,0);
			glLoadName(i);	//replaces the value on top of the name stack
			pieceTest->draw(this->corActiva, tipos[i], (this->selectedType == tipos[i]));
			glPopMatrix();
		}
	glPopMatrix();

	if (this->gameOver == "NOT") //se o jogo não tiver acabado
		this->jogar();
	else if (this->gameOver == "restart")
		this->restartJogo(this->modoDeJogo);

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
	string tipos[5] = {"simples","ataque","defesa","expansao","salto"};
	this->selectedType = tipos[n];
	cout << this->selectedType <<endl;
}

void ProjectScene::setSelectedPiece(int x, int y){
	printf("ENTROU EM %d . %d\n",x,y);
	if (this->selectedType == "none" || this->computadorAjogar){
		this->noneSelected(); //força a que nada esteja selecionado se selectedType não for nenhum
		return;
	}

	if (x == this->selectedPiece->getX() && y == this->selectedPiece->getY())
		this->noneSelected();
	else{
		this->selectedPiece->setX(x);
		this->selectedPiece->setY(y);
	}
}


void ProjectScene::jogar(){
	char ans[2048];
	char jogadaEtabuleiro[2048];
	vector<vector<PieceData> > newBoard;
	
	if (computadorAjogar){ //jogada do computador

		string pc=jogadaComputadorToString(this->board->getBoard(), this->corActiva);
		strcpy(jogadaEtabuleiro, pc.c_str());
		envia(jogadaEtabuleiro, strlen(jogadaEtabuleiro)); //envia a jogada
		recebe(ans); // recebe resposta (ok ou not-ok)
		
		if (parseAnswerJogada((string)ans, newBoard, this->gameOver)) //se ok, faz a jogada no tabuleiro local
			this->board->setBoard(newBoard);
		//adicionar ao historico a jogada do computador
		//é preciso arranjar maneira de saber o que é que o computador jogou
		this->switchJogador();
	}
	else{ //jogada de um humano

		if (this->selectedType != "none" && this->selectedPiece->getX() != 100){
			
			PieceData jogada(this->selectedPiece->getX(), this->selectedPiece->getY(), this->corActiva, this->selectedType); 
			string j = jogadaToString(jogada, this->board->getBoard()); //transforma a jogada num comando a enviar ao prolog
			strcpy(jogadaEtabuleiro, j.c_str());
	
			envia(jogadaEtabuleiro, strlen(jogadaEtabuleiro)); //envia a jogada
			recebe(ans); // recebe resposta (ok ou not-ok)
	
			if (parseAnswerJogada((string)ans, newBoard, this->gameOver)){ //se ok, faz a jogada no tabuleiro local
				this->board->setBoard(newBoard);
				this->board->addPieceHistorico(jogada); //adiciona jogada ao historico
				this->switchJogador();
			}			
			this->selectedType = "none";
			this->noneSelected();
		}	
	}
}

void ProjectScene::switchJogador(){
	this->corActiva = !this->corActiva;
	if (this->modoDeJogo == "JvC")
		this->computadorAjogar = !this->computadorAjogar;
}

void ProjectScene::restartJogo(string modo){
	cout << "Restarting to: " << modo << endl;
	this->computadorAjogar = (modo == "CvC");
	this->modoDeJogo = modo;

	//inicializações
	this->noneSelected();
	this->selectedType = "none";
	this->corActiva = true;
	this->gameOver = "NOT";

	char *s = "novo-tabuleiro.\n";
	envia(s, strlen(s));
	char ans[2048];
	recebe(ans);
	vector<vector <PieceData> > tempBoard;
	string temp;
	parseAnswerJogada((string) ans,tempBoard, temp);
	this->board = new Board(tempBoard);
	cout << "Success restart\n";
}


ProjectScene::~ProjectScene() 
{

}

