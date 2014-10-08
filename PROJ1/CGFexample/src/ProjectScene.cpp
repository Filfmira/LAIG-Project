#include "ProjectScene.h"
#include "CGFaxis.h"
#include "CGFapplication.h"
#include <iostream>


// Positions for two lights
float light0_pos[4] = {4, 6.0, 1.0, 1.0};
float light1_pos[4] = {0, 0, 5, 1};



// Global ambient light (do not confuse with ambient component of individual lights)
//float globalAmbientLight[4]= {0.8,0.8,0.8,1.0};
float globalAmbientLight[4]= {0.2,0.2,0.2,1.0};


// Coefficients for material A
float ambA[3] = {0.2, 0.2, 0.2};
float difA[3] = {0.6, 0.6, 0.6};
float specA[3] = {0, 0.8, 0.8};
float shininessA = 120.f;

// Coefficients for material B
float ambB[3] = {0.2, 0.2, 0.2};
float difB[3] = {0.6, 0.6, 0.6};
float specB[3] = {0.8, 0.8, 0.8};
float shininessB = 120.f;



float ambientNull[4]={0,0,0,1};
float yellow[4]={1,1,0,1};

void ProjectScene::init() 
{
//	lightStatus = vector<bool>(5, false);
	for (unsigned int i = 0; i < 8; i++)
		lights[i] = NULL;
	
	glFrontFace(GL_CCW); //este parametro deve ser lido do anf file
						 //GL_CCW torna os poligonos CCW nas frontFaces (outra opção é GL_CW)
	//Enable
	//glShadeModel(GL_FLAT);
	glEnable(GL_LIGHTING);

	// Sets up some lighting parameters
	// Computes lighting only using the front face normals and materials
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);  
	
	// Define ambient light (do not confuse with ambient component of individual lights)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientLight);  
	

	 glEnable (GL_NORMALIZE);

	XMLSceneMod temp = XMLSceneMod("wall-e.xml", &sceneGraph, lights);

	for (unsigned int i = 0; i < 8; i++){
		if (lights[i] == NULL)
			break;
		cout << lights[i]->showLight();
	}

	
		
		
	// Declares and enables two lights, with null ambient component
	/*lights[0] = new CGFlight(GL_LIGHT0, light0_pos);
	lights[0]->setSpecular(yellow);
	lights[0]->setAmbient(ambientNull);

	//light0->disable();
	lights[0]->enable();*/

	
	/*
	lightTest = new CGFlight(GL_LIGHT1, light1_pos);
	lightTest->setAmbient(ambientNull);
	
	//light1->disable();
	lightTest->enable();

	*/
	
	// Uncomment below to enable normalization of lighting normal vectors
	

	
	//Declares materials
	materialA = new CGFappearance(ambA,difA,specA,shininessA);
	materialB = new CGFappearance(ambB,difB,specB,shininessB);
	
	

	visualizationMode = 1; //textured mode
	
	//XMLSceneMod temp = XMLSceneMod("anfFile - Copia.xml", &sceneGraph);
	
	/*if (0 && 1)
		printf("0 e 1\n");
	if (1 && 1)
		printf("1 e 1 \n ");
		*/

}

void ProjectScene::display() 
{

	// ---- BEGIN Background, camera and axis setup
	
	// Clear image and depth buffer everytime we update the scene
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// Initialize Model-View matrix as identity (no transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Apply transformations corresponding to the camera position relative to the origin
	CGFscene::activeCamera->applyView();


	for (unsigned int i = 0; i < 8; i++)
		if (lights[i] != NULL){
			lights[i]->getLight()->draw();
			lights[i]->getLight()->update();
		}

	//lightTest->draw();
	// Draw axis
	axis.draw();

	// ---- END Background, camera and axis setup

	// ---- BEGIN Primitive drawing section

	drawAux(sceneGraph.getRoot());

	
	// ---- END Primitive drawing section

	// We have been drawing in a memory area that is not visible - the back buffer, 
	// while the graphics card is showing the contents of another buffer - the front buffer
	// glutSwapBuffers() will swap pointers so that the back buffer becomes the front buffer and vice-versa
	glutSwapBuffers();
}
void ProjectScene::drawAux(Node* node){

	glPushMatrix();
	glMultMatrixf(node->getMatrix());
		for (unsigned int j = 0; j < node->getNumeroDePrimitivas(); j++){
			node->getPrimitiva(j)->draw();
		}
		for (unsigned int i = 0; i < node->getDescendentes().size(); i++){

			drawAux(node->getDescendentes()[i]);
		}
	glPopMatrix();
}
ProjectScene::~ProjectScene() 
{
	/*for (unsigned int i = 0; i < 8; i++)
		if (lights[i]!=NULL)
			delete(lights[i]);

			*/
	delete(materialA);
	delete(materialB);
}

