#ifndef _POLIGON_H_	
#define _POLIGON_H_
#include "CGFobject.h"
#include "Point.h"
#include <vector>
class Poligon{
	
private:
	std::vector<Point3D> points;
	std::vector<Point2D> texturePoints;
	int sides;
public:
	Poligon();
	Poligon(int sides);
	void draw();
};


#endif

			
			
			
			
			
			
