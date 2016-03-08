
/* Copyright (c) Mark J. Kilgard, 1994. */

/* This program is freely distributable without licensing fees 
   and is provided without guarantee or warrantee expressed or 
   implied. This program is -not- in the public domain. */

/* The original glut code was modified by Kurt Debattista - February 2010*/

/* Modified version for CY903, Assignment 2 by Sebastian Potthoff - March 2016 */ 

#include <stdlib.h>
#include <stdio.h>
#ifndef WIN32
	#include <unistd.h>
#else
	#define random rand
	#define srandom srand
#endif

#ifdef __APPLE__
	#include <GLUT/glut.h> // FOR OSX
#else
   #include <GL/glut.h> // FOR LINUX (AND WINDOWS?)
#endif

#include <math.h>
#include <time.h>
#include <iostream> 
#include <fstream>
#include <stdio.h>
#include <boost/program_options.hpp>

// number of processors to be used
// could be input as a command line argument PROC_NUM 2;
#define PROC_NUM 1

// Include simulation AFTER #define statements
#include "simulation.h"


/*========================================================================*/
/*========================================================================*/

/* global variables for the simulation */
int size,proc_num;
double theta, eps;
bool boundary,barneshut,show_octants;
std::string infile;

// create an empty simulation object
simulation nbody;

/*========================================================================*/



// global variables for the visualisation 
float roty = 0.0f;
float rotx = 0.0f;
float posz = -400.0f; 
int perspective = 1; 

float ups;
int currentTime = 0;
int previousTime = 0;


/*========================================================================*/
namespace po = boost::program_options;
void cli_parser(int ac, char **av)
{
	try {

		po::options_description desc("Allowed options");
		desc.add_options()
			("help,h", "produce help message")
			("particles,p", po::value<int>(&size)->default_value(100), "set number of particles")
			("infile,f",  po::value<std::string>(&infile), "specify input file")
			("epsilon,e", po::value<double>(&eps)->default_value(3.0e4), "set softening factor")
			("theta,t", po::value<double>(&theta)->default_value(1.0), "set number of particles")
			("Barneshut,B",  po::value<bool>(&barneshut)->default_value(0), "use Barnes-Hut method")
			("boundary,b",  po::value<bool>(&boundary)->default_value(true), "set periodic boundary conditions")
			("octants,o",  po::value<bool>(&show_octants)->default_value(false), "make octants visible")
			("threads,n",  po::value<int>(&proc_num)->default_value(1), "specify number of threads")
		;

		po::variables_map vm;        
		po::store(po::parse_command_line(ac, av, desc), vm);
		po::notify(vm);    

		if (vm.count("help")) {
			cout << desc << "\n";
			exit(0);
		}

		if (vm.count("infile")) {
			cout << "Reading from " << vm["infile"].as<std::string>() << " ...\n";
			nbody = simulation(infile,size, boundary, show_octants, barneshut, theta, eps);
		}
		else if (vm.count("particles")) {
			cout << "Number of particles: " << vm["particles"].as<int>() << endl;
			// Initialise simulation object nbody
			nbody = simulation(size, boundary, show_octants, barneshut, theta, eps);
		}
		if (vm.count("epsilon")) {
			cout << "Epsilon: " << vm["epsilon"].as<double>() << endl;
		}
		if (vm.count("theta")) {
			cout << "Theta: " << vm["theta"].as<double>() << endl;
		}
		if (vm.count("Barneshut") && vm["Barneshut"].as<bool>()) {
			cout << "Using Barnes-Hut method." << endl;
		}
		if (vm.count("boundary") && vm["boundary"].as<bool>()) {
			cout << "Using periodic boundary conditions." << endl;
		}
		if (vm.count("octants") && vm["octants"].as<bool>()) {
			cout << "Displaying octants." << endl;
		}
		if (vm.count("threads") && vm["threads"].as<int>() > 1) {
			cout << "Number of threads: " << vm["threads"].as<int>() << endl;
		}
	}
	catch(exception& e) {
		cerr << "error: " << e.what() << "\n";
		exit(1);
	}
	catch(...) {
		cerr << "Exception of unknown type!\n";
	}


	// exit(0);
}

/*========================================================================*/
// Added a function for printing output in the graphics window
void output(int x, int y, float r, float g, float b, void* font, std::string string)
{
  glColor3f( r, g, b );
  glRasterPos2f(x, y);
  int len, i;
  len = string.length();
  for (i = 0; i < len; i++) {
	glutBitmapCharacter(font, string[i]);
  }
}

/*========================================================================*/
void init3D() 
{ 
	 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (perspective) 
	  gluPerspective(40.0, 1.0, 1.0, 1000.0);
	else
	  glOrtho(-110.0, 110.0, -110.0, 110.0, -500, 500.0);  
	glMatrixMode(GL_MODELVIEW);
}

/*========================================================================*/
void draw()
{
	// re-initialise 3D - in case perspective is changed
	init3D(); 

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glColor3f(1.0f, 0.0f, 0.0f); 
	glLoadIdentity(); 

	// update camera coordinates
	glTranslatef(0.0f, 0.0f, posz);
	glRotatef(roty, 0.0f, 1.0f, 0.0f);
	glRotatef(rotx, 1.0f, 0.0f, 0.0f);

	// relocated the update function here to be able to draw octants 
	if (proc_num > 1)
	{
		// update simulation - multithreaded version 
		nbody.MTupdate(proc_num);
	}
	else
	{
		// update simulation - uniprocessor version 
		nbody.update();
	}
	
	nbody.draw();


	/**** THIS PART CACULATES THE UPDATES PER SECOND ****/
	/****************************************************/
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	//  Calculate time passed
	int timeInterval = currentTime - previousTime;
	if(timeInterval > 1000)
	{
		//  calculate the number of frames per second
		ups = nbody.updatesCounter / (timeInterval / 1000.0f);
		//  Set time
		previousTime = currentTime;
		//  Reset updates counter
		nbody.updatesCounter = 0;
	}
	// Output for the console
	std::cout << "UPS: "<< ups <<'\r' << std::flush;
	// Output for the display window
	std::string out = "Updates per second: " + std::to_string(ups);
	output(0,100, 0,198,247, GLUT_BITMAP_HELVETICA_18, out);
	/****************************************************/


	glutSwapBuffers();
	glutPostRedisplay();

}

/*========================================================================*/
// I changed, and added some keyboard strokes!
void keyboard(unsigned char ch, int x, int y)
{
	switch (ch) 
	{
		case 'a': roty += 1.0f; break; 
		case 'd': roty -= 1.0f; break; 
		case 'w': rotx += 1.0f; break; 
		case 's': rotx -= 1.0f; break;
		case 'z': posz += 1.0f; break; 
		case 'x': posz -= 1.0f; break;
		case ' ': perspective = !perspective; break; 
		case 27: exit(0); break;
	}
}

/*========================================================================*/
/*========================================================================*/

int main(int argc, char **argv)
{
	cli_parser(argc, argv);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutCreateWindow("nbody");
	glutReshapeWindow(1024,768);
	glutDisplayFunc(draw);
	glutKeyboardFunc(keyboard);
	glClearDepth(1.0);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	init3D(); 
	srandom(time(NULL));

	glutMainLoop();

	return 0;             
}
