#include "ProjectScene.h"
#include "CGFaxis.h"
#include "CGFapplication.h"
#include <iostream>
#include "Socket.h"
#include "ParseProlog.h"

float Appearance::texlength_s = 1;
float Appearance::texlength_t = 1;

float globalAmbientLight[4]= {0.8,0.8,0.8,1.0};
bool jogadaSimples;

float alturaPecas=0;
float ditanciaPecas=10;
float posicaoLixo=5;

float timeSpan=2;


void ProjectScene::init() 
{
	animacoes=0;

	appDefault=new Appearance("default");
	Texture* texture = new Texture("tabuleiro", "72.jpg", 1, 1);
	appDefault->setTexture(texture);

	appBoard=new Appearance("appBoard");
	Texture* texture2 = new Texture("tabuleiro", "b.jpg", 1, 1);
	appBoard->setTexture(texture2);

	float pos[3]={15,10,15};
	float target[3]={5,0,0,};
	perspective=new Perspective(1,20,45,pos,target);
	cubeTest=new Cube();
	box=new Box();

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

	float light0_pos[4] = {4.0, 50.0, 5.0, 1.0};
	light0 = new CGFlight(GL_LIGHT0, light0_pos);
	light0->enable();

	

	// Animation-related code
	unsigned long updatePeriod=50;
	setUpdatePeriod(updatePeriod);

	//inicializações
	this->pieceTest = new Piece();
	this->selectedPiece = new PieceData(100, 100, true, "simples");
	this->marcadorBranco = new MarcadorPontuacao();
	this->marcadorPreto = new MarcadorPontuacao();

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
	
	////////////////////////////////////////////////////////////////camaras
	this->perspective->applyView();
	//CGFscene::activeCamera->applyView();
	
	light0->draw();


	if (wireFrame)
		setWireFrameMode();
	else setTextureMode();

	axis.draw();
	

	////////////////////////////////////////////////////////////////desenhar mesa
	glPushMatrix();
	appBoard->apply();
	glTranslated(0,-0.5,0);
	glScaled(40,0.01,42);
	this->cubeTest->draw();
	glPopMatrix();

	////////////////////////////////////////////////////////////////desenhar tabuleiro vase
	glPushMatrix();
	appBoard->apply();
	glTranslated(0,-0.5,0);
	glScaled(8,0.5,8);
	glRotated(30,0,1,0);
	this->pieceTest->drawBooard(false, "tabuleiro", false);
	glPopMatrix();

	////////////////////////////////////////////////////////////////desenhar as caixas das peças que cada jogador pode jogar
	drawPecasBox();

	////////////////////////////////////////////////////////////////desenhar caixas do lixo
	glPushMatrix();
	float divisao=floor(float(this->board->getBoard().size())/2);
	glTranslated(-(divisao*2),0,0);
	
	
	glPushMatrix();
	glTranslated(-posicaoLixo,0,-posicaoLixo);
	appDefault->apply();
	box->draw(3,3,1.5,0.3);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-posicaoLixo,0,posicaoLixo);
	appDefault->apply();
	box->draw(3,3,1.5,0.3);
	glPopMatrix();
	
	////////////////////////////////////////////////////////////////desenhar board
	glPushMatrix();	
		board->draw(this->selectedPiece->getX(), this->selectedPiece->getY());
	glPopMatrix();
	appDefault->apply();
	
	////////////////////////////////////////////////////////////////desenhar peças que foram removidas
	for(int i=0; i<this->pecasLixo.size();i++)
		this->pieceTest->drawAnimation(this->pecasLixo[i].getCor(),this->pecasLixo[i].getTipo(),this->pecasLixo[i].getAnimation());
	glPopMatrix();

	////////////////////////////////////////////////////////////////desenhar caixa dos marcadores
	glPushMatrix();
	glTranslated((this->board->getBoard().size()+4.025), 2, 0);
	glScaled(2,4.25,8);
	this->appDefault->apply();
	this->cubeTest->draw();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0,0.5,0);
		glPushMatrix();
		
			glTranslated((this->board->getBoard().size())+4, 0.25, -2);
			glRotated(90,0,0,1);
			glTranslated(1.25,0,0);
			this->appBoard->apply();
			box->draw(2.5,3.5,1,0.25);
		glPopMatrix();

		glPushMatrix();
			glTranslated((this->board->getBoard().size())+4, 0.25, 2);
			glRotated(90,0,0,1);
			glTranslated(1.25,0,0);
			this->appBoard->apply();
			box->draw(2.5,3.5,1,0.25);
		glPopMatrix();
		//desenhar marcadores
		glPushMatrix();
			glTranslated((this->board->getBoard().size()+3), 0, -3);
			glRotated(-90, 0, 1, 0);
			this->marcadorBranco->draw();
		glPopMatrix();

		glPushMatrix();
			glTranslated((this->board->getBoard().size()+3), 0, 1);
			glRotated(-90, 0, 1, 0);
			this->marcadorPreto->draw();
		glPopMatrix();
	glPopMatrix();

	////////////////////////////////////////////////////////////////

	if (this->gameOver == "NOT") //se o jogo não tiver acabado
		this->jogar();
	else if (this->gameOver == "restart")
	{
		this->restartJogo(this->modoDeJogo);
		this->perspective->change(this->corActiva);
	}
	else if(this->gameOver == "winBranco"){ this->marcadorBranco->incPontuacao(); this->gameOver = "END"; }
	else if(this->gameOver == "winPreto") { this->marcadorPreto->incPontuacao(); this->gameOver = "END"; }

	// We have been drawing in a memory area that is not visible - the back buffer, 
	// while the graphics card is showing the contents of another buffer - the front buffer
	// glutSwapBuffers() will swap pointers so that the back buffer becomes the front buffer and vice-versa
	glutSwapBuffers();


}

