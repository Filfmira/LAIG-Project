#include "ProjectScene.h"
#include "CGFaxis.h"
#include "CGFapplication.h"
#include <iostream>
#include "Socket.h"
#include "ParseProlog.h"

float Appearance::texlength_s = 0;
float Appearance::texlength_t = 0;

float globalAmbientLight[4]= {0.8,0.8,0.8,1.0};
bool jogadaSimples;

void ProjectScene::init() 
{
	float pos[3]={15,10,15};
	float target[3]={5,0,0,};
	perspective=new Perspective(1,20,45,pos,target);

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

	//inicializa��es
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
	PieceData jogada(-3, -1, false, "simples"); //est� manualmente a fazer uma jogada no x = -3, y = -1;
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
	jogadaSimples=false;
}

void ProjectScene::display() 
{
	//cout<<"animations:"<<this->animationsPieces.size()<<endl;
	// Clear image and depth buffer everytime we update the scene
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// Initialize Model-View matrix as identity (no transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//CGFscene::activeCamera->applyView();
	this->perspective->applyView();
	light0->draw();


	if (wireFrame)
		setWireFrameMode();
	else setTextureMode();

	axis.draw();
	
	//primitives
	//printf("x: %d, y: %d\n", this->selectedPiece->getX(), this->selectedPiece->getY());

	glPushMatrix();
	glTranslated(-5.8,0,0);
	glPushMatrix();	
		board->draw(this->selectedPiece->getX(), this->selectedPiece->getY());
	glPopMatrix();

	drawPecasLaterais();

	for(int i=0; i<this->pecasLixo.size();i++)
	{
		this->pieceTest->drawAnimation(this->pecasLixo[i].getCor(),this->pecasLixo[i].getTipo(),this->pecasLixo[i].getAnimation());

	}
	glPopMatrix();

	if (this->gameOver == "NOT") //se o jogo n�o tiver acabado
		this->jogar();
	else if (this->gameOver == "restart")
		this->restartJogo(this->modoDeJogo);

	// We have been drawing in a memory area that is not visible - the back buffer, 
	// while the graphics card is showing the contents of another buffer - the front buffer
	// glutSwapBuffers() will swap pointers so that the back buffer becomes the front buffer and vice-versa
	glutSwapBuffers();


}



void ProjectScene::drawPecasLaterais(){
	glPushMatrix();
	if(this->corActiva)
		glPushName(-1);		// Load a default name
		
		glTranslatef(0,3,-9);
		string tipos[5] = {"simples","ataque","defesa","expansao","salto"};
		for (int i=0; i< 5;i++)
		{
			glPushMatrix();
			glTranslatef(i*2,0,0);
			if(this->corActiva)
			glLoadName(i);	//replaces the value on top of the name stack
			if((this->selectedType == tipos[i])&& this->corActiva)
			pieceTest->draw(true, tipos[i],true );
			else
			pieceTest->draw(true, tipos[i],false );
			glPopMatrix();
		}
	glPopMatrix();
	glPopName() ;
	glPushMatrix();
		if(!this->corActiva)
		glPushName(-1);		// Load a default name
		
		glTranslatef(0,3,9);
		for (int i=0; i< 5;i++)
		{
			glPushMatrix();
			glTranslatef(i*2,0,0);
			if(!this->corActiva)
			glLoadName(i);	//replaces the value on top of the name stack
			if((this->selectedType == tipos[i])&& !this->corActiva)
			pieceTest->draw(false, tipos[i],true );
			else
			pieceTest->draw(false, tipos[i],false );
			glPopMatrix();
		}
	glPopMatrix();

}

/*
 * set selectedPiece to default (x = 100; y = 100)	
*/
void ProjectScene::noneSelected(){
	this->selectedPiece->setX(100);
	this->selectedPiece->setY(100);
}

void ProjectScene::setTypePiece(int n){
	if(n>4)
		return;
	if (this->jogadaSimples){
		this->selectedType = "simples";
		return;
	}

	string tipos[5] = {"simples","ataque","defesa","expansao","salto"};
	this->selectedType = tipos[n];
	cout << this->selectedType <<endl;
}

