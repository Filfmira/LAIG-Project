#include "CGFlight.h"

#include "CGFapplication.h"

#define CG_GLIGHT_DEFAULT_COLOR 1.0,1.0,0.0 /* yellow */
#define CG_GLIGHT_DEFAULT_BACKGROUND_AMBIENT 0.2,0.2,0.2,1.0
#define CG_GLIGHT_DEFAULT_RADIUS 0.2
#define CG_GLIGHT_DEFAULT_SLICES 8
#define CG_GLIGHT_DEFAULT_STACKS 8
#define CG_GLIGHT_DEFAULT_AMBIENT 0.0,0.0,0.0,1.0
#define CG_GLIGHT_DEFAULT_DIFFUSE 0.8,0.8,0.8,1.0
#define CG_GLIGHT_DEFAULT_SPECULAR 0.8,0.8,0.8,1.0
#define CG_GLIGHT_DEFAULT_KC 1.0
#define CG_GLIGHT_DEFAULT_KL 0.0
#define CG_GLIGHT_DEFAULT_KQ 0.0


float __tmp_0[3] = {CG_GLIGHT_DEFAULT_COLOR};
CGFappearance* CGFlight::default_light_material = new CGFappearance(__tmp_0);
float CGFlight::background_ambient[4] = {CG_GLIGHT_DEFAULT_BACKGROUND_AMBIENT};

CGFlight::CGFlight(unsigned int lightid, float* pos, float *dir) {
	try {
		id = lightid;

		position[0] = pos[0];
		position[1] = pos[1];
		position[2] = pos[2];
		position[3] = pos[3];
		material = CGFlight::default_light_material;

		if (dir==NULL)
		{
			direction[0]=0;
			direction[1]=-1;
			direction[2]=0;
		}
		else
		{
			direction[0]=dir[0];
			direction[1]=dir[1];
			direction[2]=dir[2];
		}


		float __tmp_1[4] = {CG_GLIGHT_DEFAULT_AMBIENT};
		setAmbient(__tmp_1);
		float __tmp_2[4] = {CG_GLIGHT_DEFAULT_DIFFUSE};
		setDiffuse(__tmp_2);
		float __tmp_3[4] = {CG_GLIGHT_DEFAULT_SPECULAR};
		setSpecular(__tmp_3);

		setKc(CG_GLIGHT_DEFAULT_KC);
		setKl(CG_GLIGHT_DEFAULT_KL);
		setKq(CG_GLIGHT_DEFAULT_KQ);

		enable();

		update();
	}
	catch(std::exception&) {
		throw GLexception("CGFlight::constructor failed");
	}

	glu_quadric = gluNewQuadric();
	gluQuadricOrientation(glu_quadric, GLU_INSIDE);
}

void CGFlight::setAmbient(float* li) {
	ambient [0] = li[0];
	ambient [1] = li[1];
	ambient [2] = li[2];
	ambient [3] = li[3];

	glLightfv(id, GL_AMBIENT, ambient);
}

void CGFlight::setDiffuse(float* li) {
	diffuse [0] = li[0];
	diffuse [1] = li[1];
	diffuse [2] = li[2];
	diffuse [3] = li[3];

	glLightfv(id, GL_DIFFUSE, diffuse);
}

void CGFlight::setSpecular(float* li) {
	specular [0] = li[0];
	specular [1] = li[1];
	specular [2] = li[2];
	specular [3] = li[3];

	glLightfv(id, GL_SPECULAR, specular);
}

CGFlight::~CGFlight() {
	gluDeleteQuadric(glu_quadric);
}

void CGFlight::enable() {
	enabled=true;
}

void CGFlight::disable() {
	enabled=false;
}

void CGFlight::update() {
	if (enabled)
		glEnable(id);
	else
		glDisable(id);

	glLightfv(id, GL_POSITION, position);
	glLightfv(id, GL_SPOT_DIRECTION, direction);
}

void CGFlight::draw() {
	update();

	material->apply();
	glPushMatrix();
		glTranslatef(position[0],position[1],position[2]);
		gluSphere(glu_quadric, CG_GLIGHT_DEFAULT_RADIUS, CG_GLIGHT_DEFAULT_SLICES, CG_GLIGHT_DEFAULT_STACKS);
	glPopMatrix();
}

void CGFlight::setKc(float f) { kc = f; 	glLightf(id, GL_CONSTANT_ATTENUATION, kc); }
void CGFlight::setKl(float f) { kl = f; 	glLightf(id, GL_LINEAR_ATTENUATION, kl); }
void CGFlight::setKq(float f) { kq = f; 	glLightf(id, GL_QUADRATIC_ATTENUATION, kq); }

void CGFlight::setAngle(float a) { angle = a; 	glLightf(id, GL_SPOT_CUTOFF, angle); }


