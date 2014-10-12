#include "TPinterface.h"
#include "ProjectScene.h"
#include <sstream>


#define initialId 2

TPinterface::TPinterface()
{
	//testVar=0;
}

/*
void TPinterface::processKeyboard(unsigned char key, int x, int y)
{
	// Uncomment below if you would like to process the default keys (e.g. 's' for snapshot, 'Esc' for exiting, ...)
	// CGFinterface::processKeyboard(key, x, y);

	switch(key)
	{
		case 'a':
		{
			// This is an example of accessing the associated scene
			// To test, create the function toggleSomething in your scene to activate/deactivate something
			((LightingScene *) scene)->toggleSomething();
			break;
		}
		case 'j':
			{
				((LightingScene *) scene)->incAngle();
				break;				
			}
		case 'l':
			{
				((LightingScene *) scene)->decAngle();
				break;	
			}
		case 'i':
			{
				((LightingScene *) scene)->moveFront();
				break;	
			}
		case 'k':
			{
				((LightingScene *) scene)->moveBack();
				break;	
			}
	}
}
*/




void TPinterface::initGUI()
{
	// Check CGFinterface.h and GLUI documentation for the types of controls available
	GLUI_Panel *lightsPanel= addPanel("Lights", GLUI_PANEL_EMBOSSED);

	//int initialId = 2;

	for (unsigned int i = 0; i < 8; i++){
		if (((ProjectScene*) scene)->lights[i] != NULL){
			ostringstream ss;
			ss << "Light " << ((ProjectScene*) scene)->lights[i]->getLightId()-GL_LIGHT0;
			lightCheckboxes.push_back(addCheckboxToPanel(lightsPanel,(char *) ss.str().c_str(), 0, initialId+i));
			if (glIsEnabled(((ProjectScene*) scene)->lights[i]->getLightId())){
				lightCheckboxes[i]->set_int_val(GL_TRUE);
			}
		}
	}


	addColumn();


	GLUI_Panel *visualizationMode= addPanel("Visualization", GLUI_PANEL_EMBOSSED);
	visualizationRadGroup = addRadioGroupToPanel (visualizationMode, &visualizationId, initialId+8);
	GLUI_RadioButton * 	wireMode = addRadioButtonToGroup (visualizationRadGroup, "WireFrame");
	GLUI_RadioButton * 	textMode = addRadioButtonToGroup (visualizationRadGroup, "Textured");
	wireMode->set_id(0);
	textMode->set_id(1);
	visualizationRadGroup->set_int_val(1);
	((ProjectScene*) scene)->wireFrame = false;

	addColumn();

	GLUI_Panel *cameras= addPanel("Cameras", GLUI_PANEL_EMBOSSED);
	camerasRadGroup = addRadioGroupToPanel (cameras, &cameraId, initialId+9);

	unsigned int sizeCameras = ((ProjectScene*) scene)->cameras.size();
	Camera* activeCamera = ((ProjectScene*) scene)->activeCamera;

	for (unsigned int i = 0; i < sizeCameras; i++){
		Camera* temp = &((ProjectScene*) scene)->cameras[i];
		ostringstream ss;
		ss << temp->getType() << " camera " << temp->getId();
		camerasRadioButtons.push_back(addRadioButtonToGroup (camerasRadGroup, (char*) ss.str().c_str() ));
		camerasRadioButtons.back()->set_id(i);
		if (temp->getId() == activeCamera->getId())
			camerasRadGroup->set_int_val(i);
	}

}


void TPinterface::processGUI(GLUI_Control *ctrl)
{
	printf ("GUI control id: %d\n  ",ctrl->user_id);
	
	int temp = ctrl->user_id - initialId;
	
	/////////LUZES/////////
	if (temp < 8){
		int state = lightCheckboxes[temp]->get_int_val();
		if (state)
			((ProjectScene*) scene)->lights[temp]->getLight()->enable();
		else ((ProjectScene*) scene)->lights[temp]->getLight()->disable();
		lightCheckboxes[temp]->set_int_val(lightCheckboxes[temp]->get_int_val());
	}
	//////////////////////
	


	switch (ctrl->user_id)
	{

	//////////CAMERAS////////
	case(initialId+9): 
		((ProjectScene*) scene)->activeCamera = &((ProjectScene*) scene)->cameras[ctrl->get_int_val()];
		camerasRadGroup->set_int_val(ctrl->get_int_val());
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

