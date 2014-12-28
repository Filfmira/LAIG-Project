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
	Animation();
	virtual void draw(){cout << "draw classe mae" << endl;};
	virtual void update(float t){};
	virtual void show(){};
	string getId();
	bool isEnd();
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
	LinearAnimation();
	LinearAnimation(string idS, float span, vector< vector<float>> pC);
	vector< vector<float>> getPontosDeControlo();
	float getStartTime();

	void show();
	void update(int t,float tp,float tc);
	void update(float t);
	float modificaAngulo();
	void draw();
	void restart();

};


class CircularAnimation:public Animation{

private:
	float startTime;
	float raio;
	float startAngle;
	float endAngle;
	float incAngle;
public:
	CircularAnimation();
	CircularAnimation(string idS,float span,float r,float sA,float eA);
	void update(float t);
	void show();
	void draw();
	void restart();
};


#endif