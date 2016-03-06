#ifndef __SIMULATION_H
#define __SIMULATION_H

#include "particle.h"
#include "node.h"
#include "bhtree.h"
#include <fstream> 

#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

#define G 6.6470831e-11

class simulation 
{
	vec3D boundary;
	particle *Ps;
	int size;
public:
	int updatesCounter;
	/* CONSTRUCTOR */
	simulation (const int n) : size(n), boundary(100,100,100), updatesCounter(0)
	{
		Ps = new particle[n];
		randParticles();
	}

	// load from file
	simulation (const char * filename) : boundary(100,100,100), updatesCounter(0)
	{
		// n-body data from http://bima.astro.umd.edu/nemo/archive/#dubinski
		ifstream fin;
		int count = 0;
		// fixed for now - will have to correct later
		fin.open(filename, ios::in); 
	//	size = 1024;
		fin >> size; 
		Ps = new particle [size]; 
		
		if (fin.good()) {
			while (!fin.eof())
			{
				cout << count << '\r'; 
				fin >> Ps[count];
				//cout << Ps[count];
				count++; 
			}
		} else 
			cout << "Error opening file: " << filename <<  endl;
		fin.close(); 
		
	}


	void randParticles()
	{
		// A REALLY HEAVY PARTICLE IN THE MIDDLE
		// Ps[0].set(vec3D(0,0,0), vec3D(0,0,0),1e8);

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

			// UNCOMMENT IF YOU WANT TO START WITH ZERO VELOCITY / MIGHT CAUSE ERROR FOR LARGE N??
			// vx = 0;
			// vy = 0;
			// vz = 0;

			Ps[i].set(vec3D(x,y,z), vec3D(vx,vy,vz),100000000.0f); 
		}
	}

	/* draw should be called in between glBegin and glEnd */
	void draw()
	{
		glBegin(GL_POINTS);
		// first one is red - just to be able to follow one
		glVertex3dv(Ps[0].pos.v);
		// rest are green
		glColor3f(0.0f,1.0f, 0.0f);
		for (int i = 1; i < size; i++)
		{
			// draw all the particles 
			glVertex3dv(Ps[i].pos.v); 		
		}
		glEnd();

		glColor3f(1.0f, 1.0f, 1.0f); 
		glutWireCube(boundary.x*2);

		updatesCounter++;
	}




	void update()
	{
	    update(0,size);
		//BarnesHut(0,size);
	}



	void update(const unsigned int start, const unsigned int end)
	{
		for (unsigned int i = start; i < end; i++)
		{
			#ifdef BOUNDARIES
			// boundary update makes them recurse back - like asteroids	
			Ps[i].update(boundary);
			#else 
			// proper update
			Ps[i].update();
			#endif 
		}
		//calculateForce(start, end);
		BarnesHut(start, end);
	}



	// this code updates the particles using multi threading 
	void MTupdate()
	{
		// NOTE: this is not the most efficient way of handling threads  
		// the most efficient way of doing this is to 
		// create the threads once and use barriers
		boost::thread * t[PROC_NUM];
		
		// NOTE: this form of load balancing may not be ideal
		// when the work load is not uniform 
		// for the simplest of cases it is 
		unsigned int per_thread = ceilf((float) size/ (float)(PROC_NUM)); 

		for (int i = 0; i < PROC_NUM; i++)
		{
			unsigned int start = i * per_thread;
			unsigned int end = start + per_thread;
			end = (end < size) ? end : size;
			cout <<  i  << " Start = " << start << " End = " << end << endl;
			// launch all threads
			t[i] = new boost::thread(boost::bind(&simulation::update, this, start, end));
		}
		
		// barrier wait on all threads
		for (int i = 0; i < PROC_NUM; i++)
			t[i]->join();

		// remove thread
		// as mentioned above, we would ideally keep using the same threads
		for (int i = 0; i < PROC_NUM; i++)
			delete t[i]; 
	}

	void calculateForce(const unsigned int start, const unsigned int end)
	{
		// calcuate forces here
		for (unsigned int i = start; i < end; i++)
		{
			vec3D sum(0,0,0);
			for (unsigned int j = start; j < end; ++j)
			{
				if(j != i)
				{
					vec3D diff  = (Ps[j].getPos()-Ps[i].getPos());
					double mag   = diff.magnitude();
					double var1 = (Ps[j].getMass())/(mag*mag*mag);
					sum += diff * var1;
				}
			}
			Ps[i].setVel(Ps[i].getVel() + sum*G);
		}
	}



	void BarnesHut(const unsigned int start, const unsigned int end)
	{
		double newlength = boundary.x*2;
		Node root = Node(vec3D(0),newlength);
		BHTree tree = BHTree(root);

		// Add bodies to the tree
		for (unsigned int i = start; i < end; i++)
		{
			if (Ps[i].inNode(root)) tree.addParticle(Ps[i]);
		}
		// UNCOMMENT TO SHOW OCTANTS
		//tree.traverse(&tree);

		//Update the velocities traveling recursively through the tree
		for (unsigned int i = start; i < end; i++)
		{
			tree.updateVelocity(Ps[i]);
			//Ps[i].update(boundary);
		}
	}
	
};


#endif
