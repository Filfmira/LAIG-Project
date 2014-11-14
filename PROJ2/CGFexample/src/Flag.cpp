#include <stdlib.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>

#include "Flag.h"



FlagShader::FlagShader(char* texture){
	//init("appValues.vert", "dualVaryingColor.frag");
	init("textureDemo2.vert", "textureDemo2.frag");

	CGFshader::bind();

	this->plane = new Plane(50);
	
	// Store Id for the uniform "normScale", new value will be stored on bind()

	this->texture=new CGFtexture(texture);

	// get the uniform location for the sampler
	textureLoc = glGetUniformLocation(id(), "texture");

	// set the texture id for that sampler to match the GL_TEXTUREn that you 
	// will use later e.g. if using GL_TEXTURE0, set the uniform to 0
	glUniform1i(textureLoc, 0);

		
	startTime = clock();
	this->timer = 0;
	timerLoc = glGetUniformLocation(id(), "time");	
	glUniform1i(timerLoc, 1);


	// Initialize parameter in memory
	this->speed=0.0;
	speedLoc = glGetUniformLocation(id(), "speed");	
	glUniform1i(speedLoc, 2);

}


void FlagShader::bind(void)
{
	CGFshader::bind();
	//timer = time(NULL) - startTime;
	 timer = (((float)clock() - (float)startTime) / 1000000.0F ) * 1000;   
	// update uniforms
	glUniform1f(timerLoc, timer);
	glUniform1f(speedLoc, speed);
	// make sure the correct texture unit is active
	glActiveTexture(GL_TEXTURE0);

	// apply/activate the texture you want, so that it is bound to GL_TEXTURE0
	texture->apply();

}

void FlagShader::draw(){
	this->bind();
	this->plane->draw();
	this->unbind();
}


char* FlagShader::getNome(){
	return "flag";
}

void FlagShader::setSpeed(float s){
	this->speed = s;
}