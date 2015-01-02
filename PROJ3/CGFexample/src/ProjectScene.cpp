#include "ProjectScene.h"
#include "CGFaxis.h"
#include "CGFapplication.h"
#include <iostream>
#include "Socket.h"
#include "ParseProlog.h"
#include <sstream>

float Appearance::texlength_s = 1;
float Appearance::texlength_t = 1;

float globalAmbientLight[4]= {0,0,0,1.0};
bool jogadaSimples;

float alturaPecas=0;
float ditanciaPecas=10;
float posicaoLixo=5;




void ProjectScene::init() 
{
	animacoes=0;

	camera=true;		//true é com a perspectiva e false com a defualt

	appValues=new Appearance("appValues");
	appDefaultValues=new Appearance("appValues");

	float spec[4]={0.9,0.9,0.9,1};
	float difuse[4]={0.9,0.9,0.9,1};
	appValues->setSpecular(spec);
	appValues->setDiffuse(difuse);
	appValues->setShininess(50.f);

	Texture* tSimples = new Texture("b", "texto/simples.png", 1, 1);
	Texture* tAtaque = new Texture("b", "texto/ataque.png", 1, 1);
	Texture* tDefesa = new Texture("b", "texto/defesa.png", 1, 1);
	Texture* tExpansao = new Texture("b", "texto/expansao.png", 1, 1);
	Texture* tSalto = new Texture("b", "texto/salto.png", 1, 1);
	
	texturesLabeling.push_back(tSimples);
	texturesLabeling.push_back(tAtaque);
	texturesLabeling.push_back(tDefesa);
	texturesLabeling.push_back(tExpansao);
	texturesLabeling.push_back(tSalto);



	float pos[3]={15,10,15};
	float target[3]={5,0,0,};
	perspective=new Perspective(1,20,45,pos,target);
	cubeTest=new Cube();
	box=new Box();
	clock=new Clock();
	cylinder=new Cylinder(1,1,1,50,30);
	plane=new Plane(20);

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


	//lights
	float light0_pos[4] = {4.0, 50.0, 5.0, 1.0};
	light0 = new CGFlight(GL_LIGHT0, light0_pos);
	light0->enable();


	float light1_pos[4] = {0.0, 10.0, 0.0, 1.0};
	light1 = new CGFlight(GL_LIGHT1, light1_pos);
	light1->enable();
	float amb[4]= {1,1,1,1.0};
	light1->setAmbient(amb);
	light1->setSpecular(amb);
	light1->setSpecular(amb);
	//light1->setKl(0.9);
	//light1->setKc(1);

	float light2_pos[4] = {85.0, 10.0, 0.0, 1.0};
	light2 = new CGFlight(GL_LIGHT2, light2_pos);
	light2->enable();
	float amb2[4]= {0.3,0.3,0.3,1.0};
	light1->setAmbient(amb2);
	float dif2[4]= {0.5,0.5,0.5,1.0};
	light2->setDiffuse(dif2);
	light2->setSpecular(dif2);


	// Animation-related code
	unsigned long updatePeriod=50;
	setUpdatePeriod(updatePeriod);

	//inicializações
	this->pieceTest = new Piece();
	this->selectedPiece = new PieceData(100, 100, true, "simples");
	this->marcadorBranco = new MarcadorPontuacao();
	this->marcadorPreto = new MarcadorPontuacao();
	this->timeSpan = 2;

	//socket prolog
	if (!socketConnect()){
		printf("Error connecting to PROLOG...\n");
		cin.get();
		exit(1);
	}


	this->aparenciaActiva = 0;

	this->appBoard.push_back(new Appearance("appBoard0"));
	this->appBoard.push_back(new Appearance("appBoard0"));
	this->appDefault.push_back(new Appearance("default0"));
	this->appDefault.push_back(new Appearance("default1"));
	this->background.push_back(new Appearance("background0"));
	this->background.push_back(new Appearance("background1"));

	this->appDefault[0]->setTexture(new Texture("d", "madeira0/default.jpg", 1, 1));
	this->appDefault[1]->setTexture(new Texture("d", "madeira1/default.jpg", 1, 1));

	this->appBoard[0]->setTexture(new Texture("a", "madeira0/mesa.jpg", 1, 1));
	this->appBoard[1]->setTexture(new Texture("a", "madeira1/mesa.jpg", 1, 1));

	this->background[0]->setTexture(new Texture("b", "madeira0/background.jpg", 1, 1));
	this->background[1]->setTexture(new Texture("b", "madeira1/background.jpg", 1, 1));



	//////////////////////////////////


	this->tamanhoTabuleiro = 7;
	this->restartJogo("CvC");	
	this->filmeEmCurso = false;
	this->jogadaFilmeActual = 0;
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
	this->perspective->cameraUpdate();
	if(camera)
	this->perspective->applyView();
	else
	CGFscene::activeCamera->applyView();
	

	////////////////////////////////////////////////////////////////desenhar luzes
	//light0->draw();
	//light1->draw();
	//light2->draw();

	if (wireFrame)
		setWireFrameMode();
	else setTextureMode();

	axis.draw();


	////////////////////////////////////////////////////////////////desenhar fundo
	glPushMatrix();
	glScaled(90,90,90);
	glCullFace(GL_FRONT);
	this->background[this->aparenciaActiva]->apply();
	this->cubeTest->draw();
	glPopMatrix();
		glCullFace(GL_BACK);

	////////////////////////////////////////////////////////////////desenhar mesa
	glPushMatrix();
	appBoard[this->aparenciaActiva]->apply();
	glTranslated(0,-0.5,0);
	glScaled(60,1,60);
	plane->draw();
	//this->cubeTest->draw();
	glPopMatrix();

	////////////////////////////////////////////////////////////////desenhar tabuleiro base
	glPushMatrix();
	appBoard[this->aparenciaActiva]->apply();
	glTranslated(0,-0.5,0);
	glScaled(8,0.5,8);
	glRotated(30,0,1,0);
	this->pieceTest->drawBooard(false, "tabuleiro", false);
	glPopMatrix();



	////////////////////////////////////////////////////////////////desenhar o relogio
	glPushMatrix();
	glTranslated(8,1,ditanciaPecas-0.5);
		light1->draw();
	glRotated(-20,0,1,0);
	glRotated(-40,1,0,0);
	this->clock->draw();
	glPopMatrix();

	glPushMatrix();
	glRotated(180,0,1,0);
	glTranslated(8,1,ditanciaPecas-0.5);
		light1->draw();
	glRotated(-20,0,1,0);
	glRotated(-40,1,0,0);
	this->clock->draw();
	glPopMatrix();

	
	glPushMatrix();
	float divisao=floor(float(this->board->getBoard().size())/2);
	glTranslated(-(divisao*2),0,0);
	////////////////////////////////////////////////////////////////desenhar as caixas das peças que cada jogador pode jogar
	glPushMatrix();
	float d=floor(float(this->board->getBoard().size())/2);
	glTranslated((d*2),0,0);
	drawPecasBox();
	glPopMatrix();
	
	////////////////////////////////////////////////////////////////desenhar caixas do lixo
	glPushMatrix();
	glTranslated(-posicaoLixo,0,-posicaoLixo);
	appDefault[this->aparenciaActiva]->apply();
	box->draw(3,3,1.5,0.3);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-posicaoLixo,0,posicaoLixo);
	appDefault[this->aparenciaActiva]->apply();
	box->draw(3,3,1.5,0.3);
	glPopMatrix();
	
	////////////////////////////////////////////////////////////////desenhar board
	glPushMatrix();	
		board->draw(this->selectedPiece->getX(), this->selectedPiece->getY());
	glPopMatrix();
	appDefault[this->aparenciaActiva]->apply();
	
	////////////////////////////////////////////////////////////////desenhar peças que foram removidas
	for(int i=0; i<this->pecasLixo.size();i++)
		this->pieceTest->drawAnimation(this->pecasLixo[i].getCor(),this->pecasLixo[i].getTipo(),this->pecasLixo[i].getAnimation());
	glPopMatrix();

	
	////////////////////////////////////////////////////////////////desenhar caixa dos marcadores
	glPushMatrix();
	glTranslated((this->board->getBoard().size()+4.025), 2, 0);
	glScaled(2,4.25,8);
	this->appDefault[this->aparenciaActiva]->apply();
	this->cubeTest->draw();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0,0.5,0);
		glPushMatrix();
			glTranslated((this->board->getBoard().size())+4, 0.25, -2);

			glRotated(90,0,0,1);
			glTranslated(1.25,0,0);
			this->appBoard[this->aparenciaActiva]->apply();
			box->draw(2.5,3.5,1,0.25);
		glPopMatrix();

		glPushMatrix();
			glTranslated((this->board->getBoard().size())+4, 0.25, 2);
			glRotated(90,0,0,1);
			glTranslated(1.25,0,0);
			this->appBoard[this->aparenciaActiva]->apply();
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
	if (this->filmeEmCurso){
		this->playJogadaFilme();
	}
	else{
		if (this->gameOver == "NOT") //se o jogo não tiver acabado
			this->jogar();
		else if (this->gameOver == "restart")
		{
			this->restartJogo(this->modoDeJogo);
			this->perspective->change(this->corActiva);
		}
		else if(this->gameOver == "winBranco"){ this->marcadorBranco->incPontuacao(); this->gameOver = "END"; }
		else if(this->gameOver == "winPreto") { this->marcadorPreto->incPontuacao(); this->gameOver = "END"; }

	}

	// We have been drawing in a memory area that is not visible - the back buffer, 
	// while the graphics card is showing the contents of another buffer - the front buffer
	// glutSwapBuffers() will swap pointers so that the back buffer becomes the front buffer and vice-versa
	glutSwapBuffers();


}



