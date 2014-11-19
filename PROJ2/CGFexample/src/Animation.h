#ifndef ANIMATION_H
#define ANIMATION_H


#include <iostream>
#include "CGFApplication.h"

class Animation{
protected:
	string id;
	float time;
	bool ended;
public:
	Animation(){};
	virtual void draw(){};
	virtual void update(float t){};
	virtual void show(){};
	string getId(){return id;};
	bool isEnd(){return ended;};
	virtual void restart(){};

};



class LinearAnimation:public Animation{
	float startTime;
	float x,y,z;
	float x0,y0,z0;
	float x1,y1,z1;
	float rad2degree;
	float xalpha,zalpha,alpha;
	vector< vector<float>> pontosDeControlo;
	int indice;

public:
	LinearAnimation(){};
	LinearAnimation(string idS, float span, vector< vector<float>> pC){
		rad2degree=180/acos(-1.0);
		x=y=z=0;
		startTime=0;
		id=idS;
		time=span;
		pontosDeControlo=pC;
		xalpha=zalpha=0;
		ended=false;
	};

	vector< vector<float>> getPontosDeControlo(){return pontosDeControlo;}
	float getStartTime(){return startTime;}


	void show(){
		cout<<"id:"<<id<<"TIME:"<<time<<"startTime:"<<startTime<<endl;
		for(int i =0;i<pontosDeControlo.size();i++)
			cout<<"x:"<<pontosDeControlo.at(i)[0]<<" y:"<<pontosDeControlo.at(i)[1]<<" z:"<<pontosDeControlo.at(i)[2]<<endl;

	}

	void update(int t,float tp,float tc){
		int x0=pontosDeControlo.at(indice).at(0);
		int y0=pontosDeControlo.at(indice).at(1);
		int z0=pontosDeControlo.at(indice).at(2);

		int x1=pontosDeControlo.at(indice+1).at(0);
		int y1=pontosDeControlo.at(indice+1).at(1);
		int z1=pontosDeControlo.at(indice+1).at(2);

		float a=x1-x0;
		float b=y1-y0;
		float c=z1-z0;

		float incX=((((tp-indice*tc)*a)/tc));
		float incY=((((tp-indice*tc)*b)/tc));
		float incZ=((((tp-indice*tc)*c)/tc));


		x=x0+(incX);
		y=y0+(incY);
		z=z0+(incZ);

		xalpha=x-pontosDeControlo.at(indice).at(0);
		zalpha=z-pontosDeControlo.at(indice).at(2);
		alpha=modificaAngulo();
		//cout<</*"tp:"<<tp<<" tc:"<<tc<<*/" indice:"<<indice<<"  ia:"<<incX<<" ib:"<<incY<<" ic:"<<incZ<<"  x:"<<x<<" y:"<<y<<" z:"<<z<<endl;

	};

	void update(float t)
	{
		if(startTime==0)
			startTime=t;
		
		float tp=(t-startTime)/1000;		//tempo passado desde o inicio
		int length=pontosDeControlo.size();
		float tc=time/length;
		indice=int(tp/tc);
		if(indice<length-1)
		update(t,tp,tc);
		//se ja tiver acabado a animacao, nao faz nada
		else
			ended=true;
		
	};

	float modificaAngulo()
	{
		float alpha1;
		if(xalpha==0)
		{
			if(zalpha>0)
				alpha1=-90;
			else
				alpha1=90;
		}
		else if(zalpha==0)
		{
			if(xalpha>0)
				alpha1=0;
			else
				alpha1=180;
		}
		else
		{
			alpha1=90-(atan(abs(xalpha)/abs(zalpha))*rad2degree);
			if(zalpha<0)
			{
				if(xalpha<0)
				{
					alpha1=180-alpha1;
				}
				else 
				{

				}
			}
			else
			{
				if(xalpha>0)
				alpha1=-alpha1;
				else
				alpha1=180+alpha1;
			}

		}
		return alpha1;

	}

	void draw(){
		glPushMatrix();
		glTranslated(x, y, z);
		
		//cout<<xalpha<<"||"<<zalpha<<" alpha1:"<<alpha1<<endl;

		glRotatef(alpha,0,1,0);
		glRotated(90,0,1,0);
		//glRotatef(alpha2,0,0,1);
		
			/*	glRotated(-90,0,0,1);
				glRotated(-90,0,1,0);
				glBegin(GL_TRIANGLES);
					glVertex2d( 1.0, 0);
					glVertex2d( 0.0, 3.0);
					glVertex2d(-1.0, 0);
				glEnd();


		glPopMatrix();*/
	}


	void restart()
	{
		x=y=z=0;
		startTime=0;
		xalpha=zalpha=0;
		ended=false;
	}

};


class CircularAnimation:public Animation{

private:
	float startTime;
	float raio;
	float startAngle;
	float endAngle;
	float incAngle;
public:
	CircularAnimation(){};
	CircularAnimation(string idS,float span,float r,float sA,float eA)
	{
		time=span;
		raio=r;
		startAngle=sA;
		endAngle=eA+startAngle;
		startTime=0;
		id=idS;
		ended=false;
	}

	void update(float t){
		if(startTime==0)
			startTime=t;
		float tp=(t-startTime)/1000;
		if(tp<time)
		 incAngle=(tp*(endAngle-startAngle))/time;
		else
			ended=true;
		//cout<<"  tp:"<<tp<<endl;
	}

	void show(){
		cout<<incAngle<<endl;
	}

	void draw(){
		glPushMatrix();
		glRotated(incAngle,0,1,0);
		glRotated(startAngle,0,1,0);
	
		glTranslated(raio,0,0);
		glRotated(180,0,1,0);
		/*		glRotated(-90,0,0,1);
				glRotated(-90,0,1,0);
				glBegin(GL_TRIANGLES);
					glVertex2d( 1.0, 0);
					glVertex2d( 0.0, 3.0);
					glVertex2d(-1.0, 0);
				glEnd();


		glPopMatrix();*/
	};



	void restart()
	{
		startTime=0;
		incAngle=0;
	}
};


#endif