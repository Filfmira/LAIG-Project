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

	int increment = 8;
	////////////VISUALIZATION MODE ////////////////
	GLUI_Panel *visualizationMode= addPanel("Visualization", GLUI_PANEL_EMBOSSED);
	visualizationRadGroup = addRadioGroupToPanel (visualizationMode, &visualizationId, initialId+increment); increment++;// id = init+8
	GLUI_RadioButton * 	wireMode = addRadioButtonToGroup (visualizationRadGroup, "WireFrame");
	GLUI_RadioButton * 	textMode = addRadioButtonToGroup (visualizationRadGroup, "Fill");
	wireMode->set_id(0);
	textMode->set_id(1);
	visualizationRadGroup->set_int_val(1);
	((ProjectScene*) scene)->wireFrame = false;

	
	/////CAMERAS/////////////
	GLUI_Panel *cameras= addPanel("Cameras", GLUI_PANEL_EMBOSSED);
	camerasRadGroup = addRadioGroupToPanel (cameras, &cameraId, initialId+increment); increment++;// id = init+9

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
	
	addColumn();
	GLUI_Panel *restarts = addPanel("Restarts", GLUI_PANEL_EMBOSSED);
	addButtonToPanel (restarts, "JvJ", initialId+(increment++)); // id = init+10
	addButtonToPanel (restarts, "JvC", initialId+(increment++)); // id = init+11
	addButtonToPanel (restarts, "CvC", initialId+(increment++)); // id = init+12
	

	addColumn();
	GLUI_Panel *undoAndSpinnersPannel = addPanel("Undo",  GLUI_PANEL_NONE);
	addButtonToPanel (undoAndSpinnersPannel, "UNDO", initialId+(increment++)); // id = init+13
	animationSpinner = addSpinnerToPanel(undoAndSpinnersPannel, "animationSpeed", 2,NULL, initialId+(increment++)); // id = init+14
	animationSpinner->set_float_val(((ProjectScene*) scene)->timeSpan);
	clockSpinner = addSpinnerToPanel(undoAndSpinnersPannel, "clock", 2,NULL, initialId+(increment++)); // id = init+15
	clockSpinner->set_float_val(((ProjectScene*) scene)->clock->getSpan());
	zoomSpinner = addSpinnerToPanel(undoAndSpinnersPannel, "zoom", 2,NULL, initialId+(increment++)); // id = init+16
	zoomSpinner->set_float_val(((ProjectScene*) scene)->perspective->getZoom());
}


void TPinterface::processGUI(GLUI_Control *ctrl)
{
	printf ("GUI control id: %d\n  ",ctrl->user_id);
	
	int temp = ctrl->user_id - initialId;
	int downLimit = 1;
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

	////////////////// RESTARTS /////////////////
	case(initialId+10): ((ProjectScene*) scene)->gameOver = "restart";
		((ProjectScene*) scene)->modoDeJogo = "JvJ"; break;
	case(initialId+11): ((ProjectScene*) scene)->gameOver = "restart";
		((ProjectScene*) scene)->modoDeJogo = "JvC"; break;
	case(initialId+12): ((ProjectScene*) scene)->gameOver = "restart";
		((ProjectScene*) scene)->modoDeJogo = "CvC"; break;

	////////////////// UNDO /////////////////
	case(initialId+13): ((ProjectScene*) scene)->undo(); break;


	///////TIME SPAN ANIMATIONS//////
	case(initialId+14):
		if (val > 60) {animationSpinner->set_float_val(60); val = 60;}
		else if (val < 1) {animationSpinner->set_float_val(1); val = 1;}
		((ProjectScene*) scene)->timeSpan = val;
		if (this->clockSpinner->get_float_val() < val+5) clockSpinner->set_float_val(val+5);
		break;

	//////CLOCK TIMER SPINNER //////
	case(initialId+15):
		downLimit = ((ProjectScene*) scene)->timeSpan+5;
		if (val > 180) {clockSpinner->set_float_val(180); val = 180;}
		else if (val < downLimit) {clockSpinner->set_float_val(downLimit); val = downLimit;}
		((ProjectScene*) scene)->clock->setSpan(val);
		break;

	//////ZOOM SPINNER //////
	case(initialId+16):
		if (val > 50) {zoomSpinner->set_float_val(50); val = 50;}
		else if (val < 15) {zoomSpinner->set_float_val(15); val = 15;}
		this->zoomSpinner->set_float_val(val);
		((ProjectScene*) scene)->perspective->setZoom(val);
		break;
			
	};
}

//////////////////////////////////////////////////////
///////////////////// PICKING ////////////////////////
//////////////////////////////////////////////////////


// buffer to be used to store the hits during picking
#define BUFSIZE 256
GLuint selectBuf[BUFSIZE];
int xAnterior=0;
int yAnterior=0;

void TPinterface::processMouse(int button, int state, int x, int y) 
{
	CGFinterface::processMouse(button,state, x, y);

	// do picking on mouse press (GLUT_DOWN)
	// this could be more elaborate, e.g. only performing picking when there is a click (DOWN followed by UP) on the same place
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		xAnterior=x;
		yAnterior=y;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && xAnterior==x && yAnterior==y)
	{
		performPicking(x,y);
	}
		
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
			printf("%d ",selected[i]);
		printf("\n");
		if(nselected==2)
			((ProjectScene*) scene)->setSelectedPiece(selected[1], selected[0]);
		else
			((ProjectScene*) scene)->setTypePiece(selected[0]);
	

	}
	else{
		printf("Nothing selected while picking \n");
		((ProjectScene*) scene)->noneSelected(); //reset selected piece
	}
}
