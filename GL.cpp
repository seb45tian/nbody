
/* Copyright (c) Mark J. Kilgard, 1994. */

/* This program is freely distributable without licensing fees 
   and is provided without guarantee or warrantee expressed or 
   implied. This program is -not- in the public domain. */

/* The original glut code was modified by Kurt Debattista - February 2010*/ 

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

// use this if you want the particles to bounce back at the boundary
#define BOUNDARIES 

// number of processors to be used
// could be input as a command line argument PROC_NUM 2;
#define PROC_NUM 2

#include "simulation.h"

// this constructor uses a file to load the particles
//simulation nbody("128.tab"); 
// this constructor creates random particles
simulation nbody(8192);


// global variables for the visualisation 
float roty = 0.0f;
float rotx = 0.0f;
float posz = -400.0f; 
int perspective = 1; 

float ups;
int currentTime = 0;
int previousTime = 0;

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


void draw()
{
#if (PROC_NUM == 1)  
		// update simulation - multithreaded version 
		nbody.MTupdate();
#else
		// update simulation - uniprocessor version 
		// nbody.update();
#endif

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

	// I PUT THE UPDATE HERE TO TEST DRAWING OCTANTS
	nbody.update();
	
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


int main(int argc, char **argv)
{

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
