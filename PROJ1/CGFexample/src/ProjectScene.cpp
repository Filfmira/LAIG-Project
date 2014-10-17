#include "ProjectScene.h"
#include "CGFaxis.h"
#include "CGFapplication.h"
#include <iostream>

float Appearance::texlength_s = 0;
float Appearance::texlength_t = 0;


// Global ambient light (do not confuse with ambient component of individual lights)
//float globalAmbientLight[4]= {0.8,0.8,0.8,1.0};
float globalAmbientLight[4]= {0.2,0.2,0.2,1.0};



float ambientNull[4]={0,0,0,1};
float yellow[4]={1,1,0,1};

void ProjectScene::init() 
{

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

	XMLSceneMod temp = XMLSceneMod("wall-e.xml", &sceneGraph, lights ,textures,appearances,&cameras, activeCamera);


	for (unsigned int i = 0; i < 8; i++){
		if (lights[i] == NULL)
			break;
		cout << lights[i]->showLight();
	}


	


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
	//activeCamera->getCamera()->applyView();
	CGFscene::activeCamera->applyView();
	//VisualizationMode
	if (wireFrame)
		setWireFrameMode();
	else setTextureMode();


	for (unsigned int i = 0; i < 8; i++)
		if (lights[i] != NULL){
			lights[i]->getLight()->draw();
			lights[i]->getLight()->update();
		}


	// Draw axis
	//axis.draw();

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
		
		if (node->getAppearance() != NULL){
			if (node->getAppearance()->getTexture() != NULL){
				//printf("Node: %s\n", node->getId().c_str());
				//printf("	Textura: %s\n", node->getAppearance()->getTexture()->getId());
				Appearance::texlength_s = node->getAppearance()->getTexture()->getTexlengths();
				Appearance::texlength_t = node->getAppearance()->getTexture()->getTexlengtht();
			}
			node->getAppearance()->apply();
			this->appearancesStack.push(node->getAppearance());
		}
		else appearancesStack.top()->apply();
	

		for (unsigned int j = 0; j < node->getNumeroDePrimitivas(); j++){
			node->getPrimitiva(j)->draw();
		}


		for (unsigned int i = 0; i < node->getDescendentes().size(); i++)
			drawAux(node->getDescendentes()[i]);

		if (node->getAppearance() != NULL)
			appearancesStack.pop();


	glPopMatrix();
}
ProjectScene::~ProjectScene() 
{
	/*for (unsigned int i = 0; i < 8; i++)
		if (lights[i]!=NULL)
			delete(lights[i]);

			*/

}

