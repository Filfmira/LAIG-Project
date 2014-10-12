#ifndef TPinterface_H
#define TPinterface_H

#include "CGFinterface.h"


class TPinterface: public CGFinterface {
	public:
		TPinterface();

		virtual void initGUI();
		virtual void processGUI(GLUI_Control *ctrl);

		//virtual void processKeyboard(unsigned char key, int x, int y);
		std::vector<GLUI_Checkbox*> lightCheckboxes;
		std::vector<GLUI_RadioButton*> camerasRadioButtons;
		GLUI_RadioGroup * camerasRadGroup;
		GLUI_RadioGroup * visualizationRadGroup;

		int visualizationId;
		int cameraId;
};


#endif
