#include "CGFapplication.h"
#include "Primitives.h"


class Patch: public Primitive{
private:
	float u0;
	float u1;
	float v0;
	float v1;
	int order;
	int partsU;
	int partsV;
	int u_stride;
	int v_stride;
	vector< vector<float>> controlPoints;

public:
	Patch(){
		u_stride = 3;
		v_stride = 9;
		order=2;
		u0 = 0.0;
		v0 = 0.0;
		u1 = 1.0;
		v1 = 1.0;
		partsU=10;
		partsV=10;
		vector<float> point;
		point.push_back(-0.5);
		point.push_back(0);
		point.push_back(0.5);
		vector<float> point1;
		point1.push_back(-0.5);
		point1.push_back(0);
		point1.push_back(-0.5);
		vector<float> point2;
		point2.push_back(0.5);
		point2.push_back(0);
		point2.push_back(0.5);
		vector<float> point3;
		point3.push_back(0.5);
		point3.push_back(1);
		point3.push_back(0.5);

		controlPoints.push_back(point);
		controlPoints.push_back(point1);
		controlPoints.push_back(point2);
		controlPoints.push_back(point3);
		controlPoints.push_back(point);
		controlPoints.push_back(point1);
		controlPoints.push_back(point2);
		controlPoints.push_back(point3);
		controlPoints.push_back(point);
		cout<<"passou"<<endl;
	};

	void draw()
	{

		glEnable(GL_CW);
		glEnable(GL_AUTO_NORMAL);
		/*GLfloat ctrl_pts[9][3];
		for (int i =0;i<controlPoints.size();i++){
			ctrl_pts[i][0] = controlPoints.at(i).at(0);
			ctrl_pts[i][1] = controlPoints.at(i).at(1);
			ctrl_pts[i][2] = controlPoints.at(i).at(2);
		}
		*/
		GLfloat exampleControlPoints[48] = {
			-1.5,-1.5,4.0,
			-0.5,-1.5,2.0,
			0.5,-1.5,-1.0,
			1.5,-1.5,2.0,

			-1.5,-0.5,1.0,
			-0.5,-0.5,3.0,
			0.5,-0.5,0.0,
			1.5,-0.5,-1.0,

			-1.5,0.5,4.0,
			-0.5,0.5,0.0,
			0.5,0.5,3.0,
			1.5,0.5,4.0,

			-1.5,1.5,-2.0,
			-0.5,1.5,-2.0,
			0.5,1.5,0.0,
			1.5,1.5,-1.0

       };

		/*u_stride = 2;
		v_stride = 9;*/
		order=3;

		glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, order+1,
			0, 1, 3*(order+1), order+1, &exampleControlPoints[0]);

		glEnable(GL_MAP2_VERTEX_3);
		glEnable(GL_AUTO_NORMAL);
		glEnable(GL_MAP2_TEXTURE_COORD_2);
		glMapGrid2f(partsU, 0.0, 1.0, partsV, 0.0, 1.0);

		glPushMatrix();
	glFrontFace(GL_CW);

	//if(compute.compare("fill") == 0)
		glEvalMesh2(GL_FILL, 0, partsU, 0, partsV);
	/*else if(compute.compare("line") == 0)
		glEvalMesh2(GL_LINE, 0, partsU, 0, partsV);
	else glEvalMesh2(GL_POINT, 0, partsU, 0, partsV);*/
	
	glFrontFace(GL_CCW);
	glPopMatrix();
		/*
		glMap2f(GL_MAP2_VERTEX_3, u0, u1, u_stride, u_order, v0, v1, v_stride, v_order, &exampleControlPoints[0]);
		glMap2f(GL_MAP2_TEXTURE_COORD_2, u0, u1, 2, 3, v0, v1, 6, 3, &exampleControlPoints[0]);




		glEnable(GL_MAP2_VERTEX_3);
		glEnable(GL_MAP2_NORMAL);
		glEnable(GL_MAP2_TEXTURE_COORD_2);
		glMapGrid2f(partsU, u0, u1, partsV, v0, v1);
		glEvalMesh2(GL_FILL, 0, partsU, 0, partsV);
		glEnable(GL_CCW);
		*/
	};



};