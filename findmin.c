/*
   CMSC 483 Project

   Nathan Broemm: nbroemm1@umbc.edu
   Chris Powell: cpowel2@umbc.edu

   Project link: http://www.csee.umbc.edu/~squire/cs455_proj.shtml
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#define NUM_THREADS 4   //# of threads
#define MAX_RECURSE 12  //# of recursive calls to localMinimum()

int rCount = 0;  //recurse counter
double x = 0;
double y = 0;
double z = 0;
double z_minimum = 100.0;
double y_minimum = 0;
double x_minimum = 0;
double x_local = 0;
double y_local = 0;
double z_local = 0;
double increment = 1.0E-3;  //step size

//Struct used for pthread implementation - work in progress
typedef struct {
	int proc_id;
	double x;
	double y;
	double z;
	double increment;
} thread_data;

void *runner(void *param);
void globalMinimum();
double localMinimum(double x, double y, double z, double increment);
double function(double x, double y);

int main(int argc, char **argv){

	//Pthread code - Work in progress

	int i = 0;
	int j = 0;
	pthread_t threads[NUM_THREADS];
	pthread_attr_t pthread_custom_attr;
	thread_data *my_data;

	pthread_attr_init(&pthread_custom_attr);
	my_data = (thread_data *)malloc(sizeof(thread_data)*NUM_THREADS);

	for(i = 0; i < NUM_THREADS; i++) {
		my_data[i].proc_id = i;
		my_data[i].x = x;
		my_data[i].y = y;
		pthread_create(&threads[i], &pthread_custom_attr, runner, (void *) (my_data+i));
	}

	for(i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	free(my_data);


	//Compute global minimum
	globalMinimum();
	//exit(1);	
	//Compute local minimum
	z = localMinimum(x_minimum, y_minimum, z_minimum, increment);

	return 0;
}

//runner function for pthread implementation - work in progress
void *runner(void *param) {

	// cast our argument
	thread_data *my_data = (thread_data *)param;

	// compute the local 
	localMinimum(my_data->x, my_data->y, my_data->z, my_data->increment);
}

//get a general global minimum
void globalMinimum() {
	x = -2.00;
	while(x >= -2.0 && x <= 2.0) {
		y = -2.00;
		while(y >= -2.0 && y <= 2.0) {
			//fprintf(stdout,"x = %f  y = %f\n", x, y);
			//fflush(stdout);
			//call our function
			z = function(x, y);
			if(z <= z_minimum) {
				z_minimum = z;
				x_minimum = x;
				y_minimum = y;
			}
			y += increment;
		}
		x += increment;
	}
	printf(
			"Global Minimum:\n"
			"z = %2.20f\n"
			"x =  %2.20f\n"
			"y = %2.20f\n\n", z_minimum, x_minimum, y_minimum);
}


//Uses a 9 point stencil to search solution space
double localMinimum(double x, double y, double z, double increment){
	double i = 0;
	double ii = 0;
	double j = 0;
	double x_local = 0;
	double y_local = 0;
	double z_minimum = z;
	double x_minimum = 0;
	double y_minimum = 0;

	//3x3 grid - check 8 surrounding points
	for(i = increment; i >= increment / MAX_RECURSE; i = i / 2) {
		
		//check y points up and down
		x_local = x;
		y_local = (y - i);
		z = function(x_local, y_local);
		if(z <= z_minimum) {
			z_minimum = z;
			x_minimum = x_local;
			y_minimum = y_local;
			ii = i;
		}

		x_local = x;
		y_local = (y + i);
		z = function(x_local, y_local);
		if(z <= z_minimum) {
			z_minimum = z;
			x_minimum = x_local;
			y_minimum = y_local;
			ii = i;
		}

		//check x points left then right
		y_local = y;
		x_local = (x - i);
		z = function(x_local, y_local);
		if(z <= z_minimum) {
			z_minimum = z;
			x_minimum = x_local;
			y_minimum = y_local;
			ii = i;
		}

		y_local = y;
		x_local = (x + i);
		z = function(x_local, y_local);
		if(z <= z_minimum) {
			z_minimum = z;
			x_minimum = x_local;
			y_minimum = y_local;
			ii = i;
		}

		//check 4 diagonal points
		x_local = x + i;
		y_local = y + i;
		z = function(x_local, y_local);
		if(z <= z_minimum) {
			z_minimum = z;
			x_minimum = x_local;
			y_minimum = y_local;
			ii = i;
		}

		x_local = x - i;
		y_local = y - i;
		z = function(x_local, y_local);
		if(z <= z_minimum) {
			z_minimum = z;
			x_minimum = x_local;
			y_minimum = y_local;
			ii = i;
		}

		x_local = x - i;
		y_local = y + i;
		z = function(x_local, y_local);
		if(z <= z_minimum) {
			z_minimum = z;
			x_minimum = x_local;
			y_minimum = y_local;
			ii = i;
		}

		x_local = x + i;
		y_local = y - i;
		z = function(x_local, y_local);
		if(z <= z_minimum) {
			z_minimum = z;
			x_minimum = x_local;
			y_minimum = y_local;
			ii = i;
		}       
	}
	rCount++;

	//Start recursion
	if(rCount <= MAX_RECURSE) {
		z = localMinimum(x_minimum, y_minimum, z_minimum, ii);
	}
	else{
		//Print Hardcoded solution
		printf("Result should be:\n"
				"z = -3.13833328825201318679\n"
				"x =  0.46865387240424877247\n"
				"y = -0.92291651641950001039\n\n");
		//Print what we got
		printf("Final Results:\n"
				"z = %2.20f\n"
				"x =  %2.20f\n"
				"y = %2.20f\n\n", z_minimum, x_minimum, y_minimum);
	}
	return z;
}

/*
   Find the minimum z of this function:  z=exp(sin(50x)) +sin(60exp(y)) +sin(70sin(x)) +sin(sin(80y)) -sin(10(x+y)) +(x^2+y^2)/4
 */
double function(double x, double y){
	double z;
	z = exp(sin(50.0 * x)) + sin(60.0 * exp(y)) + sin(80.0 * sin(x)) + sin(sin(70.0*y)) - sin(10.0 * (x + y)) + (x * x + y * y) /4.0;
	return z;
}
