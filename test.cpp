#include <iostream>
#include <math.h>
#include "vec3d.h"
#include "node.h"
#include "bhtree.h"
#include "particle.h"

using namespace std;

int main(int argc, char const *argv[])
{
	const int size = 3;
	particle Ps[size];

	Ps[0].set(vec3D(0,1,1), vec3D(0,0,0), 1);
	Ps[1].set(vec3D(0,-1,-1), vec3D(0,0,0), 1);
	Ps[2].set(vec3D(1,1,1), vec3D(0,0,0), 1);




	Node root = Node(vec3D(0),20);
	BHTree *tree = new BHTree(root);


	for (int i = 0; i < size; ++i)
	{
		cout << "Particle id: " << i << endl;
		if (Ps[i].inNode(root)) {tree->addParticle(Ps[i]);}
		
	}

	cout << "#############################" << endl;
	cout << "####### Traverse Tree #######" << endl;
	cout << "#############################" << endl;
	tree->traverse(tree);



	return 0;
}