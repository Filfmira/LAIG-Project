#ifndef CAMERAS_H
#define CAMERAS_H

#include <vector>
#include "CGFcamera.h"
#include "CGFapplication.h"
#include <string>

class Camera{

protected:
	CGFcamera* c;
	string id;
	string type;
public:
	Camera(){};
	Camera(CGFcamera* c,string id, string tipo){
		this->id=(string)id;
		this->c=c;
		this->type = tipo;
	}
	string getId(){return id;}
	CGFcamera* getCamera(){return this->c;}

	string getType(){
		return type;
	}
};



class Perspective:public CGFcamera{
	
private:
	float near, far,angle;
	float pos[3];
	float target[3];
	vector< vector<float>> v;
	vector< vector<float>> v2;
	int k;
	bool player;
	bool ended;
	float zoom;

public:
	Perspective(float near, float far, float angle, float pos[3], float target[3]){
		this->near=near;
		this->angle=angle;
		this->pos[0]=pos[0];
		this->pos[1]=pos[1];
		this->pos[2]=pos[2];
		this->target[0]=target[0];
		this->target[1]=target[1];
		this->target[2]=target[2];
		float a=0;
		zoom=25;
		while(a<3.14159265){
			vector<float> f;
			f.push_back(cos(a+1.57079633));
			f.push_back(sin(a+1.57079633));
			v.push_back(f);
			a+=0.1;
		}
		vector<float> final;
			final.push_back(0);
			final.push_back(-zoom);
			v.push_back(final);
		a=3.14159265;
		while(a>=0){
			vector<float> f;
			f.push_back(cos(a+1.57079633));
			f.push_back(sin(a+1.57079633));
			v2.push_back(f);
			a-=0.1;
		}
		vector<float> final2;
			final2.push_back(0);
			final2.push_back(zoom);
			v2.push_back(final2);
		k=0;
		player=false;
		ended=true;
	}

	void updateProjectionMatrix (int width, int height){//reshape
		glMatrixMode (GL_PROJECTION ) ;
		glLoadIdentity();
		gluPerspective(angle,width/height,near,far);
	}
	bool hasEnded(){return ended;}

	void change(bool player){
		this->player=player;
			k=0;
			ended=false;

	}

	void setZoom(float z)
	{
		zoom=z;
	}

	float getZoom()
	{
		return zoom;
	}

	void applyView() {
		k++;
		if(player)
		{
			if(k>=v.size())
				{
				k=v.size();
				ended=true;
				gluLookAt(0,15,-zoom,0,0,0,0.0,1.0,0.0);
				}
			else
				gluLookAt(v[k][0]*zoom,15,v[k][1]*zoom,0,0,0,0.0,1.0,0.0);
		}
		else{
			if(k>=v2.size())
			{
				k=v2.size()-1;
				ended=true;
				gluLookAt(0,15,zoom,0,0,0,0.0,1.0,0.0);
			}
			else
			gluLookAt(v2[k][0]*zoom,15,v2[k][1]*zoom,0,0,0,0.0,1.0,0.0);


		}
		//gluLookAt(pos[0],pos[1],pos[2],target[0],target[1],target[2],0.0,1.0,0.0);
	};




};

class Ortho:public CGFcamera{
	private:
	float near, far,left,right,top,bottom;
	string direction;
	string type;
	float angle;
public:
	Ortho(string direction,float near, float far, float left, float right, float top, float bottom)
	{
		this->direction=direction;
		this->near=near;
		this->far=far;
		this->left=left;
		this->right=right;
		this->top=top;
		this->bottom=bottom;
		this->type = "ortho";
		angle=0;
	}

	void updateProjectionMatrix(int width, int height){
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(left, right, bottom, top, near, far);
	}



	void applyView(){	
		angle=angle+0.1;
		CGFcamera::applyView();
		glRotatef(angle,0,1,0);
		/*if(direction=="x")c
			glRotatef(-90,0,1,0);
		else if (direction=="y")
			glRotatef(90,1,0,0);*/
	}

};


#endif