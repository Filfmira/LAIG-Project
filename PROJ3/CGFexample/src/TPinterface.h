#ifndef TPINTERFACE_H
#define TPINTERFACE_H

#include "CGFinterface.h"
#include "ProjectScene.h"


class TPinterface: public CGFinterface {
	public:
		TPinterface();

		virtual void initGUI();
		virtual void processGUI(GLUI_Control *ctrl);
		virtual void processKeyboard(unsigned char key, int x, int y);

		std::vector<GLUI_Checkbox*> lightCheckboxes;
		std::vector<GLUI_RadioButton*> camerasRadioButtons;
		GLUI_RadioGroup * camerasRadGroup;
		GLUI_RadioGroup * visualizationRadGroup;
		GLUI_Spinner* speedSpinner;
		int visualizationId;
		int cameraId;
};


#endif
