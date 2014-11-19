#include "Vehicle.h"


Vehicle::Vehicle()
{
	char* text="rocket.png";
	t=new Texture(text,text,1,1);
	app=Appearance("ola");
	app.setTexture(t);
	float amb[4]={0.3,0.3,0.3,0.3};
	app.setAmbient(&amb[0]);
	app.setDiffuse(&amb[0]);
	app.setShininess(0.5);
	app.setSpecular(&amb[0]);
	deg2rad=acos(-1.0)*180;
	float ang=acos(-1.0)/6;
	float seno=sin(ang);
	float coseno=cos(ang*2);
	cout<<seno<<endl;
	//cin.get();
	ponta=Cylinder(1.5,0,2,20,20);
	corpo=Cylinder(1,1,8,80,20);
	laterais=Cylinder(1,0.5,2,20,20);
	float lol[48] = {
		3.5,0,0,
		3,0,0,
		2,0,0,
		1.5,0,0,

		4,0,0.3,
		3,1.5,0.3,
		2,1.5,0.3,
		1,0,0.3,

		4,0,1,
		3,0.5,1,
		2,0.5,1,
		1,0,1,

		3,0,4,
		2.8,0,4,
		2.2,0,4,
		2,0,4,

    };
	p=Patch(3,20,20,"fill",&lol[0]);

	float clol[48] = {
		0,0,0,
		0,0,0,
		0,0,0,
		0,0,0,

		1,0.8,0,
		1,0.8,seno*0.8,
		1,seno*0.8,0.8,
		1,0,0.8,
			
		5,2,0,
		5,2,seno*2,
		5,seno*2,2,
		5,0,2,
				
		15,0.4,0,
		15,0.4,seno*0.4,
		15,seno*0.4,0.4,
		15,0,0.4,


    };
	c=Patch(3,20,20,"fill",&clol[0]);

}

void Vehicle::draw(){
		glPushMatrix();
		app.apply();
		glScaled(0.5,0.5,0.5);
		glRotated(90,0,1,0);
		glTranslated(-7.5,0,0);
			glPushMatrix();
			c.draw();
			glPopMatrix();

			glPushMatrix();
			glRotated(90,1,0,0);
			c.draw();
			glPopMatrix();

				glPushMatrix();
			glRotated(180,1,0,0);
			c.draw();
			glPopMatrix();

			glPushMatrix();
			glRotated(-90,1,0,0);
			c.draw();
			glPopMatrix();
		



			glPushMatrix();
			glTranslated(8,0,0);
			pata();
			glPopMatrix();


			glPushMatrix();
			glRotated(360/3,1,0,0);
			glTranslated(8,0,0);
			pata();
			glPopMatrix();

			glPushMatrix();
			glRotated(360/3*2,1,0,0);
			glTranslated(8,0,0);
			pata();
			glPopMatrix();


		glPopMatrix();
		
	}

void Vehicle::pata()
	{
		glPushMatrix();

		glScaled(4,4,0);

			glPushMatrix();
				glNormal3d(0,0,1) ;
				glBegin(GL_POLYGON);
				glTexCoord2d(0,0);
					glVertex3d(0,0,0);
					glTexCoord2d(1,0);
					glVertex3d(1.2,0,0);
					glTexCoord2d(1,1);
					glVertex3d(1.2,0.5,0);
					glTexCoord2d(0,1);
					glVertex3d(0.8,0.5,0);
					glTexCoord2d(0,0);
					glVertex3d(0,0,0);
				glEnd();
			
			glPopMatrix();

			glPushMatrix();
				glNormal3d(0,0,1) ;
				glBegin(GL_POLYGON);
				glTexCoord2d(0,0);
					glVertex3d(1.2,0,0);
					glTexCoord2d(1,0);
					glVertex3d(2.4,0.3,0);
					glTexCoord2d(1,1);
					glVertex3d(2.5,0.5,0);
					glTexCoord2d(0,1);
					glVertex3d(1.2,.5,0);
				glEnd();

			glPopMatrix();

		glPopMatrix();

		glPushMatrix();

		glScaled(4,4,0);

			glPushMatrix();
				glNormal3d(0,0,1) ;
				glBegin(GL_POLYGON);
					glTexCoord2d(0,0);
					glVertex3d(0,0,0);
					glTexCoord2d(1,0);
					glVertex3d(0.8,0.5,0);
					glTexCoord2d(1,1);
					glVertex3d(1.2,0.5,0);
					glTexCoord2d(0,1);
					glVertex3d(1.2,0,0);
					glTexCoord2d(0,0);
					glVertex3d(0,0,0);
				glEnd();
			
			glPopMatrix();

			glPushMatrix();
				glNormal3d(0,0,1) ;
				glBegin(GL_POLYGON);
				glTexCoord2d(0,0);
					glVertex3d(1.2,0,0);
					glTexCoord2d(0,1);
					glVertex3d(1.2,.5,0);
					glTexCoord2d(1,1);
					glVertex3d(2.5,0.5,0);
					glTexCoord2d(1,0);
					glVertex3d(2.4,0.3,0);
					
				glEnd();

			glPopMatrix();

		glPopMatrix();




	}