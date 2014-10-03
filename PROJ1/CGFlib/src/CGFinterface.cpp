#include "CGFinterface.h"
#include "CGFscene.h"
#include "CGFapplication.h"
#include <GL/glui.h>

#define MOUSE_ROTATE_FACTOR 0.5
#define MOUSE_PAN_FACTOR 0.05
#define MOUSE_ZOOM_FACTOR 0.5

int CGFinterface::modifiers=0;

CGFinterface::CGFinterface() {
}

CGFinterface::~CGFinterface() {
}

CGFinterface * CGFinterface::activeInterface=NULL;

void CGFinterface::setScene(CGFscene *sc) 
{
	scene=sc;
}


void CGFinterface::init(int parent) {

	glui_window = GLUI_Master.create_glui_subwindow(parent, GLUI_SUBWINDOW_BOTTOM);
	GLUI_Master.set_glutKeyboardFunc(CGFinterface::preprocessKeyboard);

	GLUI_Master.set_glutMouseFunc(CGFinterface::preprocessMouse);
	glutMotionFunc(CGFinterface::preprocessMouseMoved);
	glutPassiveMotionFunc(CGFinterface::preprocessPassiveMouseMoved);
	displacementX = 0;
	displacementY = 0;

	pressing_left=false;
	pressing_right=false;
	pressing_middle=false;

	prev_X = 0;
	prev_Y = 0;
}

void CGFinterface::initGUI()
{
};


GLUI_Checkbox* CGFinterface::addCheckbox(char* name, int* value, int id )
{
	return glui_window->add_checkbox(name, value, id, CGFinterface::preprocessGUI);
}

GLUI_Checkbox* CGFinterface::addCheckboxToPanel(GLUI_Panel *p,char* name, int* value,int id )
{
	return glui_window->add_checkbox_to_panel(p,name, value,id,CGFinterface::preprocessGUI);
}

GLUI_Button* CGFinterface::addButton(char* name,int id)
{
	return glui_window->add_button(name, id, CGFinterface::preprocessGUI);
}

GLUI_Button* CGFinterface::addButtonToPanel(GLUI_Panel *p,char* name, int id  )
{
	return glui_window->add_button_to_panel(p,name, id, CGFinterface::preprocessGUI);
}

void CGFinterface::addColumn()
{
	glui_window->add_column();
}

void CGFinterface::addColumnToPanel(GLUI_Panel *p)
{
	glui_window->add_column_to_panel(p);
}

GLUI_EditText* CGFinterface::addEditText(char* name, char* var, int id )
{
	return glui_window->add_edittext(name,GLUI_EDITTEXT_STRING, var,id,CGFinterface::preprocessGUI);
}

GLUI_EditText* CGFinterface::addEditText(char* name, int* var, int id  )
{
	return glui_window->add_edittext(name,GLUI_EDITTEXT_INT, var,id,CGFinterface::preprocessGUI);
}

GLUI_EditText* CGFinterface::addEditText(char* name, float* var, int id  )
{
	return glui_window->add_edittext(name,GLUI_EDITTEXT_FLOAT, var,id,CGFinterface::preprocessGUI);
}

GLUI_EditText* CGFinterface::addEditTextToPanel(GLUI_Panel *p,char* name, char* var, int id  )
{
	return glui_window->add_edittext_to_panel(p,name,GLUI_EDITTEXT_FLOAT, var,id,CGFinterface::preprocessGUI);
}

GLUI_EditText* CGFinterface::addEditTextToPanel(GLUI_Panel *p,char* name, int* var, int id )
{
	return glui_window->add_edittext_to_panel(p,name,GLUI_EDITTEXT_FLOAT, var,id,CGFinterface::preprocessGUI);
}

GLUI_EditText* CGFinterface::addEditTextToPanel(GLUI_Panel *p,char* name, float* var, int id  )
{
	return glui_window->add_edittext_to_panel(p,name,GLUI_EDITTEXT_FLOAT, var,id,CGFinterface::preprocessGUI);
}

GLUI_Listbox* CGFinterface::addListbox(char* name, int* var, int id )
{
	return glui_window->add_listbox(name,var,id,CGFinterface::preprocessGUI);
}

GLUI_Listbox* CGFinterface::addListboxToPanel(GLUI_Panel *p,char* name, int* var, int id)
{
	return glui_window->add_listbox_to_panel(p,name,var,id,CGFinterface::preprocessGUI);
}

GLUI_Panel* CGFinterface::addPanel(char* name, int type )
{
	return glui_window->add_panel(name,type);
}

GLUI_Panel* CGFinterface::addPanelToPanel(GLUI_Panel *p,char* name, int type)
{
	return glui_window->add_panel_to_panel(p,name,type);
}

GLUI_RadioButton* CGFinterface::addRadioButtonToGroup(GLUI_RadioGroup * group, char * name)
{
	return glui_window->add_radiobutton_to_group(group,name);
}

GLUI_RadioGroup* CGFinterface::addRadioGroup(int *var, int id )
{
	return glui_window->add_radiogroup(var,id,CGFinterface::preprocessGUI);

}

GLUI_RadioGroup* CGFinterface::addRadioGroupToPanel(GLUI_Panel *p,int *var, int id)
{
	return glui_window->add_radiogroup_to_panel(p,var,id,CGFinterface::preprocessGUI);

}

GLUI_Rollout* CGFinterface::addRollout(char *name, int open, int type )
{
	return glui_window->add_rollout(name,open,type);
}

