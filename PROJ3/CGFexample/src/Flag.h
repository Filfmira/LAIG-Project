#ifndef _FLAG_H_
#define _FLAG_H_


#include "CGFshader.h"
#include "CGFtexture.h"
#include <time.h>       /* time_t, struct tm, difftime, time, mktime */
#include "Primitives.h"
#include "Evaluator.h"
class FlagShader: public CGFshader, public Primitive
{

public:
	FlagShader(char* texture);
	virtual void bind(void);
	void draw(); //virtual de Primitive
	char* getNome(); //virtual de Primitive
	Plane* plane;
	void setSpeed(float s);
private:
	float speed;
	float timer;
	clock_t startTime;
	CGFtexture * texture;

	GLint textureLoc;
	GLint timerLoc;
	GLint speedLoc;
};




#endif