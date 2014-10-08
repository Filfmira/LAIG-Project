#include "Lights.h"
#include <sstream>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>

Light::Light(string tipo, int lightId, bool enabled, bool marker, float pos[4], 
		float target[3], float angle, float exponent,
		float* ambient, float* diffuse, float* specular){
	
	this->tipo = tipo;	
	this->lightId = GL_LIGHT0 + lightId;
	this->enabled = enabled;
	this->marker = marker;
	
	this->pos[0] = pos[0];
	this->pos[1] = pos[1];
	this->pos[2] = pos[2];
	this->pos[3] = 1;

	this->angle = angle;
	this->exponent = exponent;
	
	/*if (tipo == "spot"){
		this->target[0] = target[0];
		this->target[1] = target[1];
		this->target[2] = target[2];
	}*/

	if (tipo == "spot"){
		this->target[0] = target[0]-pos[0];
		this->target[1] = target[1]-pos[1];
		this->target[2] = target[2]-pos[2];
		
		light = new CGFlight(this->lightId, this->pos, this->target);
		
		glLightf(lightId + GL_LIGHT0, GL_SPOT_EXPONENT, exponent);
		/*light->setKc(exponent);
		light->setKl(exponent);
		light->setKq(exponent);*/

		light->setAngle(angle);
	}
	else light = new CGFlight(this->lightId, this->pos);

	
	
	
	

	printf( "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
	printf("Ambiente: %f %f %f \n", ambient[0],ambient[1],ambient[2]);
	printf( "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");

	light->setAmbient(ambient);
	light->setDiffuse(diffuse);
	light->setSpecular(specular);

	if (enabled)
		light->enable();
	else light->disable();
}

void Light::apply(){

}


string Light::showLight(){
	ostringstream ss;

	ss << "Luz: " << lightId << " \n";
	ss << "	enabled: " << (enabled? "true" : "false") << "\n";
	ss << "	marker: " << (enabled? "true" : "false") << "\n";
	ss << "	angle: " << angle << "\n";
	ss << "	exponent: " << exponent << "\n";
	ss << "	pos: " << pos[0] << " " << pos[1] << " " << pos[2] << "\n";
	if (tipo == "spot"){
		ss << "	target: " << target[0] << " " << target[1] << " " << target[2] << "\n";
	}
	ss << "\n =============================== \n\n";
	return ss.str();
}

Light::~Light(){
	delete(light);
}