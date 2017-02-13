/*
 * Bhtree.cpp
 *
 *  Created on: Feb 3, 2016
 *      Author: peterwhidden
 */

#include "Octant.cpp"
#include <random>

class Bhtree
{
private:
	body *myBod;
	body *toDelete;
	Octant *octy;
	Bhtree *UNW;
	Bhtree *UNE;
	Bhtree *USW;
	Bhtree *USE;
	Bhtree *DNW;
	Bhtree *DNE;
	Bhtree *DSW;
	Bhtree *DSE;

public:
	Bhtree(Octant* o)
	{
		myBod = NULL;
		toDelete = NULL;
		octy = o;
		UNW = NULL;
		UNE = NULL;
		USW = NULL;
		USE = NULL;
		DNW = NULL;
		DNE = NULL;
		DSW = NULL;
		DSE = NULL;
	}

	~Bhtree()
	{
		// check if each is ==0 (null)
		if (UNW!=NULL) delete UNW; //UNW->~Bhtree();
		if (UNE!=NULL) delete UNE; //UNE->~Bhtree();
		if (USW!=NULL) delete USW; //USW->~Bhtree();
		if (USE!=NULL) delete USE; //USE->~Bhtree();
		if (DNW!=NULL) delete DNW; //DNW->~Bhtree();
		if (DNE!=NULL) delete DNE; //DNE->~Bhtree();
		if (DSW!=NULL) delete DSW; //DSW->~Bhtree();
		if (DSE!=NULL) delete DSE; //DSE->~Bhtree();
		if (toDelete!=NULL) delete toDelete;
		delete octy; //octy->~Octant();
	//	if (!isExternal() && myBod!= NULL && myBod->velocity.x==0.0) delete myBod;
	//	delete myBod;
	}

	bool isExternal()
	{
		return UNW==NULL && UNE==NULL && USW==NULL && USE==NULL &&
			   DNW==NULL && DNE==NULL && DSW==NULL && DSE==NULL;
	}

	void insert(body* insertBod)
	{
		if (myBod==NULL)
		{
			myBod = insertBod;
		} else //if (!isExternal())
		{
			bool isExtern = isExternal();
			body *updatedBod;
			if (!isExtern)
			{
				updatedBod = new struct body;
				updatedBod->position.x = (insertBod->position.x*insertBod->mass +
								       myBod->position.x*myBod->mass) /
								  (insertBod->mass+myBod->mass);
				updatedBod->position.y = (insertBod->position.y*insertBod->mass +
									   myBod->position.y*myBod->mass) /
								  (insertBod->mass+myBod->mass);
				updatedBod->position.z = (insertBod->position.z*insertBod->mass +
									   myBod->position.z*myBod->mass) /
								  (insertBod->mass+myBod->mass);
				updatedBod->mass = insertBod->mass+myBod->mass;
			//	delete myBod;
				if (toDelete!=NULL) delete toDelete;
				toDelete = updatedBod;
				myBod = updatedBod;
				updatedBod = insertBod;
			} else {
				updatedBod = myBod;
			}
			Octant *unw = octy->mUNW();
			if (unw->contains(updatedBod->position))
			{
				if (UNW==NULL) { UNW = new Bhtree(unw); }
				else { delete unw; }
				UNW->insert(updatedBod);
			} else {
				delete unw;
				Octant *une = octy->mUNE();
				if (une->contains(updatedBod->position))
				{
					if (UNE==NULL) { UNE = new Bhtree(une); }
					else { delete une; }
					UNE->insert(updatedBod);
				} else {
					delete une;
					Octant *usw = octy->mUSW();
					if (usw->contains(updatedBod->position))
					{
						if (USW==NULL) { USW = new Bhtree(usw); }
						else { delete usw; }
						USW->insert(updatedBod);
					} else {
						delete usw;
						Octant *use = octy->mUSE();
						if (use->contains(updatedBod->position))
						{
							if (USE==NULL) { USE = new Bhtree(use); }
							else { delete use; }
							USE->insert(updatedBod);
						} else {
							delete use;
							Octant *dnw = octy->mDNW();
							if (dnw->contains(updatedBod->position))
							{
								if (DNW==NULL) { DNW = new Bhtree(dnw); }
								else { delete dnw; }
								DNW->insert(updatedBod);
							} else {
								delete dnw;
								Octant *dne = octy->mDNE();
								if (dne->contains(updatedBod->position))
								{
									if (DNE==NULL) { DNE = new Bhtree(dne); }
									else { delete dne; }
									DNE->insert(updatedBod);
								} else {
									delete dne;
									Octant *dsw = octy->mDSW();
									if (dsw->contains(updatedBod->position))
									{
										if (DSW==NULL) { DSW = new Bhtree(dsw); }
										else { delete dsw; }
										DSW->insert(updatedBod);
									} else {
										delete dsw;
										Octant *dse = octy->mDSE();
										if (DSE==NULL) { DSE = new Bhtree(dse); }
										else { delete dse; }
										DSE->insert(updatedBod);
										}
									}
								}
							}
						}
					}
				}
		//	delete updatedBod;
			if (isExtern) {
				insert(insertBod);
			}
		}
	}

	double magnitude(vec3* v)
	{
		return sqrt(v->x*v->x+v->y*v->y+v->z*v->z);
	}

	double magnitude( double x, double y, double z)
	{
		return sqrt(x*x+y*y+z*z);
	}

	void singleInteract(struct body* target, struct body* other, bool singlePart)
	{
		vec3 *posDiff = new struct vec3;
		posDiff->x = (target->position.x-other->position.x)*TO_METERS;
		posDiff->y = (target->position.y-other->position.y)*TO_METERS;
		posDiff->z = (target->position.z-other->position.z)*TO_METERS;
		double dist = magnitude(posDiff);
		double F = TIME_STEP*(G*target->mass*other->mass) / ((dist*dist + SOFTENING*SOFTENING) * dist);

		target->accel.x -= F*posDiff->x/target->mass;
		target->accel.y -= F*posDiff->y/target->mass;
		target->accel.z -= F*posDiff->z/target->mass;
		delete posDiff;
		
		//Friction
	#if ENABLE_FRICTION
		if (singlePart)
		{
			double friction = 0.5/pow(2.0,FRICTION_FACTOR*(
					((dist+SOFTENING))/(TO_METERS)));
		//	cout << friction << "\n";
			if (friction>0.0001 && ENABLE_FRICTION)
			{
				target->accel.x += friction*(other->velocity.x-target->velocity.x)/2;
				target->accel.y += friction*(other->velocity.y-target->velocity.y)/2;
				target->accel.z += friction*(other->velocity.z-target->velocity.z)/2;
			}
		}
	#endif		


	}

	void interactInTree(body* bod)
	{
		if (isExternal())
		{
			if (myBod!=bod) { singleInteract(bod, myBod, true); }
		}
		else if (octy->getLength() /
				magnitude(myBod->position.x-bod->position.x,
						  myBod->position.y-bod->position.y,
						  myBod->position.z-bod->position.z) < MAX_DISTANCE)
		{
			singleInteract(bod, myBod, false);
		} else {
			if (UNW!=NULL) UNW->interactInTree(bod);
			if (UNE!=NULL) UNE->interactInTree(bod);
			if (USW!=NULL) USW->interactInTree(bod);
			if (USE!=NULL) USE->interactInTree(bod);
			if (DNW!=NULL) DNW->interactInTree(bod);
			if (DNE!=NULL) DNE->interactInTree(bod);
			if (DSW!=NULL) DSW->interactInTree(bod);
			if (DSE!=NULL) DSE->interactInTree(bod);
		}
	}

};


