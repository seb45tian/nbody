#ifndef __SIMULATION_H
#define __SIMULATION_H

#include "particle.h"
#include "node.h"
#include "bhtree.h"
#include <fstream> 
#include <cmath>

#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

class simulation 
{
private:
	vec3D boundary;
	particle *Ps;
	int size;
	bool pbc, show_octants, barneshut;
	double theta, eps;

	void set(const int n, const bool npbc, const bool oct, const bool bh, const double t, const double e)
	{
		size = n;
		boundary = vec3D(100,100,100);
		updatesCounter = 0;
		pbc = npbc;
		show_octants = oct;
		barneshut = bh;
		theta = t;
		eps = e;
	}

public:
	int updatesCounter;
	/* CONSTRUCTORS */
	simulation() {}
	simulation (const int n, const bool npbc, const bool oct, const bool bh, const double t, const double e)
	{
		set(n, npbc, oct, bh, t, e);
		Ps = new particle[n];
		randParticles();	
	}

	// constructor to load particle mass, coords and velocities from a FILE!
	simulation (const std::string filename, const int n, const bool npbc, const bool oct, const bool bh, 
				const double t, const double e)
	{
		set(n, npbc, oct, bh, t, e);

		// n-body data from http://bima.astro.umd.edu/nemo/archive/#dubinski
		ifstream fin;
		int count = 0;
		// fixed for now - will have to correct later
		double m,x,y,z,vx,vy,vz;

		fin.open(filename, ios::in); 
		// First read in file and count lines for correct array size
		if (fin.good())
		{
			while(fin >> m >> x >> y >> z >> vx >> vy >> vz )
			{
				count++;
			}
		}
		else { cout << "Error opening file: " << filename <<  endl; }
		fin.close();

		// set size to counted lines/particles
		size = count;
		// create particle array
		Ps = new particle[size];
		cout << "... found " << size << " particles.\n";
		// reset count
		count = 0;

		fin.open(filename, ios::in);
		if (fin.good())
		{
			// read in file a 2nd time and assign particles to array
			while (fin >> m >> x >> y >> z >> vx >> vy >> vz )
			{
				particle newp;
				newp.set(vec3D(x,y,z),vec3D(vx,vy,vz),m);
				Ps[count] = newp;
				count++;
			}
		}
		else { cout << "Error opening file: " << filename <<  endl; }
		fin.close(); 
	}

	/*========================================================================*/
	/* Function to create random particles */
	void randParticles()
	{
		for (int i = 0; i < size; i++)
		{
			double x, y, z;
			// the following is not nice but I was feeling lazy
			// initially the particles are towards the middle and spread out slowly
			x = (double)((rand() % ((int)boundary.x)) - (int)boundary.x/2);
			y = (double)((rand() % ((int)boundary.y)) - (int)boundary.y/2);
			z = (double)((rand() % ((int)boundary.z)) - (int)boundary.z/2);

			double vx, vy, vz;
			vx = (double)((rand() % 2000) - 1000);
			vy = (double)((rand() % 2000) - 1000);
			vz = (double)((rand() % 2000) - 1000);

			vx /= 10000.0f;
			vy /= 10000.0f;
			vz /= 10000.0f;

			Ps[i].set(vec3D(x,y,z), vec3D(vx,vy,vz),100000000.0f); 
		}
	}
	/*========================================================================*/

	/* draw should be called in between glBegin and glEnd */
	void draw()
	{
		glBegin(GL_POINTS);
		// first one is red - just to be able to follow one
		glVertex3dv(Ps[0].pos.v);
		// rest are green
		glColor3f(0.0f, 1.0f, 0.0f);
		for (int i = 1; i < size; i++)
		{
			// draw all the particles 
			glVertex3dv(Ps[i].pos.v); 		
		}
		glEnd();

		glColor3f(1.0f, 1.0f, 1.0f); 
		glutWireCube(boundary.x*2);

		// Increment the updates counter
		updatesCounter++;
	}


	/*========================================================================*/

	void update()
	{
		update(0,size);
	}
	/*========================================================================*/


