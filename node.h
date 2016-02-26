#ifndef __NODE_H
#define __NODE_H


#include <iostream>
#include "vec3d.h"


class Node
{
private:
	// Center of Mass
	vec3D com;
	// total mass
	double total_mass;
	// pointers to 8 child nodes
	const Node *octant_one;
	const Node *octant_two;
	const Node *octant_three;
	const Node *octant_four;
	const Node *octant_five;
	const Node *octant_six;
	const Node *octant_seven;
	const Node *octant_eight;

	void set(vec3D ncom = vec3D(0), double tm = 0)
	{
		com = ncom;
		total_mass = tm;
		octant_one 	 = NULL;
		octant_two 	 = NULL;
		octant_three = NULL;
		octant_four  = NULL;
		octant_five  = NULL;
		octant_six 	 = NULL;
		octant_seven = NULL;
		octant_eight = NULL;
	}

public:
	/* CONSTRUCTORS */
	Node() {set();}
	Node(const vec3D ncom, const double tm) {set(ncom, tm);}
	
	// constructor for new internal node with pointers to 8 child nodes
	Node(const Node *a, const Node *b, const Node *c, const Node *d, 
		 const Node *e, const Node *f, const Node *g, const Node *h)
	{
		total_mass   = a->total_mass+b->total_mass+c->total_mass+d->total_mass
					  +e->total_mass+f->total_mass+g->total_mass+h->total_mass;

		double comx = ( a->com.x*a->total_mass + b->com.x*b->total_mass + c->com.x*c->total_mass
					  + d->com.x*d->total_mass + e->com.x*e->total_mass + f->com.x*f->total_mass
					  + g->com.x*g->total_mass + h->com.x*h->total_mass ) / total_mass;

		double comy = ( a->com.y*a->total_mass + b->com.y*b->total_mass + c->com.y*c->total_mass
					  + d->com.y*d->total_mass + e->com.y*e->total_mass + f->com.y*f->total_mass
					  + g->com.y*g->total_mass + h->com.y*h->total_mass ) / total_mass;

		double comz = ( a->com.z*a->total_mass + b->com.z*b->total_mass + c->com.z*c->total_mass
					  + d->com.z*d->total_mass + e->com.z*e->total_mass + f->com.z*f->total_mass
					  + g->com.z*g->total_mass + h->com.z*h->total_mass ) / total_mass;

		com = vec3D(comx, comy, comz);


		octant_one 	 = a;
		octant_two	 = b;
		octant_three = c;
		octant_four  = d;
		octant_five  = e;
		octant_six 	 = f;
		octant_seven = g;
		octant_eight = h;
	}


	/* GETTER */
	double get_totalMass() const {return total_mass;}
	vec3D get_com() const {return com;}


	void traverse() const
	{
		if (octant_one)
		{
			cout << "Has octant one" << endl;
			octant_one->traverse();
		}
		if (octant_two)
		{
			cout << "Has octant two" << endl;
			octant_two->traverse();
		}
		if (octant_three)
		{
			cout << "Has octant three" << endl;
			octant_three->traverse();
		}
		if (octant_four)
		{
			cout << "Has octant four" << endl;
			octant_four->traverse();
		}
		if (octant_five)
		{
			cout << "Has octant five" << endl;
			octant_five->traverse();
		}
		if (octant_six)
		{
			cout << "Has octant six" << endl;
			octant_six->traverse();
		}
		if (octant_seven)
		{
			cout << "Has octant seven" << endl;
			octant_seven->traverse();
		}
		if (octant_eight)
		{
			cout << "Has octant eight" << endl;
			octant_eight->traverse();
		}
		else
		{
			cout << "no child nodes" << endl;
		}
	}
	


};

#endif