#ifndef __BHTREE_H
#define __BHTREE_H

#include "node.h"
#include "particle.h"

class BHTree
{
private:
	particle p;
	Node octant;

	// Pointers to sub-trees
	BHTree *SWU; // South-West-Up
	BHTree *NWU; // North-West-Up
	BHTree *SEU; // South-East-Up
	BHTree *NEU; // North-East-Up
	BHTree *SWD; // South-West-Down
	BHTree *NWD; // North-West-Down
	BHTree *SED; // South-East-Down
	BHTree *NED; // North-East-Down

	void set(Node noctant = Node())
	{
		p.set(vec3D(0), vec3D(0), 0.0);
		octant = noctant;
		SWU = NULL;
		NWU = NULL;
		SEU = NULL;
		NEU = NULL;
		SWD = NULL;
		NWD = NULL;
		SED = NULL;
		NED = NULL;
	}


public:
	/* CONSTRUCTOR */
	BHTree() {set();}

	BHTree(Node noctant) {set(noctant);}

	// Check if tree is external tree
	bool isExternal(BHTree tree)
	{
		if (tree.SWU==NULL && tree.NWU==NULL && tree.SEU==NULL && tree.NEU==NULL
			&& tree.SWD==NULL && tree.NWD==NULL && tree.SED==NULL && tree.NED==NULL)
		{
			return true;
		}
		return false;
	}


	/* FUNCTION TO RETURN A PARTICLE */
	particle getParticle()
	{
		return p;
	}



	/* THIS IS THE RECURSIVE FUNCTION TO ADD PARTICLES TO THE TREE! */
	void addParticle(particle np)
	{
		// If there is no particle in the current tree, add it and we are done
		if (p.empty()==true)
		{
			p = np;
			return;
		}
		//If there already is a particle, but it's not an external node
    	//combine the two particles and figure out which octant of the 
    	//tree it should be located in. Then recursively update the nodes.
		else if (this->isExternal(*this) == false)
		{
			p = np.combine(p,np);

			Node southwestup = octant.SWU();
			if (np.inNode(southwestup))
			{
				if (SWU==NULL) {SWU = new BHTree(southwestup);}
				SWU->addParticle(np);
			}
			else
			{
				Node northwestup = octant.NWU();
				if (np.inNode(northwestup))
				{
					if (NWU==NULL) {NWU = new BHTree(northwestup);}
					NWU->addParticle(np);
				}
				else
				{
					Node southeastup = octant.SEU();
					if (np.inNode(southeastup))
					{
						if (SEU==NULL) {SEU = new BHTree(southeastup);}
						SEU->addParticle(np);
					}
					else
					{
						Node northeastup = octant.NEU();
						if (np.inNode(northeastup))
						{
							if (NEU==NULL) {NEU = new BHTree(northeastup);}
							NEU->addParticle(np);
						}
						else
						{
							Node southwestdown = octant.SWD();
							if (np.inNode(southwestdown))
							{
								if (SWD==NULL) {SWD = new BHTree(southwestdown);}
								SWD->addParticle(np);
							}
							else
							{
								Node northwestdown = octant.NWD();
								if (np.inNode(northwestdown))
								{
									if (NWD==NULL) {NWD = new BHTree(northwestdown);}
									NWD->addParticle(np);
								}
								else
								{
									Node southeastdown = octant.SED();
									if (np.inNode(southeastdown))
									{
										if (SED==NULL) {SED = new BHTree(southeastdown);}
										SED->addParticle(np);
									}
									else
									{
										Node northeastdown = octant.NED();
										if (NED==NULL) {NED = new BHTree(northeastdown);}
										NED->addParticle(np);
									}
								}
							}
						}
					}
				}
			}
		}
		//If the node is external and contains another particle, create new nodes (BHTrees)
		//where the bodies should go, update the node, and end (do not do anything recursively)
		else if (this->isExternal(*this) == true)
		{
			//cout << "External Node: " << this << endl;
			particle c;
			c = p;
			Node southwestup = octant.SWU();
			if (c.inNode(southwestup))
			{
				if (SWU==NULL) {SWU = new BHTree(southwestup);}
				SWU->addParticle(c);    
			}
			else
			{
				Node northwestup = octant.NWU();
				if (c.inNode(northwestup))
				{
					if (NWU==NULL) {NWU = new BHTree(northwestup);}
					NWU->addParticle(c);    
				}
				else
				{
					Node southeastup = octant.SEU();
					if (c.inNode(southeastup))
					{
						if (SEU==NULL) {SEU = new BHTree(southeastup);}
						SEU->addParticle(c);    
					}
					else
					{
						Node northeastup = octant.NEU();
						if (c.inNode(northeastup))
						{
							if (NEU==NULL) {NEU = new BHTree(northeastup);}
							NEU->addParticle(c);    
						}
						else
						{
							Node southwestdown = octant.SWD();
							if (c.inNode(southwestdown))
							{
								if (SWD==NULL) {SWD = new BHTree(southwestdown);}
								SWD->addParticle(c);    
							}
							else
							{
								Node northwestdown = octant.NWD();
								if (c.inNode(northwestdown))
								{
									if (NWD==NULL) {NWD = new BHTree(northwestdown);}
									NWD->addParticle(c);    
								}
								else
								{
									Node southeastdown = octant.SED();
									if (c.inNode(southeastdown))
									{
										if (SED==NULL) {SED = new BHTree(southeastdown);}
										SED->addParticle(c);    
									}
									else
									{
										Node northeastdown = octant.NED();
										if (NED==NULL) {NED = new BHTree(northeastdown);}
										NED->addParticle(c);    
									}
								}
							}
						}
					}
				}
			}
			this->addParticle(np);
      	}
	}



