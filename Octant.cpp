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

	Octant(Octant&& o): mid(std::move(o.mid)), length(std::move(o.length))
	{
	}

	Octant(const Octant& o): mid(o.mid), length(o.length)
	{
	}

	double getLength() const
	{
		return length;
	}

	inline bool contains(const vec3& p) const
	{
		return p.x<=mid.x+length/2.0 && p.x>=mid.x-length/2.0 &&
			   p.y<=mid.y+length/2.0 && p.y>=mid.y-length/2.0 &&
			   p.z<=mid.z+length/2.0 && p.z>=mid.z-length/2.0;
	}

	Octant mUNW() const
	{
		double newL = length/4.0;
		return Octant(mid.x-newL, mid.y+newL, mid.z+newL, length/2.0);
	}

	Octant mUNE() const
	{
		double newL = length/4.0;
		return Octant(mid.x+newL, mid.y+newL, mid.z+newL, length/2.0);
	}

	Octant mUSW() const
	{
		double newL = length/4.0;
		return Octant(mid.x-newL, mid.y-newL, mid.z+newL, length/2.0);
	}

	Octant mUSE() const
	{
		double newL = length/4.0;
		return Octant(mid.x+newL, mid.y-newL, mid.z+newL, length/2.0);
	}

	Octant mDNW() const
	{
		double newL = length/4.0;
		return Octant(mid.x-newL, mid.y+newL, mid.z-newL, length/2.0);
	}

	Octant mDNE() const
	{
		double newL = length/4.0;
		return Octant(mid.x+newL, mid.y+newL, mid.z-newL, length/2.0);
	}

	Octant mDSW() const
	{
		double newL = length/4.0;
		return Octant(mid.x-newL, mid.y-newL, mid.z-newL, length/2.0);
	}

	Octant mDSE() const
	{
		double newL = length/4.0;
		return Octant(mid.x+newL, mid.y-newL, mid.z-newL, length/2.0);
	}
};

