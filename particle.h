#ifndef __PARTICLE_H
#define __PARTICLE_H

#include "vec3d.h"

class simulation; 

class particle 
{
	vec3D pos; 
	vec3D vel; 
	double mass; 
public:
	particle() {}

	// this is the proper update 
	void update() 
	{
		pos += vel; 
	}

	// check for boundaries
	// IMPORTANT: this should not be used in the simulation
	// it is only for testing the visualisation 
	// adds periodic boundary conditions
	void update(const vec3D & max)
	{
		update();

		if (pos.x > max.x) pos.x = -max.x;
		if (pos.x < -max.x) pos.x = max.x;

		if (pos.y > max.y) pos.y = -max.y;
		if (pos.y < -max.y) pos.y = max.y;

		if (pos.z > max.z) pos.z = -max.z;
		if (pos.z < -max.z) pos.z = max.z;
	}

	void set(const vec3D & npos, const vec3D & nvel, const double nm) {pos = npos; vel = nvel; mass = nm;}

	void setVel(const vec3D &nvel) {vel = nvel;}

	// GETTER FUNCTIONS
	vec3D getPos()
	{
		return pos;
	}

	vec3D getVel()
	{
		return vel;
	}

	double getMass()
	{
		return mass;
	}

	friend istream& operator>>(istream& iStr, particle & p)
	{
		iStr >> p.mass;
		iStr >> p.pos; 
		iStr >> p.vel; 
		return iStr;
	}

	friend ostream& operator<<(ostream& oStr, const particle & p)
	{
		oStr << p.mass << endl;
		oStr << p.pos; 
		oStr << p.vel << endl; 
		return oStr;
	}

	friend class simulation; 
};



#endif 