#ifndef LIGHTS_H
#define LIGHTS_H

#include <CGFlight.h>
#include <vector>


class Light{

private:
	string lightId;
	string tipo;
	float pos[4];
	float target[3];
	bool enabled;
	bool marker;
	float angle;
	float exponent;
	CGFlight* light;
	unsigned int CGFlightIndex;

public:

	Light(string tipo, string lightId, bool enabled, bool marker, float pos[4], 
		float target[3], float angle, float exponent,
		float* ambient, float* diffuse, float* specular, int index);

	void apply();
	~Light();

	string showLight();
	float getAngle(){return this->angle;}
	void setAngle(float angle){this->angle=angle;}
	string getLightId(){return this->lightId;}
	CGFlight* getLight(){return light;}
	bool getMarker(){return marker;}
	void setCGFlightIndex(int i){this->CGFlightIndex = i;}
	int getCGFlightIndex(){return this->CGFlightIndex;}
};



#endif