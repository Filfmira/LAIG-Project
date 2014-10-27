#include "TPinterface.h"
#include <sstream>
#include <iostream>

#define initialId 2

TPinterface::TPinterface()
{

}

void TPinterface::initGUI()
{
	GLUI_Panel *lightsPanel= addPanel("Lights", GLUI_PANEL_EMBOSSED);

	for (unsigned int i = 0; i < 8; i++){
		if (((ProjectScene*) scene)->lights[i] != NULL){
			lightCheckboxes.push_back(addCheckboxToPanel(lightsPanel,(char*) ((ProjectScene*) scene)->lights[i]->getLightId().c_str(), 0, initialId+i));
			if (glIsEnabled(((ProjectScene*) scene)->lights[i]->getCGFlightIndex())){
				lightCheckboxes[i]->set_int_val(GL_TRUE);
			}
		}
	}


	addColumn();


	GLUI_Panel *visualizationMode= addPanel("Visualization", GLUI_PANEL_EMBOSSED);
	visualizationRadGroup = addRadioGroupToPanel (visualizationMode, &visualizationId, initialId+8);
	GLUI_RadioButton * 	wireMode = addRadioButtonToGroup (visualizationRadGroup, "WireFrame");
	GLUI_RadioButton * 	textMode = addRadioButtonToGroup (visualizationRadGroup, "Fill");
	wireMode->set_id(0);
	textMode->set_id(1);
	visualizationRadGroup->set_int_val(1);
	((ProjectScene*) scene)->wireFrame = false;

	addColumn();

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