void ProjectScene::drawPecasLaterais(bool cor){
	string tipos[5] = {"simples","ataque","defesa","expansao","salto"};
	
	for (int i=0; i< 5;i++)
	{
		glPushMatrix();
			glTranslatef(i*2,0,0);
			if(cor == this->corActiva)
				glPushName(i);
			bool selected = (this->selectedType == tipos[i] && cor == this->corActiva);
			pieceTest->draw(cor, tipos[i], selected);
			//tabelas com laebling
			glPushMatrix();
			if(cor)
			{
			glTranslated(0,0.5,-2);
			glRotated(180,0,1,0);
			}
			else
			glTranslated(0,0.5,2);
			glScaled(2,0.01,0.6);
			this->texturesLabeling[i]->apply();
			this->cubeTest->draw();
			glPopMatrix();

			if(cor == this->corActiva)
				glPopName();
		glPopMatrix();
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
	appDefault[this->aparenciaActiva]->apply();
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
    appDefault[this->aparenciaActiva]->apply();
    box->draw(largura,comprimento,altura,expessura);
    glPopMatrix();
   drawPecasLaterais(false);
  glPopMatrix();
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
	{	
		clock->setBool(false);
		clock->reset();
		return;
	}
	if(this->clock->overSpan())
	{
		jogadorActivo=true;
		this->switchJogador();
		clock->reset();
		clock->setBool(false);
	}

	if(this->animacoes!=0 && !jogadaSimples)
	{
		return;
	}

	else if(this->animacoes==0 && jogadorActivo==false )
	{
		jogadorActivo=true;
		this->switchJogador();
		clock->setBool(true);
		return;
	}
	
	if(this->animacoes==0 && jogadorActivo && clock->getBool())
	{
		cout<<"///////////////////////////////////////////////////\n";
		clock->setBool(false);
		clock->reset();
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
		{
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
				

				Jogada jogadaParaHistorico(pecasAdicionadas, pecasRemovidas, this->jogadorActivo);
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
				Jogada jogadaParaHistorico(pecasAdicionadas, pecasRemovidas, this->jogadorActivo);
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

	ostringstream ss;
	ss << "[novo-tabuleiro," << this->tamanhoTabuleiro << "].\n";
	char s[64];
	strcpy(s, (char*)ss.str().c_str());
	printf("%s\n", s);
	envia(s, strlen(s));
	char ans[2048];
	recebe(ans);
	vector<vector <PieceData> > tempBoard;
	string temp;vector<PieceData> temp2,temp3;
	parseAnswerJogada((string) ans,tempBoard, temp, temp2, temp2,temp3);
	this->board = new Board(tempBoard);
	jogadaSimples=false;
	cout << "Success restart\n";

	this->changeTextures(this->aparenciaActiva);

	int si=pecasLixo.size();
	for(int i=0;i<si;i++)
		pecasLixo.pop_back();
	
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
	int tamanho=this->board->getBoard().size();
	int xNovo,yNovo;
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

	xNovo=2*(x-pX)+o;
	yNovo=y*2;


		cout<<"Xnovo:"<<xNovo<<" Ynovo:"<<yNovo;
	//calcula coordenadas do inicio da animacao (de onde a peça sai)
	string tipos[5] = {"simples","ataque","defesa","expansao","salto"};
	int i;

	for(i =0;i<5;i++){	
		if(tipo==tipos[i])
			break;
	}
	float xi,yi,zi;
	float divisao=floor(float(this->board->getBoard().size())/2)*2;
	if(color)
	{

		xi=(divisao-4)+(i*2);
		yi=alturaPecas;
		zi=-ditanciaPecas;
	}
	else{
		xi=(divisao-4)+(i*2);
		yi=alturaPecas;
		zi=ditanciaPecas;

	}

	Animation* final;
	if(insert)
	final=getAnimation(xi,yi,zi,xNovo,0,yNovo,this->timeSpan);
	else
	{
	if(color)
	final=getAnimation(xNovo,0,yNovo,-posicaoLixo,0,-posicaoLixo,this->timeSpan);
	else
	final=getAnimation(xNovo,0,yNovo,-posicaoLixo,0,posicaoLixo,this->timeSpan);
	}
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



void ProjectScene::changeTextures(int i){
	this->aparenciaActiva = i;
	this->board->changeTextures(i);
	this->pieceTest->changeTextures(i);
}


void ProjectScene::playJogadaFilme(){
	if(this->animacoes!=0)
		return;

	vector<Jogada> hist = this->board->getHistorico();

	if (this->jogadaFilmeActual == hist.size()){ //fim do filme
		this->jogadaFilmeActual = 0;
		this->filmeEmCurso = false;
		return;
	}

	cout << "Replay jogada nr " << this->jogadaFilmeActual << endl;
	vector<PieceData> tempAdicionadas = hist[this->jogadaFilmeActual].getAdicionadas();
	vector<PieceData> tempRemovidas = hist[this->jogadaFilmeActual].getRemovidas();
	
	for(int i=0;i<tempAdicionadas.size();i++)
	{
		this->board->addPiece(tempAdicionadas[i]);
	}

	for(int i=0;i<tempRemovidas.size();i++)
	{
		this->board->removePiece(tempRemovidas[i]);
		//pecasLixo.push_back(tempRemovidas[i]);
	}

	this->jogadaFilmeActual++;

}


void ProjectScene::initFilme(){
	ostringstream ss;
	ss << "[novo-tabuleiro," << this->board->getBoard().size() << "].\n";
	char s[64];
	strcpy(s, (char*)ss.str().c_str());
	envia(s, strlen(s));
	char ans[2048];
	recebe(ans);
	vector<vector <PieceData> > tempBoard;
	string temp;
	vector<PieceData> temp2,temp3;
	parseAnswerJogada((string) ans,tempBoard, temp, temp2, temp2,temp3);
	this->board->setBoard(tempBoard);
	this->board->restartAnimacoes();
	this->filmeEmCurso = true;

	int si=pecasLixo.size();
	for(int i=0;i<si;i++)
		pecasLixo.pop_back();
}


ProjectScene::~ProjectScene() 
{

}

