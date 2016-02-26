#include <iostream>
#include <math.h>
#include "vec3d.h"
#include "node.h"

using namespace std;

int main(int argc, char const *argv[])
{
	vec3D a(2);
	double tm = 1;

	const Node *A = new Node(a, tm);
	const Node *B = new Node(a, tm);
	const Node *C = new Node(a, tm);
	const Node *D = new Node(a, tm);
	const Node *E = new Node(a, tm);
	const Node *F = new Node(a, tm);
	const Node *G = new Node(a, tm);
	const Node *H = new Node(a, tm);

	const Node *N = new Node(A,B,C,D,E,F,G,H);

	cout << N->get_totalMass() << endl;
	cout << N->get_com() << endl;

	N->traverse();

	return 0;
}