GLUI_Rollout* CGFinterface::addRolloutToPanel(GLUI_Panel* p,char *name, int open, int type )
{
	return glui_window->add_rollout_to_panel(p,name,open,type);
}

void CGFinterface::addSeparator()
{
	return glui_window->add_separator();
}

void CGFinterface::addSeparatorToPanel(GLUI_Panel *p)
{
	return glui_window->add_separator_to_panel(p);
}

GLUI_Rotation* CGFinterface::addRotation(char* name, float* var, int id)
{
	return glui_window->add_rotation(name,var,id,CGFinterface::preprocessGUI);
}

GLUI_Rotation* CGFinterface::addRotationToPanel(GLUI_Panel *p,char* name, float* var, int id)
{
	return glui_window->add_rotation_to_panel(p,name,var,id,CGFinterface::preprocessGUI);
}


GLUI_Spinner* CGFinterface::addSpinner(char* name, int type, int* var, int id)
{
	return glui_window->add_spinner(name,type,var,id,CGFinterface::preprocessGUI);
}

GLUI_Spinner* CGFinterface::addSpinnerToPanel(GLUI_Panel* p,char* name, int type, int* var, int id )
{
	return glui_window->add_spinner_to_panel(p,name,type,var,id,CGFinterface::preprocessGUI);
}

GLUI_StaticText* CGFinterface::addStaticText(char* name)
{
	return glui_window->add_statictext(name);
}

GLUI_StaticText* CGFinterface::addStaticTextToPanel(GLUI_Panel *p,char* name)
{
	return glui_window->add_statictext_to_panel(p,name);
}

GLUI_Translation* CGFinterface::addTranslationToPanel(GLUI_Panel* p,char* name, int type, float* var, int id)
{
	return glui_window->add_translation_to_panel(p,name,type,var,id,CGFinterface::preprocessGUI);
}

GLUI_Translation* CGFinterface::addTranslation(char* name, int type, float* var, int id)
{
	return glui_window->add_translation(name,type,var,id,CGFinterface::preprocessGUI);
}

void CGFinterface::preprocessKeyboard(unsigned char key, int x, int y)
{
	modifiers=glutGetModifiers();
	activeInterface->processKeyboard(key,x,y); 
}

void CGFinterface::preprocessMouse(int button, int state, int x, int y) {
	modifiers=glutGetModifiers();
	activeInterface->processMouse(button, state, x,y); 
}

void CGFinterface::preprocessMouseMoved(int x, int y) {
	activeInterface->processMouseMoved(x,y);
}

void CGFinterface::preprocessPassiveMouseMoved(int x, int y)
{ 
	activeInterface->processPassiveMouseMoved(x,y); 
}

void CGFinterface::syncVars()
{
	/****************************************************************/
	/*            This demonstrates GLUI::sync_live()               */
	/*   We change the value of a variable that is 'live' to some   */
	/*   control.  We then call sync_live, and the control          */
	/*   associated with that variable is automatically updated     */
	/*   with the new value.  This frees the programmer from having */
	/*   to always remember which variables are used by controls -  */
	/*   simply change whatever variables are necessary, then sync  */
	/*   the live ones all at once with a single call to sync_live  */
	/****************************************************************/

	glui_window->sync_live();
}

void CGFinterface::preprocessGUI(GLUI_Control *ctrl)
{
	activeInterface->processGUI(ctrl); 
}



// Default handlers (to be overriden by sub-class)

void CGFinterface::processKeyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 'w':
			scene->activeCamera->setWalkMode();
			break;
		case 'e':
			scene->activeCamera->setExamineMode();
			break;

		case 's':
			CGFapplication::snapshot();
			break;

		case 27:		// tecla de escape termina o programa
			exit(0);
			break;
	}
}

void CGFinterface::processMouse(int button, int state, int x, int y) {

	
	prev_X = x;
	prev_Y = y;

	pressing_left = (button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN);
	pressing_right = (button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN);
	pressing_middle = (button == GLUT_MIDDLE_BUTTON) && (state == GLUT_DOWN);

	glutPostRedisplay();
	
}

void CGFinterface::processMouseMoved(int x, int y) {
	
	// pedido de refrescamento da janela
	displacementX = x- prev_X;
	displacementY = y- prev_Y;

	if(pressing_left && modifiers==0)
	{
		scene->activeCamera->rotate(CG_CGFcamera_AXIS_X, displacementY*MOUSE_ROTATE_FACTOR);
		scene->activeCamera->rotate(CG_CGFcamera_AXIS_Y, displacementX*MOUSE_ROTATE_FACTOR);
	}
	else if(pressing_right && modifiers==0)
	{
		scene->activeCamera->translate(CG_CGFcamera_AXIS_X, displacementX*MOUSE_PAN_FACTOR);
		scene->activeCamera->translate(CG_CGFcamera_AXIS_Y, -displacementY*MOUSE_PAN_FACTOR);
	}
	else if(pressing_middle || (pressing_left && modifiers & GLUT_ACTIVE_CTRL))
	{
		scene->activeCamera->translate(CG_CGFcamera_AXIS_Z, displacementY*MOUSE_ZOOM_FACTOR);
	}


	prev_X = x;
	prev_Y = y;

	glutPostRedisplay();				

}

void CGFinterface::processPassiveMouseMoved(int x, int y) {

	// pedido de refrescamento da janela
	glutPostRedisplay();				
}

void CGFinterface::processGUI(GLUI_Control *ctrl)
{ 
}