	void update(const unsigned int start, const unsigned int end)
	{
		for (unsigned int i = start; i < end; i++)
		{
			if(pbc) { Ps[i].update(boundary); }
			else { Ps[i].update(); }
		}

		if (barneshut) { BarnesHut2(start, end); }
		else { calculateForce(start, end); }
	}

	/*========================================================================*/

	// this code updates the particles using multi threading 
	void MTupdate(const int proc_num)
	{
		// We create a tree here and fill it with all the particles (1 proc)
		BHTree tree;
		tree = fillTreeBH(0, size);

		// NOTE: this is not the most efficient way of handling threads  
		// the most efficient way of doing this is to 
		// create the threads once and use barriers
		boost::thread * t[proc_num];
		
		// NOTE: this form of load balancing may not be ideal
		// when the work load is not uniform 
		// for the simplest of cases it is 
		unsigned int per_thread = ceilf((float) size/ (float)(proc_num)); 

		// Loop over all processes and give everyone its own
		// start and end particle id
		for (int i = 0; i < proc_num; i++)
		{
			unsigned int start = i * per_thread;
			unsigned int end = start + per_thread;
			end = ((int)end < size) ? end : size;
			// cout <<  i  << " Start = " << start << " End = " << end << endl;
			// launch all threads
			// t[i] = new boost::thread(boost::bind(&simulation::update, this, start, end)); //ORIGINAL
			t[i] = new boost::thread(boost::bind(&simulation::updateVelBH, this, start, end, &tree)); //ADDED
		}
		
		// barrier wait on all threads
		for (int i = 0; i < proc_num; i++)
			t[i]->join();

		// remove thread
		// as mentioned above, we would ideally keep using the same threads
		for (int i = 0; i < proc_num; i++)
			delete t[i]; 

		// Update the positions
		for (unsigned int i = 0; i < size; i++)
		{
			if(pbc) { Ps[i].update(boundary); }
			else { Ps[i].update(); }
		}
	}

	/*========================================================================*/
	/* BRUTE FORCE METHOD TO CALCULATE THE NEW VELOCITIES (FORCES) */	
	void calculateForce(const unsigned int start, const unsigned int end)
	{
		for (unsigned int i = start; i < end; i++)
		{
			vec3D sum(0,0,0);
			for (unsigned int j = start; j < end; ++j)
			{
				if(j != i)
				{
					vec3D diff  = (Ps[j].getPos()-Ps[i].getPos());
					double mag   = diff.magnitude();
					double var1;
					if (eps > 0.0)
					{
						var1 = (Ps[j].getMass())/pow((mag*mag + eps*eps), 1.5);
					}
					else
					{
						var1 = (Ps[j].getMass())/(mag*mag*mag);
					}
					sum += diff * var1;
				}
			}
			Ps[i].setVel(Ps[i].getVel() + sum*G);
		}
	}



	/*========================================================================*/
	/* Function to fill the BH Tree with particles */
	BHTree fillTreeBH(const unsigned int start, const unsigned int end)
	{
		// Note: this work only for a cubic box at the moment where every side length is the same
		double newlength = boundary.x*2;
		// Create a root node
		Node root = Node(vec3D(0),newlength);
		// Create the Barnes-Hut tree
		BHTree tree = BHTree(root);

		// Add all bodies to the tree
		for (unsigned int i = start; i < end; i++)
		{
			if (Ps[i].inNode(root)) { tree.addParticle(Ps[i]); }
		}
		return tree;
	}

	/*========================================================================*/
	/* Function to update the velocities for a given Barnes-Hut tree */
	void updateVelBH(const unsigned int start, const unsigned int end, BHTree *tree)
	{
		for (unsigned int i = start; i < end; i++)
		{
			//Update the velocities traveling recursively through the tree
			tree->updateVelocity(Ps[i], theta, eps);
		}
	}

	/*========================================================================*/
	/* BARNES-HUT METHOD TO CALCULATE THE NEW VELOCITES (FORCES) */
	void BarnesHut2(const unsigned int start, const unsigned int end)
	{
		// Fill tree and get tree object back
		BHTree tree = fillTreeBH(start, end);
		// Show octants if set
		if (show_octants) { tree.traverse(&tree); }
		// Update velocities
		updateVelBH(start, end, &tree);
	}
	/*========================================================================*/
};


#endif
