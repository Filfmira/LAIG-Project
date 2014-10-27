#include "ProjectScene.h"
#include "CGFaxis.h"
#include "CGFapplication.h"
#include <iostream>


float Appearance::texlength_s = 0;
float Appearance::texlength_t = 0;


void ProjectScene::init() 
{

	for (unsigned int i = 0; i < 8; i++)
		lights[i] = NULL;
	
	globals=Global();
	XMLSceneMod temp = XMLSceneMod("wall-e.xml", &sceneGraph, lights ,textures,appearances,&cameras, activeCamera, &globals);

	glEnable(GL_LIGHTING);

	glEnable (GL_NORMALIZE);

	
	//mode
	//Defines Drawing computations
	if(strcmp(globals.getMode(), "fill") == 0)
		glPolygonMode(GL_FILL,GL_TRUE);
	else if(strcmp(globals.getMode(), "point") == 0)
		glPolygonMode(GL_POINT,GL_TRUE);
	else if(strcmp(globals.getMode(), "line") == 0)
		glPolygonMode(GL_LINE,GL_TRUE);
	//shading
	if(strcmp(globals.getShading(),"flat")==0)
		glShadeModel(GL_FLAT);
	else if (strcmp(globals.getShading(),"gouraud")==0)
	{
		glShadeModel(GL_SMOOTH);
	}
	
	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//background
	glClearColor(globals.getBackground()[0],globals.getBackground()[1],globals.getBackground()[2],globals.getBackground()[3]);
	
	//culling
	if(strcmp(globals.getFace(), "none") == 0)
			glDisable(GL_CULL_FACE);
	else if (strcmp(globals.getFace(), "front") == 0){
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		}
	else if (strcmp(globals.getFace(), "back") == 0){
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		}
	else if (strcmp(globals.getFace(), "both") == 0){
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT_AND_BACK);
	}

	//order
	if (strcmp(globals.getFace(), "ccw") == 0)
		glFrontFace(GL_CCW); 
	else if (strcmp(globals.getFace(), "cw") == 0)
		glFrontFace(GL_CW); 


	//doublesided
	if(globals.getDoublesided())
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	else
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	//local
	if(globals.getLocal())
		glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	else
		glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);

	//enabled
	if(globals.getEnabled())
		glEnable(GL_LIGHTING);



	// Sets up some lighting parameters
	// Computes lighting only using the front face normals and materials
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);  
	
	// Define ambient light (do not confuse with ambient component of individual lights)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globals.getAmbient());  
	



	for (unsigned int i = 0; i < 8; i++){
		if (lights[i] == NULL)
			break;
		cout << lights[i]->showLight();
	}


	


}

void ProjectScene::display() 
{

	// Clear image and depth buffer everytime we update the scene
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// Initialize Model-View matrix as identity (no transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	
	if (activeCamera == NULL)
		CGFscene::activeCamera->applyView();
	else activeCamera->getCamera()->applyView();
	

	if (wireFrame)
		setWireFrameMode();
	else setTextureMode();


	for (unsigned int i = 0; i < 8; i++)
		if (lights[i] != NULL){
			if (lights[i]->getMarker())
				lights[i]->getLight()->draw();
			lights[i]->getLight()->update();
		}


	//axis.draw();

	//primitives
	drawAux(sceneGraph.getRoot());



	// We have been drawing in a memory area that is not visible - the back buffer, 
	// while the graphics card is showing the contents of another buffer - the front buffer
	// glutSwapBuffers() will swap pointers so that the back buffer becomes the front buffer and vice-versa
	glutSwapBuffers();
}


void ProjectScene::drawAux(Node* node){

	glPushMatrix();
		glMultMatrixf(node->getMatrix());
		
		if (node->getAppearance() != NULL){
			this->appearancesStack.push(node->getAppearance());			
		}
		
		this->appearancesStack.top()->apply();		
		if (this->appearancesStack.top()->getTexture() != NULL){
				Appearance::texlength_s = this->appearancesStack.top()->getTexture()->getTexlengths();
				Appearance::texlength_t = this->appearancesStack.top()->getTexture()->getTexlengtht();
		}

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

}

