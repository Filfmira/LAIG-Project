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

	this->pieceTest = new Piece();

	// Animation-related code
	unsigned long updatePeriod=50;
	setUpdatePeriod(updatePeriod);

	//socket prolog
	if (!socketConnect()){
		printf("Error connecting to PROLOG...\n");
		cin.get();
		exit(1);
	}

	char *s = "novo-tabuleiro.\n";
	envia(s, strlen(s));
	char ans[10000];
	recebe(ans);
	vector<vector <PieceData> > tempBoard = parseBoard((string)ans);
	this->board = new Board(tempBoard);
	
	//quit();
	//getchar();
	//exit(0);

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
	
	//pieceTest->draw(false, "ataque");
	glPushMatrix();
		//glPushName(-1);	
		board->draw();
	glPopMatrix();



	// We have been drawing in a memory area that is not visible - the back buffer, 
	// while the graphics card is showing the contents of another buffer - the front buffer
	// glutSwapBuffers() will swap pointers so that the back buffer becomes the front buffer and vice-versa
	glutSwapBuffers();
}





ProjectScene::~ProjectScene() 
{

}