void ProjectScene::setSelectedPiece(int x, int y){
	printf("ENTROU EM %d . %d\n",x,y);
	if (this->selectedType == "none" || this->computadorAjogar){
		this->noneSelected(); //for�a a que nada esteja selecionado se selectedType n�o for nenhum
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
	if(!this->perspective->hasEnded())
		return;
	char ans[2048];
	char jogadaEtabuleiro[2048];
	vector<vector<PieceData> > newBoard;
	vector<PieceData> pecasAdicionadas, pecasRemovidas,jogadasComputador;
		
	if (computadorAjogar){ //jogada do computador

		string pc=jogadaComputadorToString(this->board->getBoard(), this->corActiva);
		strcpy(jogadaEtabuleiro, pc.c_str());
		envia(jogadaEtabuleiro, strlen(jogadaEtabuleiro)); //envia a jogada
		recebe(ans); // recebe resposta (ok ou not-ok)
		
		if (parseAnswerJogada((string)ans, newBoard, this->gameOver, pecasAdicionadas, pecasRemovidas,jogadasComputador))
			{//se ok, faz a jogada no tabuleiro local
			//this->board->setBoard(newBoard);

				
				

				for(int i=0;i<pecasAdicionadas.size();i++)
				{

					PieceData jogada=pecasAdicionadas[i];
					//generateAnimation(x,y,cor,tipo,insert)
					Animation* novaAnimacao = generateAnimation(pecasAdicionadas[i].getX(),pecasAdicionadas[i].getY(),pecasAdicionadas[i].getCor(),pecasAdicionadas[i].getTipo(),true);
					jogada.setAnimation(novaAnimacao);
					this->board->addPiece(jogada);
					//this->board->addPieceHistorico(jogada); //adiciona jogada ao historico
				}

				for(int i=0;i<jogadasComputador.size();i++)
				{
					PieceData jogada=jogadasComputador[i];
					Animation* novaAnimacao = generateAnimation(jogadasComputador[i].getX(),
						jogadasComputador[i].getY(),
						jogadasComputador[i].getAnimation(),
						jogadasComputador[i].getTipo(),true);
					jogada.setAnimation(novaAnimacao);
					this->board->addPiece(jogada);
					//this->board->addPieceHistorico(jogada); //adiciona jogada ao historico
					pecasAdicionadas.push_back(jogada);

				}

				for(int i=0;i<pecasRemovidas.size();i++)
				{

					PieceData jogada=pecasRemovidas[i];
					Animation* novaAnimacao = generateAnimation(pecasRemovidas[i].getX(),
						pecasRemovidas[i].getY(),
						pecasRemovidas[i].getCor(),
						pecasRemovidas[i].getTipo(),false);
					jogada.setAnimation(novaAnimacao);
					//pecasRemovidas.push_back(jogada);
					this->board->removePiece(pecasRemovidas[i]);
					
					//this->board->addPieceHistorico(jogada); //adiciona jogada ao historico

				}
				

				Jogada jogadaParaHistorico(pecasAdicionadas, pecasRemovidas);
				this->board->addJogadaHistorico(jogadaParaHistorico);

				cout << "Numero de jogadas: " << this->board->getHistorico().size() << endl;
			}
		//adicionar ao historico a jogada do computador
		//� preciso arranjar maneira de saber o que � que o computador jogou
		this->switchJogador();
	}
	else{ //humano a jogar

		if (this->selectedType != "none" && this->selectedPiece->getX() != 100){
			
			PieceData jogada(this->selectedPiece->getX(), this->selectedPiece->getY(), this->corActiva, this->selectedType); 
			string j = jogadaToString(jogada, this->board->getBoard()); //transforma a jogada num comando a enviar ao prolog
			strcpy(jogadaEtabuleiro, j.c_str());
	
			envia(jogadaEtabuleiro, strlen(jogadaEtabuleiro)); //envia a jogada
			recebe(ans); // recebe resposta (ok ou not-ok)
	
			if (parseAnswerJogada((string)ans, newBoard, this->gameOver, pecasAdicionadas, pecasRemovidas,jogadasComputador)){ //se ok, faz a jogada no tabuleiro local
				
				for(int i=0;i<pecasAdicionadas.size();i++)
				{

					PieceData jogada=pecasAdicionadas[i];
					Animation* novaAnimacao = generateAnimation(pecasAdicionadas[i].getX(),
						pecasAdicionadas[i].getY(),
						pecasAdicionadas[i].getCor(),
						pecasAdicionadas[i].getTipo(),true);
					jogada.setAnimation(novaAnimacao);
					this->board->addPiece(jogada);

				}

				for(int i=0;i<pecasRemovidas.size();i++)
				{

					PieceData jogada=pecasRemovidas[i];
					Animation* novaAnimacao = generateAnimation(pecasRemovidas[i].getX(),
						pecasRemovidas[i].getY(),
						pecasRemovidas[i].getCor(),
						pecasRemovidas[i].getTipo(),false);
					jogada.setAnimation(novaAnimacao);
					pecasLixo.push_back(jogada);
					this->board->removePiece(pecasRemovidas[i]);
				}
				
				Animation* novaAnimacao = generateAnimation(this->selectedPiece->getX(),
					this->selectedPiece->getY(),this->corActiva,this->selectedType,true);
				jogada.setAnimation(novaAnimacao);

				this->board->addPiece(jogada);

				pecasAdicionadas.push_back(jogada);
				Jogada jogadaParaHistorico(pecasAdicionadas, pecasRemovidas);
				this->board->addJogadaHistorico(jogadaParaHistorico);
				cout << "Numero de jogadas: " << this->board->getHistorico().size() << endl;

				if (!this->jogadaSimples && jogada.getTipo() == "simples"){//se jogou simples, e n�o era a segunda jogada
					this->jogadaSimples = true;
				}
				else this->switchJogador();
			}			
			this->noneSelected();
		}
	}


}

void ProjectScene::switchJogador(){
	this->corActiva = !this->corActiva;
	if (this->modoDeJogo == "JvC")
		this->computadorAjogar = !this->computadorAjogar;
	this->jogadaSimples = false;
	this->selectedType = "none";
	this->noneSelected();
	this->perspective->change(this->corActiva);
}

void ProjectScene::restartJogo(string modo){
	cout << "Restarting to: " << modo << endl;
	this->computadorAjogar = (modo == "CvC");
	this->modoDeJogo = modo;

	//inicializa��es
	this->noneSelected();
	this->selectedType = "none";
	this->corActiva = true;
	this->gameOver = "NOT";
	this->jogadaSimples = false;

	char *s = "novo-tabuleiro.\n";
	envia(s, strlen(s));
	char ans[2048];
	recebe(ans);
	vector<vector <PieceData> > tempBoard;
	string temp;vector<PieceData> temp2,temp3;
	parseAnswerJogada((string) ans,tempBoard, temp, temp2, temp2,temp3);
	this->board = new Board(tempBoard);
	jogadaSimples=false;
	cout << "Success restart\n";
}

Animation* ProjectScene::getAnimation(float x1,float y1,float z1,float x2,float y2,float z2,float time){
	vector<float>p1;
	p1.push_back(x1);
	p1.push_back(y1);
	p1.push_back(z1);

	vector<float>p2;
	p2.push_back(x1);
	p2.push_back(y1+2);
	p2.push_back(z1);

	vector<float>p3;
	p3.push_back(x2);
	p3.push_back(y2+2);
	p3.push_back(z2);

	vector<float>p4;
	p4.push_back(x2);
	p4.push_back(y2);
	p4.push_back(z2);

	vector< vector<float>> v;
	v.push_back(p1);
	v.push_back(p2);
	v.push_back(p3);
	v.push_back(p4);

	return new LinearAnimation("linear",time,v);

}



Animation*  ProjectScene::generateAnimation(int x, int y,bool color,string tipo, bool insert)
{
	//vai traduzir as coordenadas do tabuleiro prolog para as coordenadas graficas do prolog
	int tamanho=7;
	int pX;				//primeiro valor de x na linha
	int o;				//primeiro valor da linha
	float temp=tamanho/2;
	int t=floor(temp);

	if(y<0 || y==0)
	{
		pX=-t-y;
		o=-y;
	}
	else{
		pX=-t;
		o=y;
	}

	int xNovo;

	int x1=x;
	int distancia=0;

	if(y<=0){
		if(pX<0)
			distancia=abs(pX)+x1+1;
		else if(pX==0)
			distancia=x1+1;

		xNovo=distancia*2+pX+1;
	}

	else{

		if(x1<0)
			distancia=abs(pX)-abs(x1);
		else if(x==0)
			distancia=abs(pX);
		else
			distancia=abs(pX)+x1;
				
		xNovo=distancia*2+o;
	}
	int yNovo=y*2;

	//calcula coordenadas do inicio da animacao (de onde a pe�a sai)
	string tipos[5] = {"simples","ataque","defesa","expansao","salto"};
	int i;

	for(i =0;i<5;i++){	
		if(tipo==tipos[i])
			break;
	}
	int xi,yi,zi;
	if(color)
	{
		xi=i*2;
		yi=3;
		zi=-9;
	}
	else{
		xi=i*2;
		yi=3;
		zi=9;

	}
	Animation* final;
	if(insert)
	final=getAnimation(xi,yi,zi,xNovo,0,yNovo,3);
	else
	final=getAnimation(xNovo,0,yNovo,-10,0,-10,5);
	return final;

}


void ProjectScene::undo(){
	int numeroUndos = 1;
	if (this->modoDeJogo == "JvC" && this->computadorAjogar == false){
		if (this->jogadaSimples)
			numeroUndos = 1;
		else numeroUndos = 2;
	}

	for (int i = 0; i < numeroUndos; i++){

		Jogada jogada = this->board->getHistorico()[this->board->getHistorico().size()-1];
		this->board->popBackHistorico();

		vector<PieceData> temp = jogada.getAdicionadas();
		for (unsigned int j = 0; j < temp.size(); j++){
			this->board->removePiece(temp[j]);
		}

		temp = jogada.getRemovidas();
		for (unsigned int j = 0; j < temp.size(); j++){
			this->board->addPiece(temp[j]);
		}
		
		if (!this->jogadaSimples)
			this->switchJogador();
	}
}

ProjectScene::~ProjectScene() 
{

}

