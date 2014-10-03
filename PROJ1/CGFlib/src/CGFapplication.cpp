#ifndef CGF_NO_SHADERS
#include <GL/glew.h>
#endif
#include "CGFapplication.h"
#include <GL/glut.h>
#include <GL/glui.h>

#include "version.h"

#include <stdlib.h>
using namespace std;

#include "imagetools.h"

#ifdef _WIN32
	#include <Windows.h>
#else
	#include <sys/time.h>
#endif

#define CG_GLAPP_DISPLAY_MODE GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA
#define CG_GLAPP_INIT_WIDTH 500
#define CG_GLAPP_INIT_HEIGHT 500
#define CG_GLAPP_INIT_POSX 200
#define CG_GLAPP_INIT_POSY 200
#define CG_GLAPP_REDBITS 8
#define CG_GLAPP_BLUEBITS 8
#define CG_GLAPP_GREENBITS 8

int CGFapplication::app_window = 0;

float CGFapplication::xy_aspect = 0;

CGFapplication * CGFapplication::activeApp = NULL;

int CGFapplication::width = 0;
int CGFapplication::height = 0;
int CGFapplication::vpx = 0;
int CGFapplication::vpy = 0;
int CGFapplication::vpw = 0;
int CGFapplication::vph = 0;

bool CGFapplication::refreshRequested=false;



CGFapplication::CGFapplication() {
	char idstr[256];
	strcpy(idstr,CGFId+3);
	idstr[strlen(idstr)-1]=0;

	printf("CGF (Computer Graphics @ FEUP) library %s %s\n",(CGFVersion[0]=='$')?"":CGFVersion, idstr);

	CGFapplication::app_window = NULL;
	CGFapplication::app_scene = NULL;
	app_interface = NULL;

	activate();
}

void CGFapplication::activate()
{
	CGFapplication::activeApp=this;
}

void CGFapplication::setScene(CGFscene* gls)
{ 
	app_scene = gls; 
	app_scene->init(); 
}

void CGFapplication::setInterface(CGFinterface* gli) 
{ 
	app_interface = gli; 
	
	app_interface->setScene(app_scene);
	app_scene->setInterface(app_interface);
	
	app_interface->init(CGFapplication::app_window); 
	app_interface->initGUI(); 
	
	CGFinterface::setActiveInterface(gli);
}

int CGFapplication::getMainWindow() 
{ 
	return CGFapplication::app_window; 
}


CGFapplication::~CGFapplication() {
}

void CGFapplication::init(int* argc, char** argv) {

	try {
		glutInit(argc, argv);
		glutInitDisplayMode(CG_GLAPP_DISPLAY_MODE);

		CGFapplication::width = CG_GLAPP_INIT_WIDTH;
		CGFapplication::height = CG_GLAPP_INIT_HEIGHT;
		glutInitWindowSize(CGFapplication::width, CGFapplication::height);
		glutInitWindowPosition(CG_GLAPP_INIT_POSX, CG_GLAPP_INIT_POSY);

		CGFapplication::app_window = glutCreateWindow(argv[0]);

		glutDisplayFunc(CGFapplication::display);
		GLUI_Master.set_glutIdleFunc(CGFapplication::gluiIdleFunction);
		GLUI_Master.set_glutReshapeFunc(CGFapplication::reshape);

		environmentInit();

#ifndef CGF_NO_SHADERS
		glewInit();
#endif
	}
	catch(std::exception&) {
		throw GLexception("CGFapplication::init failed");
	}

	return;
}

void CGFapplication::run() {
	activate();

	glutMainLoop();

	return;
}

void CGFapplication::display() {
	if (refreshRequested)
	{
		reshape(CGFapplication::width, CGFapplication::height );
		refreshRequested=false;
	}

	CGFscene *sc=CGFapplication::activeApp->app_scene;
	sc->checkUpdate();
	sc->display();
}

/* GLUI IDLE FUNCTION
 * This function may have to be re-implemented
 */
void CGFapplication::gluiIdleFunction()
{
	/* According to GLUT, the current window may be
	 * undefined when idle is called back. 
	 */
	if (glutGetWindow()!=CGFapplication::app_window) glutSetWindow(CGFapplication::app_window);  

	CGFapplication::activeApp->app_interface->syncVars();

	glutPostRedisplay();

	return;
}

/* ENVIRONMENT INITIALIZATION FUNCTION
 * This function may have to be re-implemented
 */
void CGFapplication::environmentInit() {

	//glDepthFunc(GL_LESS);		/* default */
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_DEPTH_TEST);	/* Usar z-buffer para desenhar so os objetos visiveis */

	glFrontFace(GL_CCW);		/* Faces da frente definidas em CounterClockWise */

	glCullFace(GL_BACK);		/* Cull apenas nas faces de trás */
	glEnable(GL_CULL_FACE);		/* Usar Cull para melhorar performance */

	glClearColor(0.0,0.0,0.0,1.0);	/* default - fundo preto */
	//glClearColor(1.0,1.0,1.0,1.0);	/* fundo branco */
	
	glShadeModel(GL_SMOOTH);
	//glShadeModel(GL_FLAT);

	glPolygonMode(GL_FRONT, GL_FILL);	/* preenche as faces da frente */
	//glPolygonMode(GL_FRONT, GL_LINE);	/* desenha apenas as arestas das faces */
	
}

void CGFapplication::reshape(int w, int h)
{
	CGFapplication::width = w;
	CGFapplication::height = h;

	GLUI_Master.get_viewport_area( &vpx, &vpy, &vpw, &vph );
	glViewport( vpx, vpy, vpw, vph );
	CGFapplication::xy_aspect = (float)vpw / (float)vph;

	// Inicializacoes da matriz de visualizacao
	activeApp->app_scene->activeCamera->updateProjectionMatrix(vpw,vph);

	glutPostRedisplay();
}

void CGFapplication::forceRefresh()
{
	refreshRequested=true;
}


void CGFapplication::snapshot() {
	takeSnapshot(CGFapplication::width, CGFapplication::height);
}

unsigned long CGFapplication::getTime() {
	#ifdef _WIN32
		return GetTickCount();
	#else
		// glutGet(GLUT_ELAPSED_TIME) was deprecated due to potential to overflow
		struct timespec stime;
		clock_gettime(CLOCK_MONOTONIC, &stime);

		return ((stime.tv_sec) * 1000 + stime.tv_nsec/1000000);
	#endif
}

