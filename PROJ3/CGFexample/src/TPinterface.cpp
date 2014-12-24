#include "TPinterface.h"
#include <sstream>
#include <iostream>
#include "Socket.h"
#define initialId 2

TPinterface::TPinterface()
{

}

void TPinterface::processKeyboard(unsigned char key, int x, int y)
{
	// Uncomment below if you would like to process the default keys (e.g. 's' for snapshot, 'Esc' for exiting, ...)
	//CGFinterface::processKeyboard(key, x, y);
	printf("char: %x\n", key & 0xff);
	switch(key)
	{
		case 'a':
		{
			//((LightingScene *) scene)->toggleSomething();
			break;
		}
		case 27:
		{
			quit(); //socket quit
			printf("ESC\nExiting program...\n");
			exit(0);
			break;
		}
	}
}



void TPinterface::initGUI()
{


	////////////VISUALIZATION MODE ////////////////
	GLUI_Panel *visualizationMode= addPanel("Visualization", GLUI_PANEL_EMBOSSED);
	visualizationRadGroup = addRadioGroupToPanel (visualizationMode, &visualizationId, initialId+8);
	GLUI_RadioButton * 	wireMode = addRadioButtonToGroup (visualizationRadGroup, "WireFrame");
	GLUI_RadioButton * 	textMode = addRadioButtonToGroup (visualizationRadGroup, "Fill");
	wireMode->set_id(0);
	textMode->set_id(1);
	visualizationRadGroup->set_int_val(1);
	((ProjectScene*) scene)->wireFrame = false;


	/////CAMERAS/////////////
	GLUI_Panel *cameras= addPanel("Cameras", GLUI_PANEL_EMBOSSED);
	camerasRadGroup = addRadioGroupToPanel (cameras, &cameraId, initialId+9);

	unsigned int sizeCameras = ((ProjectScene*) scene)->cameras.size();
	Camera* activeCamera = ((ProjectScene*) scene)->activeCamera;

	camerasRadioButtons.push_back(addRadioButtonToGroup (camerasRadGroup, "Standard"));
	camerasRadioButtons.back()->set_id(0);


	for (unsigned int i = 0; i < sizeCameras; i++){
		Camera* temp = &((ProjectScene*) scene)->cameras[i];
		ostringstream ss;
		ss << temp->getType() << " camera " << temp->getId();
		camerasRadioButtons.push_back(addRadioButtonToGroup (camerasRadGroup, (char*) ss.str().c_str() ));
		camerasRadioButtons.back()->set_id(i+1);
		if (temp->getId() == activeCamera->getId())
			camerasRadGroup->set_int_val(i+1);
	}
	

}


void TPinterface::processGUI(GLUI_Control *ctrl)
{
	printf ("GUI control id: %d\n  ",ctrl->user_id);
	
	int temp = ctrl->user_id - initialId;
	
	/////////LUZES/////////

	//////////////////////
	
	float val = ctrl->float_val;

	switch (ctrl->user_id)
	{
	//////////CAMERAS////////
	case(initialId+9): 
		if (ctrl->get_int_val() == 0)
			((ProjectScene*) scene)->activeCamera = NULL;
		else{
			((ProjectScene*) scene)->activeCamera = &((ProjectScene*) scene)->cameras[ctrl->get_int_val()-1];
			camerasRadGroup->set_int_val(ctrl->get_int_val());
		}
		break;
	/////////////////////////


	case(initialId+8):
		printf("|||| id: %d\n", ctrl->get_int_val());
		if (ctrl->get_int_val() == 0){
			((ProjectScene*) scene)->wireFrame = true;
		}
		else ((ProjectScene*) scene)->wireFrame = false;
		break;

	};
}

//////////////////////////////////////////////////////
///////////////////// PICKING ////////////////////////
//////////////////////////////////////////////////////


// buffer to be used to store the hits during picking
#define BUFSIZE 256
GLuint selectBuf[BUFSIZE];

void TPinterface::processMouse(int button, int state, int x, int y) 
{
	CGFinterface::processMouse(button,state, x, y);

	// do picking on mouse press (GLUT_DOWN)
	// this could be more elaborate, e.g. only performing picking when there is a click (DOWN followed by UP) on the same place
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		performPicking(x,y);
}

void TPinterface::performPicking(int x, int y) 
{
	// Sets the buffer to be used for selection and activate selection mode
	glSelectBuffer (BUFSIZE, selectBuf);
	glRenderMode(GL_SELECT);

	// Initialize the picking name stack
	glInitNames();

	// The process of picking manipulates the projection matrix
	// so we will be activating, saving and manipulating it
	glMatrixMode(GL_PROJECTION);

	//store current projmatrix to restore easily in the end with a pop
	glPushMatrix ();

	//get the actual projection matrix values on an array of our own to multiply with pick matrix later
	GLfloat projmat[16];
	glGetFloatv(GL_PROJECTION_MATRIX,projmat);

	// reset projection matrix
	glLoadIdentity();

	// get current viewport and use it as reference for 
	// setting a small picking window of 5x5 pixels around mouse coordinates for picking
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	// this is multiplied in the projection matrix
	gluPickMatrix ((GLdouble) x, (GLdouble) (CGFapplication::height - y), 5.0, 5.0, viewport);

	// multiply the projection matrix stored in our array to ensure same conditions as in normal render
	glMultMatrixf(projmat);

	// force scene drawing under this mode
	// only the names of objects that fall in the 5x5 window will actually be stored in the buffer
	scene->display();

	// restore original projection matrix
	glMatrixMode (GL_PROJECTION);
	glPopMatrix ();

	glFlush();

	// revert to render mode, get the picking results and process them
	GLint hits;
	hits = glRenderMode(GL_RENDER);
	processHits(hits, selectBuf);
}

void TPinterface::processHits (GLint hits, GLuint buffer[]) 
{
	GLuint *ptr = buffer;
	GLuint mindepth = 0xFFFFFFFF;
	GLuint *selected=NULL;
	GLuint nselected;

	// iterate over the list of hits, and choosing the one closer to the viewer (lower depth)
	for (int i=0;i<hits;i++) {
		int num = *ptr; ptr++;
		GLuint z1 = *ptr; ptr++;
		ptr++;
		if (z1 < mindepth && num>0) {
			mindepth = z1;
			selected = ptr;
			nselected=num;
		}
		for (int j=0; j < num; j++) 
			ptr++;
	}
	
	// if there were hits, the one selected is in "selected", and it consist of nselected "names" (integer ID's)
	if (selected!=NULL)
	{
		// this should be replaced by code handling the picked object's ID's (stored in "selected"), 
		// possibly invoking a method on the scene class and passing "selected" and "nselected"
		printf("Picked ID's: ");
		for (int i=0; i<nselected; i++)
			printf("%d, ",selected[i]);
		printf("\n");
	}
	else
		printf("Nothing selected while picking \n");	
}
