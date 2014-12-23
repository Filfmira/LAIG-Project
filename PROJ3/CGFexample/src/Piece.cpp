#include "Piece.h"

Piece::Piece(std::string texturaTopos){
	this->hexagon = Poligon(6);
	this->appearanceLados = new Appearance("lados");
	Texture* texLados = new Texture("lados", "wood.jpg", 1, 1);
	this->appearanceLados->setTexture(texLados);

	this->appearanceTopos = new Appearance("topos");
	//printf("textura: %s\n", (char*) texturaTopos.c_str());
	Texture* texTopos = new Texture("topos",(char*) texturaTopos.c_str(), 1, 1);
	this->appearanceTopos->setTexture(texTopos);
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
		this->appearanceTopos->apply();
		glTranslated(0,0.5,0);
		this->hexagon.draw();
	glPopMatrix();

	glPushMatrix();
		this->appearanceTopos->apply();
		glRotated(180,1,0,0);
		this->hexagon.draw();
	glPopMatrix();

	std::vector<Point3D> pontos = this->hexagon.getPoints();
	for (unsigned int i = 0; i < 6; i++){
		glPushMatrix();
			this->appearanceLados->apply();
			glRotated(60.0*i,0,1,0);
			glBegin(GL_QUADS);
				glTexCoord2d(1,1);
				glVertex3d(pontos[1].x, 0.5, pontos[1].z);
				glTexCoord2d(1,0);
				glVertex3d(pontos[1].x, 0, pontos[1].z);
				glTexCoord2d(0,0);
				glVertex3d(pontos[0].x, 0, pontos[0].z);
				glTexCoord2d(0,1);
				glVertex3d(pontos[0].x, 0.5, pontos[0].z);			
			glEnd();
		glPopMatrix();
	}
}