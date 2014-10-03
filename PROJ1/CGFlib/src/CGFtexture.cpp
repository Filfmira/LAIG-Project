#include "CGFtexture.h"

#include "CGFapplication.h"
#include "imagetools.h"

#include <iostream>


CGFtexture::CGFtexture()
{
	texID=-1;
}

CGFtexture::CGFtexture(string t)
{
	texID=-1;
	loadTexture(t);
}


CGFtexture::~CGFtexture() 
{
	if (texID!=-1)
		glDeleteTextures(1,&texID);
}

void CGFtexture::apply() 
{
	if (texID!=-1)
		glBindTexture(GL_TEXTURE_2D, texID);
}

void CGFtexture::loadTexture(string t) 
{
	texName=t;

	if (texID==-1)
		glGenTextures(1,&texID);

	unsigned char *data = loadRGBImage(texName.c_str(), &texWidth, &texHeight);
	if (data)
	{
		glPixelStorei(GL_UNPACK_ROW_LENGTH, texWidth);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glBindTexture(GL_TEXTURE_2D, texID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		free(data);	
	}
	else
		throw GLexception("CGFtexture::loadTexture failed");
}

