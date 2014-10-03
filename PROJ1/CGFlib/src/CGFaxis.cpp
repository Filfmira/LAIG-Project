#include "CGFaxis.h"

#include "CGFapplication.h"

CGFaxis::CGFaxis(): CGFobject() {
	radius_begin = CG_CGFaxis_DEFAULT_RADBEG;
	radius_end = CG_CGFaxis_DEFAULT_RADEND;
	lenght = CG_CGFaxis_DEFAULT_LENGTH;
	nslices = CG_CGFaxis_DEFAULT_SLICES;
	nstacks = CG_CGFaxis_DEFAULT_STACKS;

	ball_radius = CG_CGFaxis_DEFAULT_SPHRAD;
	ball_slices = CG_CGFaxis_DEFAULT_SPHSLI;
	ball_stacks = CG_CGFaxis_DEFAULT_SPHSTK;

	glu_quadric = gluNewQuadric();
}


CGFaxis::~CGFaxis() {
	gluDeleteQuadric(glu_quadric);
}

void CGFaxis::draw() {
	glEnable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();

		// esfera representativa da origem das coordenadas
		glColor3f(1.0,1.0,1.0);		// cor branca
		gluSphere(glu_quadric, ball_radius, ball_slices, ball_radius);

		// cilindro representativo do eixo X
		glColor3f(1.0,0.0,0.0);		// vermelho
		glPushMatrix();
			glRotated(90.0, 0.0,1.0,0.0);
			gluCylinder(glu_quadric, radius_begin, radius_end, lenght, nslices, nstacks);   // nao tem bases
		glPopMatrix();

		// cilindro representativo do eixo Y
		glColor3f(0.0,1.0,0.0);		// verde
		glPushMatrix();
			glRotated(-90.0, 1.0,0.0,0.0);
			gluCylinder(glu_quadric, radius_begin, radius_end, lenght, nslices, nstacks);   // nao tem bases
		glPopMatrix();
	
		// cilindro representativo do eixo Z
		glColor3f(0.0,0.0,1.0);		// azul
		glPushMatrix();
			// nao necessita rotacao... glRotated(...);
			gluCylinder(glu_quadric, radius_begin, radius_end, lenght, nslices, nstacks);   // nao tem bases
		glPopMatrix();

	glPopMatrix();

	glColor3f(1.0,1.0,1.0);
	glDisable(GL_COLOR_MATERIAL);

}