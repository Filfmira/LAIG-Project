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

