#include "CGFappearance.h"

#include "CGFapplication.h"


#define CG_GLMAT_COLOR 0
#define CG_GLMAT_MATERIAL 1
#define CG_GLMAT_TEXTURE 2

#include "imagetools.h"

#include <iostream>


CGFappearance::CGFappearance()
{
	color[0] = 0.5;
	color[1] = 0.5;
	color[2] = 0.5;
	color[3] = 1;

	materialType = CG_GLMAT_COLOR;

	texture=NULL;
}

CGFappearance::CGFappearance(float* a, float* d, float* sp, float sh) 
{
	ambient[0] = a[0];
	ambient[1] = a[1];
	ambient[2] = a[2];
	ambient[3] = a[3];

	diffuse[0] = d[0];
	diffuse[1] = d[1];
	diffuse[2] = d[2];
	diffuse[3] = d[3];
		
	specular[0] = sp[0];
	specular[1] = sp[1];
	specular[2] = sp[2];
	specular[3] = sp[3];

	shininess = sh;

	materialType = CG_GLMAT_MATERIAL;

	texture=NULL;
}

CGFappearance::CGFappearance(float* c) 
{
	color[0] = c[0];
	color[1] = c[1];
	color[2] = c[2];
	color[3] = c[3];

	materialType = CG_GLMAT_COLOR;

	texture=NULL;
	privateTexture=false;
}

CGFappearance::~CGFappearance() 
{
	if (texture!=NULL && privateTexture==true)
		delete(texture);
}

void CGFappearance::apply() 
{

	if(materialType==CG_GLMAT_MATERIAL) {
		glDisable(GL_COLOR_MATERIAL);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR,  specular);
		glMaterialfv(GL_FRONT, GL_DIFFUSE,   diffuse);
		glMaterialfv(GL_FRONT, GL_AMBIENT,   ambient);
	}
	else if(materialType==CG_GLMAT_COLOR) {
		glEnable(GL_COLOR_MATERIAL);
		glColor4fv(color);
	}

	if (texture!=NULL)
	{
		glEnable(GL_TEXTURE_2D);
		texture->apply();	       
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrap);
	}
	else
		glDisable(GL_TEXTURE_2D);	       
}

void CGFappearance::setAmbient(float* li) 
{
	ambient [0] = li[0];
	ambient [1] = li[1];
	ambient [2] = li[2];
	ambient [3] = li[3];
	materialType = CG_GLMAT_MATERIAL;
}

void CGFappearance::setDiffuse(float* li) 
{
	diffuse [0] = li[0];
	diffuse [1] = li[1];
	diffuse [2] = li[2];
	diffuse [3] = li[3];
	materialType = CG_GLMAT_MATERIAL;
}

void CGFappearance::setSpecular(float* li) 
{
	specular [0] = li[0];
	specular [1] = li[1];
	specular [2] = li[2];
	specular [3] = li[3];
	materialType = CG_GLMAT_MATERIAL;
}

void CGFappearance::setShininess(float shi) 
{
	shininess = shi;
	materialType = CG_GLMAT_MATERIAL;
}

void CGFappearance::setTexture(string t) 
{
	setTexture(new CGFtexture(t));

	privateTexture=true;
}

void CGFappearance::setTexture(CGFtexture *tp) 
{
	if (texture!=NULL && privateTexture==true)
		delete(texture);

	texture=tp;
}

void CGFappearance::setTextureWrap(int s, int t)
{
		sWrap=s;
		tWrap=t;
}

CGFappearance::CGFappearance(string tex, int s, int t)
{
	color[0] = 0.5;
	color[1] = 0.5;
	color[2] = 0.5;
	color[3] = 1;

	materialType = CG_GLMAT_COLOR;

	texture=NULL;
	privateTexture=false;

	try {
		setTexture(tex);
		setTextureWrap(s,t);
	}
	catch (GLexception& ex) {
		throw ex;
	}
	catch (std::exception&) {
		throw GLexception("CGFappearance::constructor for texture failed");
	}
}