#ifndef TPINTERFACE_H
#define TPINTERFACE_H

#include "CGFinterface.h"
#include "ProjectScene.h"


class TPinterface: public CGFinterface {
	public:
		TPinterface();

		virtual void initGUI();
		virtual void processGUI(GLUI_Control *ctrl);

		std::vector<GLUI_Checkbox*> lightCheckboxes;
		std::vector<GLUI_RadioButton*> camerasRadioButtons;
		GLUI_RadioGroup * camerasRadGroup;
		GLUI_RadioGroup * visualizationRadGroup;

		int visualizationId;
		int cameraId;
};


#endif
