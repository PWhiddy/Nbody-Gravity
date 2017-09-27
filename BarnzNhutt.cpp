//============================================================================
// Name        : BarnzNhutt.cpp
// Author      : Peter Whidden
//============================================================================

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <random>
#include <omp.h>
#include "Bhtree.cpp"

void initializeBodies(struct body* bods);
void runSimulation(struct body* b, char* image, double* hdImage);
void interactBodies(struct body* b);
void singleInteraction(struct body* a, struct body* b);
double magnitude(vec3 v);
void updateBodies(struct body* b);
void createFrame(char* image, double* hdImage, struct body* b, int step);
double toPixelSpace(double p, int size);
void renderClear(char* image, double* hdImage);
void renderBodies(struct body* b, double* hdImage);
void colorDot(double x, double y, double vMag, double* hdImage);
void colorAt(int x, int y, struct color c, double f, double* hdImage);
unsigned char colorDepth(unsigned char x, unsigned char p, double f);
double clamp(double x);
void writeRender(char* data, double* hdImage, int step);

int main()
{
	std::cout << SYSTEM_THICKNESS << "AU thick disk\n";;
	char *image = new char[WIDTH*HEIGHT*3];
	double *hdImage = new double[WIDTH*HEIGHT*3];
	struct body *bodies = new struct body[NUM_BODIES];

	initializeBodies(bodies);
	runSimulation(bodies, image, hdImage);
	std::cout << "\nwe made it\n";
	delete[] bodies;
	delete[] image;
	return 0;
}

void initializeBodies(struct body* bods)
{
	using std::uniform_real_distribution;
	uniform_real_distribution<double> randAngle (0.0, 200.0*PI);
	uniform_real_distribution<double> randRadius (INNER_BOUND, SYSTEM_SIZE);
	uniform_real_distribution<double> randHeight (0.0, SYSTEM_THICKNESS);
	std::default_random_engine gen (0);
	double angle;
	double radius;
	double velocity;
	struct body *current;

	//STARS
	velocity = 0.67*sqrt((G*SOLAR_MASS)/(4*BINARY_SEPARATION*TO_METERS));
	//STAR 1
	current = &bods[0];
	current->position.x = 0.0;///-BINARY_SEPARATION;
	current->position.y = 0.0;
	current->position.z = 0.0;
	current->velocity.x = 0.0;
	current->velocity.y = 0.0;//velocity;
	current->velocity.z = 0.0;
	current->mass = SOLAR_MASS;
	//STAR 2
	/*
	current = bods + 1;
	current->position.x = BINARY_SEPARATION;
	current->position.y = 0.0;
	current->position.z = 0.0;
	current->velocity.x = 0.0;
	current->velocity.y = -velocity;
	current->velocity.z = 0.0;
	current->mass = SOLAR_MASS;
	*/

	    ///STARTS AT NUMBER OF STARS///
	double totalExtraMass = 0.0;
	for (int index=1; index<NUM_BODIES; index++)
	{
		angle = randAngle(gen);
		radius = sqrt(SYSTEM_SIZE)*sqrt(randRadius(gen));
		velocity = pow(((G*(SOLAR_MASS+((radius-INNER_BOUND)/SYSTEM_SIZE)*EXTRA_MASS*SOLAR_MASS))
					  	  	  	  	  / (radius*TO_METERS)), 0.5);
		current = &bods[index];
		current->position.x =  radius*cos(angle);
		current->position.y =  radius*sin(angle);
		current->position.z =  randHeight(gen)-SYSTEM_THICKNESS/2;
		current->velocity.x =  velocity*sin(angle);
		current->velocity.y = -velocity*cos(angle);
		current->velocity.z =  0.0;
		current->mass = (EXTRA_MASS*SOLAR_MASS)/NUM_BODIES;
		totalExtraMass += (EXTRA_MASS*SOLAR_MASS)/NUM_BODIES;
	}
	std::cout << "\nTotal Disk Mass: " << totalExtraMass;
	std::cout << "\nEach Particle weight: " << (EXTRA_MASS*SOLAR_MASS)/NUM_BODIES
			  << "\n______________________________\n";
}

