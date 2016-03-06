#ifndef __NODE_H
#define __NODE_H


#include <iostream>
#include "vec3d.h"

using namespace std;

class particle;

class Node
{
private:
	// Midpoint of this octant
	vec3D mid;
	// side length of this octant
	double length;
	// private method to set parameters
	void set(vec3D nmid = vec3D(0), double len = 0) {mid = nmid; length = len;}

public:
	/* CONSTRUCTORS */
	Node() {set();}
	Node(const vec3D nmid, const double len) {set(nmid, len);}



	//	  NWU--------NEU
	//	  /|         /|
	//   / |        / |
	// SWU--------SEU |
	//  |  |       |  |   
	//  | NWD------|-NED 
    //  | /        | /
    //  |/         |/
    // SWD--------SED
	//	



	Node SWU()
	{
		double newlength = 0.5*length;
		vec3D newmid = vec3D(0); 
		newmid.x = mid.x - 0.25*length;
		newmid.y = mid.y - 0.25*length;
		newmid.z = mid.z + 0.25*length;
		Node newnode = Node(newmid, newlength);
		return newnode;
	}

	Node NWU()
	{
		double newlength = 0.5*length;
		vec3D newmid = vec3D(0); 
		newmid.x = mid.x + 0.25*length;
		newmid.y = mid.y - 0.25*length;
		newmid.z = mid.z + 0.25*length;
		Node newnode = Node(newmid, newlength);
		return newnode;
	}

	Node SEU()
	{
		double newlength = 0.5*length;
		vec3D newmid = vec3D(0); 
		newmid.x = mid.x - 0.25*length;
		newmid.y = mid.y + 0.25*length;
		newmid.z = mid.z + 0.25*length;
		Node newnode = Node(newmid, newlength);
		return newnode;
	}

	Node NEU()
	{
		double newlength = 0.5*length;
		vec3D newmid = vec3D(0); 
		newmid.x = mid.x + 0.25*length;
		newmid.y = mid.y + 0.25*length;
		newmid.z = mid.z + 0.25*length;
		Node newnode = Node(newmid, newlength);
		return newnode;
	}

	Node SWD()
	{
		double newlength = 0.5*length;
		vec3D newmid = vec3D(0); 
		newmid.x = mid.x - 0.25*length;
		newmid.y = mid.y - 0.25*length;
		newmid.z = mid.z - 0.25*length;
		Node newnode = Node(newmid, newlength);
		return newnode;
	}

	Node NWD()
	{
		double newlength = 0.5*length;
		vec3D newmid = vec3D(0); 
		newmid.x = mid.x + 0.25*length;
		newmid.y = mid.y - 0.25*length;
		newmid.z = mid.z - 0.25*length;
		Node newnode = Node(newmid, newlength);
		return newnode;
	}

	Node SED()
	{
		double newlength = 0.5*length;
		vec3D newmid = vec3D(0); 
		newmid.x = mid.x - 0.25*length;
		newmid.y = mid.y + 0.25*length;
		newmid.z = mid.z - 0.25*length;
		Node newnode = Node(newmid, newlength);
		return newnode;
	}

	Node NED()
	{
		double newlength = 0.5*length;
		vec3D newmid = vec3D(0); 
		newmid.x = mid.x + 0.25*length;
		newmid.y = mid.y + 0.25*length;
		newmid.z = mid.z - 0.25*length;
		Node newnode = Node(newmid, newlength);
		return newnode;
	}



	/* GETTER */
	double get_length() const {return length;}
	vec3D get_mid() const {return mid;}

	/* SETTER */
	void set_mid(vec3D nmid) {mid = nmid;}
	void set_length(double l) {length = l;}


	// functions to check if a particle belongs to this node
	bool containsParticle(vec3D pos)
	{
		vec3D diff = mid - pos;
		if (diff.inftynorm() <= length*0.5) {return true;}
		return false;
	}
	
};

#endif