	/* FUNCTION TO UPDATE THE VELOCITIES OF THE PARTICLES RECURSIVELY */
	void updateVelocity(particle &np, const double theta, const double eps)
	{
		// calculate the distance between the two particles
		vec3D diff = p.getPos() - np.getPos();
		double distance = diff.magnitude();

		// if the node is external, add the velocity
		if (this->isExternal(*this) == true)
		{
			if (!(p==np))
			{
				np.addVelocity(p, eps);
				return;
			}
		}
		// if the node is sufficiently far away (theta criteria), also add the velocity
		else if (octant.get_length()/(distance)< theta)
		{
			np.addVelocity(p, eps);
		}
		// else traverse through the childe nodes and update the velocities
		else
		{
			if (this->SWU!=NULL) {this->SWU->updateVelocity(np, theta, eps);}
			if (this->NWU!=NULL) {this->NWU->updateVelocity(np, theta, eps);}
			if (this->SEU!=NULL) {this->SEU->updateVelocity(np, theta, eps);}
			if (this->NEU!=NULL) {this->NEU->updateVelocity(np, theta, eps);}
			if (this->SWD!=NULL) {this->SWD->updateVelocity(np, theta, eps);}
			if (this->NWD!=NULL) {this->NWD->updateVelocity(np, theta, eps);}
			if (this->SED!=NULL) {this->SED->updateVelocity(np, theta, eps);}
			if (this->NED!=NULL) {this->NED->updateVelocity(np, theta, eps);}
		}
	}



	/*THIS FUNCTION TRAVERSES THE TREE AND DRAWS EVERY OCTANT IT FINDS */
	void traverse(BHTree *tree)
	{
		if (tree->p.empty()==false)
		{
			// SOME AWESOME DRAWING FUN OF OCTANTS!!!
			vec3D trans = tree->octant.get_mid();
			glColor3f(1, 0, 0);   	//color of object
			glPushMatrix();		  	// ??
			glTranslatef(trans.x,trans.y,trans.z);	// need to translate origin
			glutWireCube(tree->octant.get_length());		// size of cube
			glPopMatrix();			// ??
		}

		if (tree->SWU!=NULL) {traverse(tree->SWU);}
		if (tree->NWU!=NULL) {traverse(tree->NWU);}
		if (tree->SEU!=NULL) {traverse(tree->SEU);}
		if (tree->NEU!=NULL) {traverse(tree->NEU);}
		if (tree->SWD!=NULL) {traverse(tree->SWD);}
		if (tree->NWD!=NULL) {traverse(tree->NWD);}
		if (tree->SED!=NULL) {traverse(tree->SED);}
		if (tree->NED!=NULL) {traverse(tree->NED);}

	}



};


#endif