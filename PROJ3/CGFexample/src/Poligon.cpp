
#include "Poligon.h"

Poligon::Poligon(){this->sides = 3;}

Poligon::Poligon(int sides):sides(sides){
	if (sides > 2){
		float alphaStep = 2.0*acos(-1.0)/(float) sides;
		float alpha = 0;
		for (int i = 0; i < sides; i++, alpha+=alphaStep){
			float x = cos(alpha);
			float y = 0;
			float z = sin(alpha);
			Point3D temp(x,y,z);
			this->points.push_back(temp);

			float tx = (x+1.0)/2.0;
			float ty = (z+1.0)/2.0;
			Point2D tempText(tx, ty);
			this->texturePoints.push_back(tempText);
		}	
	}
}

void Poligon::draw(){
	glPushMatrix();
		glNormal3d(0.0, 1.0, 0.0);
		glBegin(GL_TRIANGLE_FAN);
			glTexCoord2d(0.5, 0.5);
			glVertex3d(0.0, 0.0, 0.0);
			for (int i = this->sides-1; i >= 0; i--){
				glTexCoord2d(texturePoints[i].x, texturePoints[i].y);
				glVertex3d(points[i].x, points[i].y, points[i].z);
			}
			glTexCoord2d(texturePoints[this->sides-1].x, texturePoints[this->sides-1].y);
			glVertex3d(points[this->sides-1].x, points[this->sides-1].y, points[this->sides-1].z);
		glEnd();
	glPopMatrix();
}

const std::vector<Point3D> Poligon::getPoints(){
	return this->points;
}