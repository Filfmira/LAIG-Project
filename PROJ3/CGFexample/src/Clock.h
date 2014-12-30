#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <string>
#include "Primitives.h"
#include "Poligon.h"

class Clock{
private:		
	float startTime;	//quando começou	
	float currentTime;	//tempo actual
	float tp;			//tempo passado
	float spanTime;		//o tempo que deve demorar cada jogada
	bool count;			//se deve continuar a contar o tempo
	bool tempoSpan;
	Cylinder cylinder;
	Cylinder topC;
	Poligon triangle;
	Torus torus;

	Appearance* appClockHand;
	Appearance* appClock;
	Appearance* appChrome;
	Appearance* appdefault;

public:
	Clock(){
		startTime=currentTime=tp=0;
		spanTime=10;
		count=true;
		cylinder= Cylinder(1,1,0.5,40,3);
		topC=Cylinder(0.15,0.15,0.4,20,3);
		triangle= Poligon(3);
		torus=Torus(0.3,1.2,40,40);

		appdefault=new Appearance("ll");
		this->appClockHand = new Appearance("appClockHand");
		this->appClock = new Appearance("appClock");
		this->appChrome = new Appearance("appChrome");
		Texture* appClockHandText=new Texture("appClockHandText", "red.png", 1, 1);
		Texture* appClockText=new Texture("appClockHandText", "clock.jpg", 1, 1);
		Texture* appChromeText=new Texture("appChromeText", "chrome.jpg", 1, 1);
		this->appClockHand->setTexture(appClockHandText);
		this->appClock->setTexture(appClockText);
		this->appChrome->setTexture(appChromeText);
		float spec[4]={0.8,0.67,0.5,1};
		appdefault->setSpecular(spec);
		appdefault->setShininess(50.f);
	}
	void update(float t)
	{
		if(startTime==0)
		startTime=t;
		currentTime=t;
		tp=(currentTime-startTime)/1000;		//tempo passado desde o inicio
		//cout<<"tempo passado:"<<tp<<endl;

	}
	float getTime(){return tp;}

	void reset(){startTime=0;}

	void setSpan(float s){this->spanTime=s;}

	void setBool(bool b){this->count=b;}

	bool getBool(){return this->count;}

	bool overSpan(){
		if(tp>this->spanTime)
			return true;
		else
			return false;
	}

	void draw(){
		appdefault->apply();
		glPushMatrix();
	
		glScaled(1,1,0.3);
		appChrome->apply();
		cylinder.draw();
		glPopMatrix();

		////////////////////butao
		glPushMatrix();
		glRotated(-30,0,0,1);
		glTranslated(0,1.7,0.3);
		glRotated(-90,1,0,0);
		glScaled(1.3,1.3,0.5);
		appChrome->apply();
		topC.draw();
		glPopMatrix();

		glPushMatrix();
		glRotated(-30,0,0,1);
		glTranslated(0,1.4,0.3);
		glRotated(-90,1,0,0);
		appChrome->apply();
		topC.draw();
		glPopMatrix();

		////////////////////borda
		glPushMatrix();
		glTranslated(0,0,0.3);
		torus.draw();
		glPopMatrix();

		///////////////////relogio
		glPushMatrix();
			glRotated(-90,0,0,1);
			
			glTranslated(0,0,0.1);
			glScaled(1,1,0.9);
			appClock->apply();
			cylinder.draw();
		glPopMatrix();

		////////////////////ponteiro
		glPushMatrix();

			float angle=(tp*360)/spanTime;
			glRotated(-angle,0,0,1);
			glScaled(0.15,0.5,1);
			glTranslated(0,0.433012,0.6);
			glRotated(90,0,0,1);
			glRotated(90,1,0,0);
			appClockHand->apply();
			triangle.draw();
		glPopMatrix();
	}

};




#endif