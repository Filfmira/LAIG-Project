#include "TPinterface.h"
#include "ProjectScene.h"
#include <sstream>

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

	int initialId = 2;

/*	for (unsigned int i = 0; i < 8; i++){
		if (((ProjectScene*) scene)->lights[i] != NULL){
			ostringstream ss;
			ss << "Light " << i;
			addCheckboxToPanel(lightsPanel,(char *) ss.str().c_str(), 0, initialId+i);
		}
	}*/


	addColumn();


	GLUI_Panel *visualizationMode= addPanel("Visualization", GLUI_PANEL_EMBOSSED);
	GLUI_RadioGroup * visualizationRadGroup = addRadioGroupToPanel (visualizationMode, &visualizationId, initialId+8);
	GLUI_RadioButton * 	wireMode = addRadioButtonToGroup (visualizationRadGroup, "WireFrame");
	GLUI_RadioButton * 	textMode = addRadioButtonToGroup (visualizationRadGroup, "Textured");
	wireMode->set_id(0);
	textMode->set_id(1);
	textMode->enable();

	addColumn();

	GLUI_Panel *cameras= addPanel("Cameras", GLUI_PANEL_EMBOSSED);
	GLUI_RadioGroup * camerasRadGroup = addRadioGroupToPanel (cameras, &cameraId, initialId+9);


}

void TPinterface::processGUI(GLUI_Control *ctrl)
{
	printf ("GUI control id: %d\n  ",ctrl->user_id);
	switch (ctrl->user_id)
	{
		/*case 1:
		{
			printf ("New Val 1(interface): %d\n",testVar);
			break;
		};*/

	
	};
}

