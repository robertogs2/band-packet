#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include <time.h> 
#include "constants.h"

typedef struct package {
		int id;
		float weight;
		short side;
		short priority;
		short progress;
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
	
	int packageType = roll100();
	
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
		newPackage->progress = 1;
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
	printf("  Package progress: %d \n", testPackage->progress);
	printf("  Package in band: %d \n", testPackage->band);
}


// =============================================== PROBABILITY DISTRIBUTIONS ===============================================

// Creates a random number within the standard deviation of the function
int randomStdDev(int stdDev){
	int lowRange = -1*stdDev;
	int hiRange = stdDev;
	int meanModifier = (rand() % (hiRange - lowRange + 1)) + lowRange;
	return meanModifier;
}

/** Create a semi-random number that follows the Gaussian distribution
 * 
 * int mean: mean of the distribution (68% of numbers will be within stdDev of mean)
 * int stdDev: standard deviation of the function
 * 
 * returns integer number between 0 and mean*4*stdDev
 */
int gaussDistro(int mean, int stdDev) {
	int percentile = roll100();
	int pkgQuantity = 0;
	if (mean<stdDev*4){
		printf ("Warning: stdDev as been reduced to avoid negative packages \n");
		stdDev = mean/4;
	}
	int modifier = randomStdDev(stdDev);
	if (percentile<68) {
		pkgQuantity = mean + modifier; 
		return pkgQuantity;
	} else if (percentile<95){
		pkgQuantity = mean + 2*modifier;
		return pkgQuantity;
	} else if (percentile<99){
		pkgQuantity = mean + 3*modifier;
		return pkgQuantity;
	} else {
		pkgQuantity = mean + 4*modifier;
		return pkgQuantity;
	}
}

/** Create a semi-random number that follows the Gamma distribution
 * 
 * int mean: mean of the distribution (60% of numbers will be under the mean)
 * int stdDev: standard deviation of the function
 * 
 * returns integer number between 0 and mean*4*stdDev
 */
int gammaDistro (int mean, int stdDev){
	int percentile = roll100();
	int pkgQuantity = 0;
	if (mean<stdDev*4){
		printf ("Warning: stdDev as been reduced to avoid negative packages \n");
		stdDev = mean/4;
	}
	int modifier = randomStdDev(stdDev);
	if (percentile<60) {
		pkgQuantity = mean - stdDev + modifier; 
		return pkgQuantity;
	} else if (percentile<80){
		pkgQuantity = mean + modifier;
		return pkgQuantity;
	} else if (percentile<95){
		pkgQuantity = mean + stdDev + modifier;
		return pkgQuantity;
	} else {
		pkgQuantity = mean + 2*stdDev + 2*modifier;
		return pkgQuantity;
	}
}
	
int constDistro (int lowRange, int hiRange) {
	int cD = (rand() % (hiRange - lowRange + 1)) + lowRange;
	return cD;
}

// =============================================== PROBABILITY DISTRIBUTIONS ===============================================

/**Creates a random number of packages, based on the proababilty distribution selected
 * 
 * int mean: The mean that the distribution will have
 * int stdDev: max 1/4 of the mean, modifies the number of packages created
 * int distro: type of distribution
 * 		0 : Constant
 * 		1 : Gaussian
 * 		2 : Gamma
 * 		3 : TBD
 * 
 * return: the number of packages arriving this cycle
 */
int randNum (int mean, int stdDev, int distro) {
	int numPackages = 0;
	
	if (distro == GAUSSIAN_DISTRO){
		numPackages += gaussDistro(mean, stdDev);
		return numPackages;
	} else if (distro == GAMMA_DISTRO){
		numPackages += gammaDistro (mean, stdDev);
		return numPackages;
	} else { // constant distro
		numPackages += constDistro (1, 20);
		return numPackages;
	}
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

	int newPackages = randNum(8, 2, 1);
	for (int i=newPackages; i>0; --i){
		createPackage(&packageCounter, &allPackages[packageCounter], pRadioactive, pUrgente);
		cycle++;
	}
	
	printf("package count is at %d \n", packageCounter);
	
	for (int i = 0;  i<newPackages; i++){ checkPackage(i, &allPackages[i]); }	//DEBUG
	
	return 0;
}





