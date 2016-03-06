#ifndef __NODE_H
#define __NODE_H


#include <iostream>
#include "vec3d.h"
#include "particle.h"

using namespace std;

class Node
{
private:
	// Center of Mass
	vec3D com;
	// Midpoint of this octant
	vec3D mid;
	// total mass
	double total_mass;
	// side length of this octant
	double length;

	// array for indices of particle array Ps
	int *index;

	// pointers to 8 child nodes
	Node *octant_one;
	Node *octant_two;
	Node *octant_three;
	Node *octant_four;
	Node *octant_five;
	Node *octant_six;
	Node *octant_seven;
	Node *octant_eight;


	void set(vec3D nmid = vec3D(0), double len = 0)
	{
		mid = nmid;
		length = len;

		com = vec3D(0);
		total_mass = 0;
		index = NULL;
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
	Node(const vec3D nmid, const double len) {set(nmid, len);}



	// check if node is a leaf node
	bool is_leaf()
	{
		if (octant_one==NULL && octant_two==NULL && octant_three==NULL && octant_four==NULL && octant_five==NULL
			&& octant_six==NULL && octant_seven==NULL && octant_eight==NULL)
		{
			return true;
		}
		return false;
	}
	
	// calculate total mass from child nodes
	void calc_TotalMass()
	{
		if (!this->is_leaf())
		{
			total_mass   = octant_one->total_mass+octant_two->total_mass+octant_three->total_mass+octant_four->total_mass
						  +octant_five->total_mass+octant_six->total_mass+octant_seven->total_mass+octant_eight->total_mass;
		}
		else {cerr << "Can't calculate total mass. This node is not a leaf node!" << endl;}
	}

	void calc_com()
	{
		if (!this->is_leaf())
		{
			double comx = ( octant_one->com.x*octant_one->total_mass + octant_two->com.x*octant_two->total_mass + octant_three->com.x*octant_three->total_mass
						  + octant_four->com.x*octant_four->total_mass + octant_five->com.x*octant_five->total_mass + octant_six->com.x*octant_six->total_mass
						  + octant_seven->com.x*octant_seven->total_mass + octant_eight->com.x*octant_eight->total_mass ) / total_mass;

			double comy = ( octant_one->com.y*octant_one->total_mass + octant_two->com.y*octant_two->total_mass + octant_three->com.y*octant_three->total_mass
						  + octant_four->com.y*octant_four->total_mass + octant_five->com.y*octant_five->total_mass + octant_six->com.y*octant_six->total_mass
						  + octant_seven->com.y*octant_seven->total_mass + octant_eight->com.y*octant_eight->total_mass ) / total_mass;

			double comz = ( octant_one->com.z*octant_one->total_mass + octant_two->com.z*octant_two->total_mass + octant_three->com.z*octant_three->total_mass
						  + octant_four->com.z*octant_four->total_mass + octant_five->com.z*octant_five->total_mass + octant_six->com.z*octant_six->total_mass
						  + octant_seven->com.z*octant_seven->total_mass + octant_eight->com.z*octant_eight->total_mass ) / total_mass;

			com = vec3D(comx, comy, comz);
		}
	}



	//	   2 -------- 4
	//	  /|         /|
	//   / |        / |
	//	1 -------- 3  |
	//  |  |       |  |   
	//  |  6 ------|- 8 
    //  | /        | /
    //  |/         |/
    //  5 -------- 7
	//	


	Node* createOctant(int octant)
	{
		Node *newnode = NULL;
		double newlength = 0.5*length;
		vec3D newmid = vec3D(0); 
		switch (octant)
		{
			case 1:
			{
				newmid.x = mid.x - 0.25*length;
				newmid.y = mid.y - 0.25*length;
				newmid.z = mid.z + 0.25*length;
				octant_one = new Node(newmid, newlength);
				newnode = octant_one;
				break;
			}
			case 2:
			{
				newmid.x = mid.x + 0.25*length;
				newmid.y = mid.y - 0.25*length;
				newmid.z = mid.z + 0.25*length;
				octant_two = new Node(newmid, newlength);
				newnode = octant_two;
				break;
			}
			case 3:
			{
				newmid.x = mid.x - 0.25*length;
				newmid.y = mid.y + 0.25*length;
				newmid.z = mid.z + 0.25*length;
				octant_three = new Node(newmid, newlength);
				newnode = octant_three;
				break;
			}
			case 4:
			{
				newmid.x = mid.x + 0.25*length;
				newmid.y = mid.y + 0.25*length;
				newmid.z = mid.z + 0.25*length;
				octant_four = new Node(newmid, newlength);
				newnode = octant_four;
				break;
			}
			case 5:
			{
				newmid.x = mid.x - 0.25*length;
				newmid.y = mid.y - 0.25*length;
				newmid.z = mid.z - 0.25*length;
				octant_five = new Node(newmid, newlength);
				newnode = octant_five;
				break;
			}
			case 6:
			{
				newmid.x = mid.x + 0.25*length;
				newmid.y = mid.y - 0.25*length;
				newmid.z = mid.z - 0.25*length;
				octant_six = new Node(newmid, newlength);
				newnode = octant_six;
				break;
			}
			case 7:
			{
				newmid.x = mid.x - 0.25*length;
				newmid.y = mid.y + 0.25*length;
				newmid.z = mid.z - 0.25*length;
				octant_seven = new Node(newmid, newlength);
				newnode = octant_seven;
				break;
			}
			case 8:
			{
				newmid.x = mid.x + 0.25*length;
				newmid.y = mid.y + 0.25*length;
				newmid.z = mid.z - 0.25*length;
				octant_eight = new Node(newmid, newlength);
				newnode = octant_eight;
				break;
			}
			default:
			cerr << "Error: " << octant << " is no valid octant number!" << endl;
			break;
		}
		return newnode;
	}






	Node *SWU()
	{
		double newlength = 0.5*length;
		vec3D newmid = vec3D(0); 
		newmid.x = mid.x - 0.25*length;
		newmid.y = mid.y - 0.25*length;
		newmid.z = mid.z + 0.25*length;
		Node *newnode = new Node(newmid, newlength);
		return newnode;
	}

	Node *NWU()
	{
		double newlength = 0.5*length;
		vec3D newmid = vec3D(0); 
		newmid.x = mid.x + 0.25*length;
		newmid.y = mid.y - 0.25*length;
		newmid.z = mid.z + 0.25*length;
		Node *newnode = new Node(newmid, newlength);
		return newnode;
	}

	Node *SEU()
	{
		double newlength = 0.5*length;
		vec3D newmid = vec3D(0); 
		newmid.x = mid.x - 0.25*length;
		newmid.y = mid.y + 0.25*length;
		newmid.z = mid.z + 0.25*length;
		Node *newnode = new Node(newmid, newlength);
		return newnode;
	}

	Node *NEU()
	{
		double newlength = 0.5*length;
		vec3D newmid = vec3D(0); 
		newmid.x = mid.x + 0.25*length;
		newmid.y = mid.y + 0.25*length;
		newmid.z = mid.z + 0.25*length;
		Node *newnode = new Node(newmid, newlength);
		return newnode;
	}

	Node *SWD()
	{
		double newlength = 0.5*length;
		vec3D newmid = vec3D(0); 
		newmid.x = mid.x - 0.25*length;
		newmid.y = mid.y - 0.25*length;
		newmid.z = mid.z - 0.25*length;
		Node *newnode = new Node(newmid, newlength);
		return newnode;
	}

	Node *NWD()
	{
		double newlength = 0.5*length;
		vec3D newmid = vec3D(0); 
		newmid.x = mid.x + 0.25*length;
		newmid.y = mid.y - 0.25*length;
		newmid.z = mid.z - 0.25*length;
		Node *newnode = new Node(newmid, newlength);
		return newnode;
	}

	Node *SED()
	{
		double newlength = 0.5*length;
		vec3D newmid = vec3D(0); 
		newmid.x = mid.x - 0.25*length;
		newmid.y = mid.y + 0.25*length;
		newmid.z = mid.z - 0.25*length;
		Node *newnode = new Node(newmid, newlength);
		return newnode;
	}

	Node *NED()
	{
		double newlength = 0.5*length;
		vec3D newmid = vec3D(0); 
		newmid.x = mid.x + 0.25*length;
		newmid.y = mid.y + 0.25*length;
		newmid.z = mid.z - 0.25*length;
		Node *newnode = new Node(newmid, newlength);
		return newnode;
	}





	/* GETTER */
	double get_totalMass() const {return total_mass;}
	double get_length() const {return length;}
	vec3D get_com() const {return com;}
	vec3D get_mid() const {return mid;}

	/* SETTER */
	void set_com(vec3D ncom) {com = ncom;}
	void set_totalMass(double m) {total_mass = m;}



	// functions to check if a particle belongs to this node
	bool containsParticle(vec3D p)
	{
		vec3D diff = mid - p;
		if (diff.inftynorm() <= length*0.5) {return true;}
		return false;
	}











	// test traverse functions
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