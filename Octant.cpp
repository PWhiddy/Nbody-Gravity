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
	vec3 mid;
	double length;

public:

	Octant(double x, double y, double z, double l): mid(x, y, z)
	{
		length = l;
	}

	double getLength()
	{
		return length;
	}

	inline bool contains(vec3 p)
	{
		return p.x<=mid.x+length/2.0 && p.x>=mid.x-length/2.0 &&
			   p.y<=mid.y+length/2.0 && p.y>=mid.y-length/2.0 &&
			   p.z<=mid.z+length/2.0 && p.z>=mid.z-length/2.0;
	}

	Octant* mUNW()
	{
		double newL = length/4.0;
		return new Octant(mid.x-newL, mid.y+newL, mid.z+newL, length/2.0);
	}

	Octant* mUNE()
	{
		double newL = length/4.0;
		return new Octant(mid.x+newL, mid.y+newL, mid.z+newL, length/2.0);
	}

	Octant* mUSW()
	{
		double newL = length/4.0;
		return new Octant(mid.x-newL, mid.y-newL, mid.z+newL, length/2.0);
	}

	Octant* mUSE()
	{
		double newL = length/4.0;
		return new Octant(mid.x+newL, mid.y-newL, mid.z+newL, length/2.0);
	}

	Octant* mDNW()
	{
		double newL = length/4.0;
		return new Octant(mid.x-newL, mid.y+newL, mid.z-newL, length/2.0);
	}

	Octant* mDNE()
	{
		double newL = length/4.0;
		return new Octant(mid.x+newL, mid.y+newL, mid.z-newL, length/2.0);
	}

	Octant* mDSW()
	{
		double newL = length/4.0;
		return new Octant(mid.x-newL, mid.y-newL, mid.z-newL, length/2.0);
	}

	Octant* mDSE()
	{
		double newL = length/4.0;
		return new Octant(mid.x+newL, mid.y-newL, mid.z-newL, length/2.0);
	}

};

