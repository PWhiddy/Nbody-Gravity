/*
 * Constants.h
 *
 *  Created on: Feb 3, 2016
 *      Author: peterwhidden
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

//#define NULL 0

#define WIDTH	1024
#define HEIGHT	1024
#define NUM_BODIES (1024*32)
#define PI      3.14159265358979323846
#define TO_METERS 1.496e11 // Meters in an AU
#define SYSTEM_SIZE 3.5    // Farthest particles in AU
#define SYSTEM_THICKNESS 0.08  // 0.05  Thickness in AU
#define INNER_BOUND 0.3    // Closest particles in AU
#define SOFTENING (0.015*TO_METERS)
#define SOLAR_MASS 2.0e30  // in kg
#define BINARY_SEPARATION 0.07 // AU
#define EXTRA_MASS 1.5 //0.02 Portion of solar mass
#define ENABLE_FRICTION 0
#define FRICTION_FACTOR 25.0 //275.0
#define MAX_DISTANCE 0.75 //2.0  Minimum distance for approximation
#define G 6.67408e-11
#define RENDER_SCALE 2.5 //1.1
#define MAX_VEL_COLOR 40000.0  // Both in km/s
#define MIN_VEL_COLOR 14000.0
#define PARTICLE_BRIGHTNESS 0.52//0.03 for 256/512k, 0.4 for 16k
#define PARTICLE_SHARPNESS 1.0
#define DOT_SIZE 8 // 15  // Range of pixels to render
#define TIME_STEP (3*32*1024) //(1*128*1024)   Seconds
#define STEP_COUNT 16000
#define RENDER_INTERVAL 1 //1
#define DEBUG_INFO true

struct vec3
{
	double x, y, z;
};

struct body
{
	vec3 position, velocity, accel;
	double mass;
};

struct color
{
	double r, g, b;
};

#endif /* CONSTANTS_H_ */
