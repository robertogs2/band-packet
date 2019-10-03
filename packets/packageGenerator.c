#include <stdio.h> 
#include <stdlib.h> 
#include <gsl/gsl_math.h>
#include <math.h>
#include <time.h> 
#include "constants.h"

typedef struct package {
		int id;
		float weight;
		short side;		// 0: a, 1: b
		short priority; // 0: norm, 1: urge, 2: rads
		short status;	// 0: not ready, 1: ready to send, 2: in transit, 3: delivered
		short band;		
	} package_t;

/** Generate a random 0-100 number
 */
int roll100(){
	const int HUNDRED_PERCENT = 100;
	int lowRange = ZERO;	  
	int hiRange = HUNDRED_PERCENT;
	int percentRoll = (rand() % (hiRange - lowRange + 1)) + lowRange;
	return percentRoll;
}


// =============================================== DEFINE PACKAGE ATRIBUTES ===============================================

/** Define the weight of packages according to their priority, assuming radiactive
 * packages are very heavy, in this version this is arbitrary.
 */
float measureWeight (int priority){
	int maxNormalWeight = 9;	// could turn to cts if important
	int minNormalWeight = 1;
	if (priority==URGENT){
		return 2.0;
	} else if (priority==RADIOACTIVE) {
		return 15.0;
	} else {
		return (rand() % (maxNormalWeight - minNormalWeight + 1))+1.0; // could make all random or semi-random
	}
}

/** Choose on which side of the band the package starts based on the probability 
 * of the package being on the left
 */
short chooseSide (int pLeft){
	int sideRoll = (rand() % (100 + 1));
	if (sideRoll > pLeft) {
		return LEFT_SIDE;
	} else {
		return RIGHT_SIDE;
	}
}

/** Choose in which band will the package be sent based on the probability of the
 * probability it will be sent on the sign band and the random band
 * 
 */
short assignBand (int pSignBand, int pRandomBand){
	int bandSelect = roll100();

	if (bandSelect > HUNDRED_PERCENT-pSignBand) {
		return SIGN_BAND;
	} else if (bandSelect < pRandomBand) {
		return RANDOM_BAND;
	} else {
		return W_BAND;
	}
}


/** Returns the type of package base on a percentile roll
  * 
  * int pRads: percentage of Radioactive packages
  * int pPrime:  percentage of Priority packages 
  * 
  * return: the type of Package
  * 		0 : normal
  * 		1 : priority
  *   		2 : radioactive
  */
int packageType (int pRads, int pPrime) {
	
	int packageType = roll100()+1;
	
	if (packageType < pRads) {
		return RADIOACTIVE;
	} else if (packageType > HUNDRED_PERCENT-pPrime) {
		return URGENT;
	}
	else {
		return NORMAL;
	}
}

// Test function
void testPackageType () {
  int pRads = 20;
  int pPrime = 20;
  int totalPkg = 1000;
  int is, rads, prio, norm;
  is = rads = prio = norm = 0;
  
  srand(time(0));  
  
  for (;totalPkg>0;--totalPkg){
	  is = packageType(pRads, pPrime);
	  switch (is) {
		  case 1: prio++;
			break;
		  case 2: rads++;
			break;
		  default: norm++;
	  }
	}
	
	printf ("received: %d normal packages, %d priority packages and %d radioactive packages \n", norm, prio, rads);
}

/** Create a package 
 * 
 * packageCounter: auto-incremented package id/position on the array
 * newPackage: pointer to the package in the array we are defining
 * pRads: probability that the package is radioactive, read from .config
 * pUrge: probability that the package is urgent, read from .config
 */
void createPackage(int* packageCounter, package_t* newPackage, int pRads, int pUrge){

		int pPriority = packageType(pRads,pUrge);
		float pWeight = measureWeight(pPriority);
		short pSide = chooseSide(80);				// define probability of side somewhere else
		short pBand = assignBand(40, 30);			// define probability of band somewhere else
					
		newPackage->id = *packageCounter;
		newPackage->weight = pWeight;
		newPackage->side = pSide;
		newPackage->priority = pPriority;	
		newPackage->status = 1;
		newPackage->band = pBand;
		*packageCounter  = *packageCounter + 1;
			
		printf ("Created package %d \n", newPackage->id);
}