void ProjectScene::drawPecasBox()
{
	float largura,comprimento,altura,expessura;
	largura=5;
	comprimento=11;
	altura=1;
	expessura=0.2;
	

		glPushMatrix();
		glTranslated(-4,0,0);
			glTranslated(0,0,-ditanciaPecas);
				glPushMatrix();
				glTranslated(0,0,-1);
				glTranslatef(comprimento/2-1.5,alturaPecas,/*-9.5*/0);
				appDefault->apply();
				box->draw(largura,comprimento,altura,expessura);
				glPopMatrix();
			drawPecasLaterais(true);
		glPopMatrix();

		glPushMatrix();
		glTranslated(-4,0,0);
			glTranslated(0,0,ditanciaPecas);
				glPushMatrix();
				glTranslated(0,0,1);
				glTranslatef(comprimento/2-1.5,alturaPecas,/*-9.5*/0);
				appDefault->apply();
				box->draw(largura,comprimento,altura,expessura);
				glPopMatrix();
			drawPecasLaterais(false);
		glPopMatrix();
}


void ProjectScene::drawPecasLaterais(bool cor){
	string tipos[5] = {"simples","ataque","defesa","expansao","salto"};
	if(cor)
	{
		glPushMatrix();
				glPushName(-1);		// Load a default name
		
				//glTranslatef(0,alturaPecas,-9);
		
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
	}
	else
	{
	glPushMatrix();
	glPushName(-1);		// Load a default name
		
		//glTranslatef(0,alturaPecas,9);
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
	glPopName() ;
	}
	

}


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
	if(!this->perspective->hasEnded())
		return;

	if(this->animacoes!=0 )
		return;
	else if(this->animacoes==0 && jogadorActivo==false )
	{
		jogadorActivo=true;
		this->switchJogador();
		return;
	}
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
						jogadasComputador[i].getCor(),
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
					pecasLixo.push_back(jogada);
					this->board->removePiece(pecasRemovidas[i]);
					
					//this->board->addPieceHistorico(jogada); //adiciona jogada ao historico

				}
				

				Jogada jogadaParaHistorico(pecasAdicionadas, pecasRemovidas);
				this->board->addJogadaHistorico(jogadaParaHistorico);

				cout << "Numero de jogadas: " << this->board->getHistorico().size() << endl;
			}
		//adicionar ao historico a jogada do computador
		//é preciso arranjar maneira de saber o que é que o computador jogou
		jogadorActivo=false;
		//this->switchJogador();
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
					this->selectedPiece->getY(),jogada.getCor(),this->selectedType,true);
				jogada.setAnimation(novaAnimacao);

				this->board->addPiece(jogada);

				pecasAdicionadas.push_back(jogada);
				Jogada jogadaParaHistorico(pecasAdicionadas, pecasRemovidas);
				this->board->addJogadaHistorico(jogadaParaHistorico);
				cout << "Numero de jogadas: " << this->board->getHistorico().size() << endl;

				if (!this->jogadaSimples && jogada.getTipo() == "simples"){//se jogou simples, e não era a segunda jogada
					this->jogadaSimples = true;
				}
				else 
					{
					jogadorActivo=false;	
					//this->switchJogador();
					}
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

	//inicializações
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

	int si=pecasLixo.size();
	for(int i=0;i<si;i++)
		pecasLixo.pop_back();
	jogadorActivo==true;
	
}




Animation* ProjectScene::getAnimation(float x1,float y1,float z1,float x2,float y2,float z2,float time){
	vector<float>p1;
	p1.push_back(x1);
	p1.push_back(y1);
	p1.push_back(z1);

	vector<float>p2;
	p2.push_back(x1);
	p2.push_back(y1+2.5);
	p2.push_back(z1);

	vector<float>p3;
	p3.push_back(x2);
	p3.push_back(y2+2.5);
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

	//calcula coordenadas do inicio da animacao (de onde a peça sai)
	string tipos[5] = {"simples","ataque","defesa","expansao","salto"};
	int i;

	for(i =0;i<5;i++){	
		if(tipo==tipos[i])
			break;
	}
	float xi,yi,zi;
	if(color)
	{
		xi=(i*2)+2;
		yi=alturaPecas;
		zi=-ditanciaPecas;
	}
	else{
		xi=i*2+2;
		yi=alturaPecas;
		zi=ditanciaPecas;

	}
	Animation* final;
	if(insert)
	final=getAnimation(xi,yi,zi,xNovo,0,yNovo,timeSpan);
	else
	{
	if(color)
	final=getAnimation(xNovo,0,yNovo,-posicaoLixo,0,-posicaoLixo,timeSpan);
	else
	final=getAnimation(xNovo,0,yNovo,-posicaoLixo,0,posicaoLixo,timeSpan);
	}
	cout<<"xi####"<<xi<<endl;
	return final;

}


void ProjectScene::undo(){
	if (this->board->getHistorico().size() == 0)
		return;

	int numeroUndos = 1;
	if (this->modoDeJogo == "JvC" && this->computadorAjogar == false){
		if (this->jogadaSimples)
			numeroUndos = 1;
		else numeroUndos = 2;
	}
	if (this->board->getHistorico().size() == 1)
		numeroUndos = 1;

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

