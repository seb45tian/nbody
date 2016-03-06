#ifndef __BHTREE_H
#define __BHTREE_H

#include "node.h"
#include "particle.h"

class BHTree
{
private:
	particle *p;
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


public:
	/* CONSTRUCTOR */
	BHTree(Node noctant)
	{
		p = NULL;
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




	void addParticle(particle *np)
	{
		if (p==NULL)
		{
			this->p = np;
			cout << "Tree: " << this << endl;
			cout << "Added particle\n" << *np;
			return;
		}
		//If there already is a particle, but it's not an external node
    	//combine the two particles and figure out which octant of the 
    	//tree it should be located in. Then recursively update the nodes below it.
		else if (this->isExternal(*this) == false)
		{
			cout << "Internal Node: " << this << endl;
			*p = np->combine(p,np);

			Node southwestup = octant.SWU();
			if (np->inNode(southwestup))
			{
				if (SWU==NULL) {SWU = new BHTree(southwestup);}
				SWU->addParticle(np);
			}
			else
			{
				Node northwestup = octant.NWU();
				if (np->inNode(northwestup))
				{
					if (NWU==NULL) {NWU = new BHTree(northwestup);}
					NWU->addParticle(np);
				}
				else
				{
					Node southeastup = octant.SEU();
					if (np->inNode(southeastup))
					{
						if (SEU==NULL) {SEU = new BHTree(southeastup);}
						SEU->addParticle(np);
					}
					else
					{
						Node northeastup = octant.NEU();
						if (np->inNode(northeastup))
						{
							if (NEU==NULL) {NEU = new BHTree(northeastup);}
							cout << "Add particle to NEU:\n";
							NEU->addParticle(np);
						}
						else
						{
							Node southwestdown = octant.SWD();
							if (np->inNode(southwestdown))
							{
								if (SWD==NULL) {SWD = new BHTree(southwestdown);}
								cout << "Add particle to SWD:\n";
								SWD->addParticle(np);
							}
							else
							{
								Node northwestdown = octant.NWD();
								if (np->inNode(northwestdown))
								{
									if (NWD==NULL) {NWD = new BHTree(northwestdown);}
									NWD->addParticle(np);
								}
								else
								{
									Node southeastdown = octant.SED();
									if (np->inNode(southeastdown))
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
		//If the node is external and contains another particle, create BHTrees
		//where the bodies should go, update the node, and end (do not do anything recursively)
		else if (this->isExternal(*this) == true)
		{
			cout << "External Node: " << this << endl;
			particle *c = this->p;
			Node southwestup = octant.SWU();
			if (c->inNode(southwestup))
			{
				if (SWU==NULL) {SWU = new BHTree(southwestup);}
				SWU->addParticle(c);    
			}
			else
			{
				Node northwestup = octant.NWU();
				if (c->inNode(northwestup))
				{
					if (NWU==NULL) {NWU = new BHTree(northwestup);}
					NWU->addParticle(c);    
				}
				else
				{
					Node southeastup = octant.SEU();
					if (c->inNode(southeastup))
					{
						if (SEU==NULL) {SEU = new BHTree(southeastup);}
						SEU->addParticle(c);    
					}
					else
					{
						Node northeastup = octant.NEU();
						if (c->inNode(northeastup))
						{
							if (NEU==NULL) {NEU = new BHTree(northeastup);}
							cout << "Add particle to NEU:\n";
							NEU->addParticle(c);    
						}
						else
						{
							Node southwestdown = octant.SWD();
							if (c->inNode(southwestdown))
							{
								if (SWD==NULL) {SWD = new BHTree(southwestdown);}
								cout << "Add particle to SWD:\n";
								SWD->addParticle(c);    
							}
							else
							{
								Node northwestdown = octant.NWD();
								if (c->inNode(northwestdown))
								{
									if (NWD==NULL) {NWD = new BHTree(northwestdown);}
									NWD->addParticle(c);    
								}
								else
								{
									Node southeastdown = octant.SED();
									if (c->inNode(southeastdown))
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


	//Start at the main node of the tree. Then, recursively go to each branch
	//until either we reach an external node or we reach a node that is sufficiently
	//far away that the external nodes would not matter much.
	void updateVelocity(particle *np)
	{
		vec3D diff = p->getPos() - np->getPos();
		double distance = diff.magnitude();

		if (this->isExternal(*this) == true)
		{
			if (p != np) np->calcNewVelocity(*p);
		}
		else if (octant.get_length()/(distance)<2)
		{
			np->calcNewVelocity(*p);
		}
		else
		{
			if (SWU!=NULL) SWU->updateVelocity(np);
			if (NWU!=NULL) NWU->updateVelocity(np);
			if (SEU!=NULL) SEU->updateVelocity(np);
			if (NEU!=NULL) NEU->updateVelocity(np);
			if (SWD!=NULL) SWD->updateVelocity(np);
			if (NWD!=NULL) NWD->updateVelocity(np);
			if (SED!=NULL) SED->updateVelocity(np);
			if (NED!=NULL) NED->updateVelocity(np);
		}
	}


	particle getParticle()
	{
		return *p;
	}


	void traverse(BHTree *tree)
	{
		cout << tree << endl;
		cout << tree->getParticle();

		if (tree->SWU!=NULL)
		{
			cout << "SWU has particle" << endl;
			traverse(tree->SWU);
		}
		if (tree->NWU!=NULL)
		{
			cout << "NWU has particle" << endl;
			traverse(tree->NWU);
		}
		if (tree->SEU!=NULL)
		{
			cout << "SEU has particle" << endl;
			traverse(tree->SEU);
		}
		if (tree->NEU!=NULL)
		{
			cout << "NEU has particle" << endl;
			traverse(tree->NEU);
		}
		if (tree->SWD!=NULL)
		{
			cout << "SWD has particle" << endl;
			traverse(tree->SWD);
		}
		if (tree->NWD!=NULL)
		{
			cout << "NWD has particle" << endl;
			traverse(tree->NWD);
		}
		if (tree->SED!=NULL)
		{
			cout << "SED has particle" << endl;
			traverse(tree->SED);
		}
		if (tree->NED!=NULL)
		{
			cout << "NED has particle" << endl;
			traverse(tree->NED);
		}
	}



};


#endif