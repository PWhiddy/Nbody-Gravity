/*
 * Octant.cpp
 *
 *  Created on: Feb 3, 2016
 *      Author: peterwhidden
 */

#include "Constants.h"

class Octant
{
private:
	vec3 *mid;
	double length;
public:
	Octant(vec3* m, double l)
	{
		mid = m;
		length = l;
	}

	~Octant()
	{
		delete mid;
	}

	double getLength()
	{
		return length;
	}

	inline bool contains(vec3 p)
	{
		return p.x<=mid->x+length/2.0 && p.x>=mid->x-length/2.0 &&
			   p.y<=mid->y+length/2.0 && p.y>=mid->y-length/2.0 &&
			   p.z<=mid->z+length/2.0 && p.z>=mid->z-length/2.0;
	}

	Octant* mUNW()
	{
		vec3 *newMid = new struct vec3;
		double newL = length/4.0;
		newMid->x = mid->x-newL;
		newMid->y = mid->y+newL;
		newMid->z = mid->z+newL;
		return new Octant(newMid, length/2.0);
	}

	Octant* mUNE()
	{
		vec3 *newMid = new struct vec3;
		double newL = length/4.0;
		newMid->x = mid->x+newL;
		newMid->y = mid->y+newL;
		newMid->z = mid->z+newL;
		return new Octant(newMid, length/2.0);
	}

	Octant* mUSW()
	{
		vec3 *newMid = new struct vec3;
		double newL = length/4.0;
		newMid->x = mid->x-newL;
		newMid->y = mid->y-newL;
		newMid->z = mid->z+newL;
		return new Octant(newMid, length/2.0);
	}

	Octant* mUSE()
	{
		vec3 *newMid = new struct vec3;
		double newL = length/4.0;
		newMid->x = mid->x+newL;
		newMid->y = mid->y-newL;
		newMid->z = mid->z+newL;
		return new Octant(newMid, length/2.0);
	}

	Octant* mDNW()
	{
		vec3 *newMid = new struct vec3;
		double newL = length/4.0;
		newMid->x = mid->x-newL;
		newMid->y = mid->y+newL;
		newMid->z = mid->z-newL;
		return new Octant(newMid, length/2.0);
	}

	Octant* mDNE()
	{
		vec3 *newMid = new struct vec3;
		double newL = length/4.0;
		newMid->x = mid->x+newL;
		newMid->y = mid->y+newL;
		newMid->z = mid->z-newL;
		return new Octant(newMid, length/2.0);
	}

	Octant* mDSW()
	{
		vec3 *newMid = new struct vec3;
		double newL = length/4.0;
		newMid->x = mid->x-newL;
		newMid->y = mid->y-newL;
		newMid->z = mid->z-newL;
		return new Octant(newMid, length/2.0);
	}

	Octant* mDSE()
	{
		vec3 *newMid = new struct vec3;
		double newL = length/4.0;
		newMid->x = mid->x+newL;
		newMid->y = mid->y-newL;
		newMid->z = mid->z-newL;
		return new Octant(newMid, length/2.0);
	}

};

