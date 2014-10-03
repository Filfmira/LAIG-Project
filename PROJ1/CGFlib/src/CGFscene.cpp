#include "CGFscene.h"

#include <GL/glut.h>
#include "CGFapplication.h"
#include "CGFaxis.h"

CGFscene::CGFscene() {
	iface=NULL;
	lastUpdate=CGFapplication::getTime();
	setUpdatePeriod(0);
	initCameras();
}

CGFscene::~CGFscene() {

}


void CGFscene::activateCamera(int i)
{
	if(i<scene_cameras.size() && i >=0)
	{
		activeCamera = scene_cameras.at(i);
		CGFapplication::activeApp->forceRefresh();
	}
}

void CGFscene::setUpdatePeriod(unsigned long millis)
{
	updatePeriod=millis;
}

void CGFscene::checkUpdate()
{
	if (updatePeriod>0)
	{
		unsigned long currTime=CGFapplication::getTime();
		if (currTime-lastUpdate>=updatePeriod)
		{
			update(currTime);
			lastUpdate=currTime;
			/// TODO: Handle possible drifts
		}
	}
}

void CGFscene::update(unsigned long millis)
{
}


void CGFscene::initCameras()
{
	CGFcamera* first = new CGFcamera();
	CGFcamera* second = new CGFcamera();
	CGFcamera* third = new CGFcamera();

	first->moveTo(25,0,0);
	first->rotate(0,20);
	first->rotate(1,-45);

	second->moveTo(20,20,20);

	third->moveTo(0,30,0);
	
	scene_cameras.push_back(first);
	scene_cameras.push_back(second);
	scene_cameras.push_back(third);

	activateCamera(0);
}

void CGFscene::init() {
	glEnable(GL_LIGHTING);

	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, CGFlight::background_ambient);  // define ambient light
	
	// Light
	float light0_pos[4] = {4.0, 6.0, 3.0, 1.0};
	CGFlight* light0 = new CGFlight(GL_LIGHT0, light0_pos);
	light0->enable();
}

void CGFscene::display() {

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// Inicializacoes da matriz de transformacoes geometricas
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	CGFscene::activeCamera->applyView();

	axis.draw();

	glutSwapBuffers();
}

void CGFscene::setInterface(CGFinterface *i)
{
	iface=i;
}