void runSimulation(struct body* b, char* image, double* hdImage)
{
	createFrame(image, hdImage, b, 1);
	for (int step=1; step<STEP_COUNT; step++)
	{
		std::cout << "\nBeginning timestep: " << step;
		interactBodies(b);

		if (step%RENDER_INTERVAL==0)
		{
			createFrame(image, hdImage, b, step + 1);
		}
		if (DEBUG_INFO) {std::cout << "\n-------Done------- timestep: "
			       << step << "\n" << std::flush;}
	}
}

void interactBodies(struct body* bods)
{
	// Sun interacts individually
	if (DEBUG_INFO) {std::cout << "\nCalculating Force from star..." << std::flush;}
	struct body *sun = &bods[0];
	for (int bIndex=1; bIndex<NUM_BODIES; bIndex++)
	{
		singleInteraction(sun, &bods[bIndex]);
	}

	if (DEBUG_INFO) {std::cout << "\nBuilding Octree..." << std::flush;}

	// Build tree
	vec3 *center = new struct vec3;
	center->x = 0;
	center->y = 0;
	center->z = 0.1374; /// Does this help?
	Octant *root = new Octant(center, 60*SYSTEM_SIZE);
	Bhtree *tree = new Bhtree(root);

	for (int bIndex=1; bIndex<NUM_BODIES; bIndex++)
	{
		if (root->contains(bods[bIndex].position))
		{
			tree->insert(&bods[bIndex]);
		}
	}

	if (DEBUG_INFO) {std::cout << "\nCalculating particle interactions..." << std::flush;}

	// loop through interactions
	#pragma omp parallel for
	for (int bIndex=1; bIndex<NUM_BODIES; bIndex++)
	{
		if (root->contains(bods[bIndex].position))
		{
			tree->interactInTree(&bods[bIndex]);
		}
	}

	// Destroy tree
	delete tree;
	//
	if (DEBUG_INFO) {std::cout << "\nUpdating particle positions..." << std::flush;}
	updateBodies(bods);
}

void singleInteraction(struct body* a, struct body* b)
{
	vec3 posDiff;
	posDiff.x = (a->position.x-b->position.x)*TO_METERS;
	posDiff.y = (a->position.y-b->position.y)*TO_METERS;
	posDiff.z = (a->position.z-b->position.z)*TO_METERS;
	double dist = magnitude(posDiff);
	double F = TIME_STEP*(G*a->mass*b->mass) / ((dist*dist + SOFTENING*SOFTENING) * dist);

	a->accel.x -= F*posDiff.x/a->mass;
	a->accel.y -= F*posDiff.y/a->mass;
	a->accel.z -= F*posDiff.z/a->mass;
	b->accel.x += F*posDiff.x/b->mass;
	b->accel.y += F*posDiff.y/b->mass;
	b->accel.z += F*posDiff.z/b->mass;
}

double magnitude(vec3 v)
{
	return sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
}

void updateBodies(struct body* bods)
{
	double mAbove = 0.0;
	double mBelow = 0.0;
	for (int bIndex=0; bIndex<NUM_BODIES; bIndex++)
	{
		struct body *current = &bods[bIndex];
		if (DEBUG_INFO)
		{
			if (bIndex==0)
			{
				std::cout << "\nStar x accel: " << current->accel.x
						  << "  Star y accel: " << current->accel.y;
			} else if (current->position.y > 0.0)
			{
				mAbove += current->mass;
			} else {
				mBelow += current->mass;
			}
		}
		current->velocity.x += current->accel.x;
		current->velocity.y += current->accel.y;
		current->velocity.z += current->accel.z;
		current->accel.x = 0.0;
		current->accel.y = 0.0;
		current->accel.z = 0.0;
		current->position.x += TIME_STEP*current->velocity.x/TO_METERS;
		current->position.y += TIME_STEP*current->velocity.y/TO_METERS;
		current->position.z += TIME_STEP*current->velocity.z/TO_METERS;
	}
	if (DEBUG_INFO)
	{
		std::cout << "\nMass below: " << mBelow << " Mass Above: "
				  << mAbove << " \nRatio: " << mBelow/mAbove;
	}
}

void createFrame(char* image, double* hdImage, struct body* b, int step)
{
	std::cout << "\nWriting frame " << step;
	if (DEBUG_INFO)	{std::cout << "\nClearing Pixels..." << std::flush;}
	renderClear(image, hdImage);
	if (DEBUG_INFO) {std::cout << "\nRendering Particles..." << std::flush;}
	renderBodies(b, hdImage);
	if (DEBUG_INFO) {std::cout << "\nWriting frame to file..." << std::flush;}
	writeRender(image, hdImage, step);
}

