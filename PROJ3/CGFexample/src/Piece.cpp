#include "Piece.h"

Piece::Piece(bool cor, std::string tipo){
	this->cor = cor;
	this->tipo = tipo;
	this->hexagon = Poligon(6);
}

void Piece::draw(){

/*	glPushMatrix();
		glBegin(GL_TRIANGLE_FAN);
			glVertex3d( 0.0, 0.0, 0.0);
			glVertex3d( 1.0, 0.0, 0.0);
			glVertex3d( 0.5, 0.0, 0.866);
			glVertex3d(-0.5, 0.0, 0.866);
			glVertex3d(-1.0, 0.0, 0.0);
			glVertex3d(-0.5, 0.0,-0.866);
			glVertex3d( 0.5, 0.0,-0.866);
			glVertex3d( 1.0, 0.0, 0.0);
		glEnd();
	glPopMatrix();
		
	glPushMatrix();

	glPopMatrix();

	glPushMatrix();
		glBegin(GL_TRIANGLE_FAN);
			glVertex3d( 0.0, 0.5, 0.0);
			glVertex3d( 1.0, 0.5, 0.0);
			glVertex3d( 0.5, 0.5,-0.866);
			glVertex3d(-0.5, 0.5,-0.866);
			glVertex3d(-1.0, 0.5, 0.0);
			glVertex3d(-0.5, 0.5, 0.866);
			glVertex3d( 0.5, 0.5, 0.866);
			glVertex3d( 1.0, 0.5, 0.0);
		glEnd();
	glPopMatrix();*/
	glPushMatrix();
		glTranslated(0,0.5,0);
		this->hexagon.draw();
	glPopMatrix();

	glPushMatrix();
		glRotated(180,1,0,0);
		this->hexagon.draw();
	glPopMatrix();
}