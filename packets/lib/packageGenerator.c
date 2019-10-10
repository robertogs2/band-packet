#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include <time.h> 
#include "../include/constants.h"
#include "../include/linked_list.h"
#include "../include/packageGenerator.h"

/** Generate a random 0-100 number
 */
int roll100(){
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
short packageType (int pRads, int pPrime) {
	
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

/** Calculate how long will the package will be on the band
 * 
 * int pkgWeight: the weight of the package being sent [kg]
 * int bandLen: the length of the transport band [m]
 * int bandStr: the strength of the transport band [N]
 * 
 * return t: the time the package will be on the band [s]
 */
short timeOnBand (int pkgWeight, int bandLen, int bandStr){
	double mdf = pkgWeight*bandLen/bandStr;
	short t = (short) 0.5*mdf;//* sqrt(mdf); problems linking math.h in the makefile
	return t;
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

// =============================================== GENERAL PACKAGE CREATION ===============================================

/**Creates a random number of packages, based on the proababilty distribution selected
 * 
 * int mean: The mean that the distribution will have
 * int stdDev: max 1/4 of the mean, modifies the number of packages created
 * int distro: type of distribution
 * 		0 : Constant (ignores mean and stdDev, creates between 1 and 20)
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


/** Create a package 
 * 
 * packageCounter: auto-incremented package id
 * newPackage: pointer to the package we are instantiating
 * pRads: probability % that the package is radioactive, read from .config
 * pUrge: probability % that the package is urgent, read from .config
 * pLeft: probability % that the package starts on "the left side"
 * pSignBand: probabiliy % that the package will be assigned to the sign controlled band
 * pRandomBand: probability % that the package will be assigned to the randomly controlled band
 */
void createPackage(int* packageCounter, package_t* newPackage, int pRads, int pUrge, int pLeft, int pSignBand, int pRandomBand){

		short pkgPriority = packageType(pRads,pUrge);
		float pkgWeight = measureWeight(pkgPriority);
		short pkgSide = chooseSide(pLeft);
		short pkgBand = assignBand(pSignBand, pRandomBand);
		short exeTime = timeOnBand(pkgWeight, 10, 100);//getBandLen(pkgBand), getBandStr(pkgBand));
					
		newPackage->id = *packageCounter;
		newPackage->weight = pkgWeight;
		newPackage->side = pkgSide;
		newPackage->priority = pkgPriority;	
		newPackage->progress = 1;
		newPackage->band = pkgBand;
		newPackage->on_band = false;
		newPackage->execution_time = exeTime;
		newPackage->remaining_time = exeTime;
		newPackage->usage_time_start = -1;
		*packageCounter  = *packageCounter + 1;
			
		printf ("Created package %d \n", newPackage->id);
}

/** Prints a package contents
 * 
 * package_t testPackage: the package to be printed out
 */
void checkPackage(package_t* testPackage){
	printf("Checking package id: %d \n", testPackage->id);
	printf("  Package weight: %.2f \n", testPackage->weight);
	printf("  Package side: %d \n", testPackage->side);
	printf("  Package piority: %d \n", testPackage->priority);
	printf("  Package progress: %d \n", testPackage->progress);
	printf("  Package in band: %d \n", testPackage->band);
	printf("  Package execution time: %d \n", testPackage->execution_time);
	printf("  Package has: %d s left on the band \n", testPackage->remaining_time);
	printf("  Package started on: %d \n", testPackage->usage_time_start);
}

