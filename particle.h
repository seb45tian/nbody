#ifndef __PARTICLE_H
#define __PARTICLE_H

#include "vec3d.h"
#include "node.h"

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

	/* SETTER */
	void set(const vec3D & npos, const vec3D & nvel, const double nm) {pos = npos; vel = nvel; mass = nm;}
	void setVel(const vec3D &nvel) {vel = nvel;}

	/* GETTER */
	vec3D getPos() {return pos;}
	vec3D getVel() {return vel;}
	double getMass() {return mass;}

	// function that returns true if particle is in a given Node/Octant
	bool inNode(Node octant) {return octant.containsParticle(pos);}

	

	bool empty()
	{
		if (mass==0.0 && pos==vec3D(0) && vel==vec3D(0))
		{
			return true;
		}
		return false;
	}


    friend bool operator == (const particle &p1, const particle &p2)
    {
    	if (p1.pos == p2.pos)
    	{
    		return true;
    	}
    	return false;
    }


	// function to combine two particles -> calculate new center of mass and total mass
	particle combine(particle &a, particle &b)
	{
		double totalmass, nx, ny, nz;
		particle c;
		totalmass = b.mass + a.mass;
		nx  = (a.pos.x*a.mass + b.pos.x*b.mass)/totalmass;
		ny  = (a.pos.y*a.mass + b.pos.y*b.mass)/totalmass;
		nz  = (a.pos.z*a.mass + b.pos.z*b.mass)/totalmass;

		c.mass = totalmass;
		c.pos.x = nx;
		c.pos.y = ny;
		c.pos.z = nz;

		return c;
	}


	void addVelocity(particle &a)
	{
		const double G = 6.6470831e-11;
		vec3D acc(0,0,0);
		vec3D diff  = a.pos - pos;
		vec3D nom   = diff*a.mass;
		double dist = diff.magnitude();
		double denom =  (dist*dist*dist);
		acc = (nom/denom)*G;
		vel = vel + acc;
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