void renderClear(char* image, double* hdImage)
{
	for (int i=0; i<WIDTH*HEIGHT*3; i++)
	{
	//	char* current = image + i;
		image[i] = 0; //char(image[i]/1.2);
		hdImage[i] = 0.0;
	}
}

void renderBodies(struct body* b, double* hdImage)
{
	/// ORTHOGONAL PROJECTION
	for(int index=0; index<NUM_BODIES; index++)
	{
		struct body *current = &b[index];

		int x = toPixelSpace(current->position.x, WIDTH);
		int y = toPixelSpace(current->position.y, HEIGHT);

		if (x>DOT_SIZE && x<WIDTH-DOT_SIZE &&
			y>DOT_SIZE && y<HEIGHT-DOT_SIZE)
		{
			double vMag = magnitude(current->velocity);
			colorDot(current->position.x, current->position.y, vMag, hdImage);
		}
	}
}

double toPixelSpace(double p, int size)
{
	return (size/2.0)*(1.0+p/(SYSTEM_SIZE*RENDER_SCALE));
}

void colorDot(double x, double y, double vMag, double* hdImage)
{
	const double velocityMax = MAX_VEL_COLOR; //35000
	const double velocityMin = sqrt(0.8*(G*(SOLAR_MASS+EXTRA_MASS*SOLAR_MASS))/
			(SYSTEM_SIZE*TO_METERS)); //MIN_VEL_COLOR;
	const double vPortion = sqrt((vMag-velocityMin) / velocityMax);
	color c;
	c.r = clamp(4*(vPortion-0.333));
	c.g = clamp(fmin(4*vPortion,4.0*(1.0-vPortion)));
	c.b = clamp(4*(0.5-vPortion));
	for (int i=-DOT_SIZE/2; i<DOT_SIZE/2; i++)
	{
		for (int j=-DOT_SIZE/2; j<DOT_SIZE/2; j++)
		{
			double xP = floor(toPixelSpace(x, WIDTH));
			double yP = floor(toPixelSpace(y, HEIGHT));
			double cFactor = PARTICLE_BRIGHTNESS /
					(pow(exp(pow(PARTICLE_SHARPNESS*
					(xP+i-toPixelSpace(x, WIDTH)),2.0))
				       + exp(pow(PARTICLE_SHARPNESS*
					(yP+j-toPixelSpace(y, HEIGHT)),2.0)),/*1.25*/0.75)+1.0);
			colorAt(int(xP+i),int(yP+j),c, cFactor, hdImage);
		}
	}

}

void colorAt(int x, int y, struct color c, double f, double* hdImage)
{
	int pix = 3*(x+WIDTH*y);
	hdImage[pix+0] += c.r*f;//colorDepth(c.r, image[pix+0], f);
	hdImage[pix+1] += c.g*f;//colorDepth(c.g, image[pix+1], f);
	hdImage[pix+2] += c.b*f;//colorDepth(c.b, image[pix+2], f);
}

unsigned char colorDepth(unsigned char x, unsigned char p, double f)
{
	return fmax(fmin((x*f+p),255),0);
//	unsigned char t = fmax(fmin((x*f+p),255),0);
//	return 2*t-(t*t)/255;
}

double clamp(double x)
{
	return fmax(fmin(x,1.0),0.0);
}

void writeRender(char* data, double* hdImage, int step)
{
	
	for (int i=0; i<WIDTH*HEIGHT*3; i++)
	{
		data[i] = int(255.0*clamp(hdImage[i]));
	}

	int frame = step/RENDER_INTERVAL + 1;//RENDER_INTERVAL;
	std::string name = "images/Step"; 
	int i = 0;
	if (frame == 1000) i++; // Evil hack to avoid extra 0 at 1000
	for (i; i<4-floor(log(frame)/log(10)); i++)
	{
		name.append("0");
	}
	name.append(std::to_string(frame));
	name.append(".ppm");

	std::ofstream file (name, std::ofstream::binary);

	if (file.is_open())
	{
//		size = file.tellg();
		file << "P6\n" << WIDTH << " " << HEIGHT << "\n" << "255\n";
		file.write(data, WIDTH*HEIGHT*3);
		file.close();
	}

}

