#ifndef LIGHTS_H
#define LIGHTS_H

#include <CGFlight.h>
#include <vector>


class Light{

private:
	int lightId;
	string tipo;
	float pos[4];
	float target[3];
	bool enabled;
	bool marker;
	float angle;
	float exponent;
	CGFlight* light;

public:

	Light(string tipo, int lightId, bool enabled, bool marker, float pos[4], 
		float target[3], float angle, float exponent,
		float* ambient, float* diffuse, float* specular);

	void apply();
	~Light();

	string showLight();
	float getAngle(){return this->angle;}
	void setAngle(float angle){this->angle=angle;}
	int getLightId(){return lightId;}
	CGFlight* getLight(){return light;}
};



#endif