// Test function
void checkPackage(int i, package_t* testPackage){
	printf("Checking package %d \n", i);
	printf("  Package id: %d \n", testPackage->id);
	printf("  Package weight: %.2f \n", testPackage->weight);
	printf("  Package side: %d \n", testPackage->side);
	printf("  Package piority: %d \n", testPackage->priority);
	printf("  Package status: %d \n", testPackage->status);
	printf("  Package in band: %d \n", testPackage->band);
}


// =============================================== PROBABILITY DISTRIBUTIONS ===============================================

/** Calculate the probability distribution function (pdf) based on which cycle
 * the program is running. This generates packages following the 
 * probability distribution. *** Might be not what the teacher asked!
 */
double gaussDistro(int cycle, double stdDev, double mean) {
	int x = cycle;
	const double pi = M_PI;
	double var = stdDev*stdDev;

	double nD = (1/sqrt(2*pi*var))*exp(-(x*x-2*x*mean+mean*mean)/(2*var))*mean*4;
	return nD;
}

double expoDistro (int cycle, int allCycles){
	int x = cycle;
	const double lambda = 0.01;
	
	double eD = lambda * exp(-lambda*x) * 20*allCycles;
	return eD;
}
	
int constDistro (int lowRange, int hiRange) {
	int cD = (rand() % (hiRange - lowRange + 1)) + lowRange;
	return cD;
}

// =============================================== PROBABILITY DISTRIBUTIONS ===============================================

/**Creates a random number of packages, based on the proababilty distri_
 * bution selected
 * 
 * int lowRange: Minimum number of packages that can appear  
 * int hiRange: Maximum number of packages that can appear 
 * int distro: type of distribution
 * 		0 : Constant
 * 		1 : Normal
 * 		2 : Exponential
 * 		3 : TBD
 * 
 * return: the number of packages arriving this cycle
 */
double randNum (int cycle, int allCycles, int distro) {
	double numPackages = 0;
	
	if (distro == GAUSSIAN_DISTRO){
		numPackages += constDistro(5, 0);
		numPackages += gaussDistro(cycle, allCycles*0.15, allCycles*0.5);
		return numPackages;
	} else if (distro == EXPO_DISTRO){
		numPackages += constDistro(5,0);
		numPackages += expoDistro (cycle, allCycles);
		return numPackages;
	} else { // constant distro
		numPackages += constDistro (0, 20);
		return numPackages;
	}
}

/** Create a testable Octave file that graphics the cumulative probabi_
 * lity distribution.
 * 
 * int cycles: amount of numbers to be tested
 * int lowRange: Minimum number of packages that can appear  
 * int hiRange: Maximum number of packages that can appear 
 * int distro: type of distribution
 * 		0 : Constant
 * 		1 : Normal
 * 		2 : Exponential
 * 		3 : TBD
 * 
 * return: a copy-paste string
 */
void testDistribution (int allCycles, int distro){
	int newPackages = 0;
	int cycle = 0;
	
	printf ("x = 0:1:%d; \n", allCycles);
	printf("y = [0, ");
	for (cycle=allCycles; cycle>1; --cycle) {
		newPackages = randNum(cycle, allCycles, distro); // += for accum
		printf ("%d, ", newPackages);
	}
	newPackages = randNum(cycle, allCycles, distro); // += for accum
	printf("%d]; \n", newPackages);
	
	printf("plot(x,y) \n");

}

int main(int argc, char **argv)
{
			// Global use
	int packageCounter = 0;
	int cycle = 0;
	int allCycles = 100; // number of time packages will appear, defines median and std dev
	int distribution = 1;
	package_t allPackages [50];
	int pRadioactive = 20;    //percentage of Radioactive packages
	int pUrgente = 20;		//percentage of Priority packages
	
	srand(time(0));
	//testDistribution(1000, 2);

	int newPackages = randNum(cycle, allCycles, distribution);
	for (int i=newPackages; i>0; --i){
		createPackage(&packageCounter, &allPackages[packageCounter], pRadioactive, pUrgente);
		cycle++;
	}
	
	printf("package count is at %d \n", packageCounter);
	
	for (int i = 0;  i<newPackages; i++){ checkPackage(i, &allPackages[i]); }	//DEBUG
	
	return 0;
}





