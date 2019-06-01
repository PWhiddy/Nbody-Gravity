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
	body myBod;
	Octant octy;
	Bhtree *UNW;
	Bhtree *UNE;
	Bhtree *USW;
	Bhtree *USE;
	Bhtree *DNW;
	Bhtree *DNE;
	Bhtree *DSW;
	Bhtree *DSE;

public:

#if 1
	Bhtree(Octant&& o): octy(std::move(o))
	{
		UNW = NULL;
		UNE = NULL;
		USW = NULL;
		USE = NULL;
		DNW = NULL;
		DNE = NULL;
		DSW = NULL;
		DSE = NULL;
	}
#endif

#if 0
	Bhtree(const Octant& o): octy(o)
	{
		UNW = NULL;
		UNE = NULL;
		USW = NULL;
		USE = NULL;
		DNW = NULL;
		DNE = NULL;
		DSW = NULL;
		DSE = NULL;
	}
#endif
	
	const Octant& octant () const { return octy; }

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
	}

	bool isExternal()
	{
		return UNW==NULL && UNE==NULL && USW==NULL && USE==NULL &&
			   DNW==NULL && DNE==NULL && DSW==NULL && DSE==NULL;
	}

	void insert(body* insertBod)
	{
		if (myBod.mass == 0)
		{
			myBod = *insertBod;
		} else //if (!isExternal())
		{
			bool isExtern = isExternal();
			body* bodToUpdate;
			if (!isExtern)
			{
				myBod.position.x = (insertBod->position.x*insertBod->mass +
								       myBod.position.x*myBod.mass) /
								  (insertBod->mass+myBod.mass);
				myBod.position.y = (insertBod->position.y*insertBod->mass +
									   myBod.position.y*myBod.mass) /
								  (insertBod->mass+myBod.mass);
				myBod.position.z = (insertBod->position.z*insertBod->mass +
									   myBod.position.z*myBod.mass) /
								  (insertBod->mass+myBod.mass);
				myBod.mass += insertBod->mass;
				bodToUpdate = insertBod;
			} else {
				bodToUpdate = &myBod;
			}
			Octant&& unw = octy.mUNW();
			if (unw.contains(bodToUpdate->position))
			{
				if (UNW==NULL) { UNW = new Bhtree(std::move(unw)); }
				UNW->insert(bodToUpdate);
			} else {
				Octant&& une = octy.mUNE();
				if (une.contains(bodToUpdate->position))
				{
					if (UNE==NULL) { UNE = new Bhtree(std::move(une)); }
					UNE->insert(bodToUpdate);
				} else {
					Octant&& usw = octy.mUSW();
					if (usw.contains(bodToUpdate->position))
					{
						if (USW==NULL) { USW = new Bhtree(std::move(usw)); }
						USW->insert(bodToUpdate);
					} else {
						Octant&& use = octy.mUSE();
						if (use.contains(bodToUpdate->position))
						{
							if (USE==NULL) { USE = new Bhtree(std::move(use)); }
							USE->insert(bodToUpdate);
						} else {
							Octant&& dnw = octy.mDNW();
							if (dnw.contains(bodToUpdate->position))
							{
								if (DNW==NULL) { DNW = new Bhtree(std::move(dnw)); }
								DNW->insert(bodToUpdate);
							} else {
								Octant&& dne = octy.mDNE();
								if (dne.contains(bodToUpdate->position))
								{
									if (DNE==NULL) { DNE = new Bhtree(std::move(dne)); }
									DNE->insert(bodToUpdate);
								} else {
									Octant&& dsw = octy.mDSW();
									if (dsw.contains(bodToUpdate->position))
									{
										if (DSW==NULL) { DSW = new Bhtree(std::move(dsw)); }
										DSW->insert(bodToUpdate);
									} else {
										Octant&& dse = octy.mDSE();
										if (DSE==NULL) { DSE = new Bhtree(std::move(dse)); }
										DSE->insert(bodToUpdate);
										}
									}
								}
							}
						}
					}
				}
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
		vec3 posDiff;
		posDiff.x = (target->position.x-other->position.x)*TO_METERS;
		posDiff.y = (target->position.y-other->position.y)*TO_METERS;
		posDiff.z = (target->position.z-other->position.z)*TO_METERS;
		double dist = magnitude(&posDiff);

if (dist > 0)
{
		double F = TIME_STEP*(G*target->mass*other->mass) / ((dist*dist + SOFTENING*SOFTENING) * dist);

		target->accel.x -= F*posDiff.x/target->mass;
		target->accel.y -= F*posDiff.y/target->mass;
		target->accel.z -= F*posDiff.z/target->mass;
		
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
    #else
        (void)singlePart;
	#endif		
}

	}

	void interactInTree(body* bod)
	{
		if (isExternal())
		{
			if (bod != &myBod) { singleInteract(bod, &myBod, true); }
		}
		else if (octy.getLength() /
				magnitude(myBod.position.x-bod->position.x,
						  myBod.position.y-bod->position.y,
						  myBod.position.z-bod->position.z) < MAX_DISTANCE)
		{
			singleInteract(bod, &myBod, false);
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


