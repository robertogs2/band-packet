#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include <time.h> 
#include <string.h> 
#include "../include/constants.h"
#include "../include/linked_list.h"
#include "../include/package_generator.h"


/** Read the band varibles from a .conf file
 * 
 * const char* conf_path: the path to the conf file
 * returns: a config_t struct with the information in the file
 */
config_t get_config_aux(const char* conf_path){
    config_t conf;
    FILE* file = fopen(conf_path, "r");
    if(file==NULL){
        printf ("Path to config not avaliable. \n");
        exit(EXIT_FAILURE); 
    }
    char line[256];
    char prev[256];
    while (fgets(line, sizeof(line), file)) {
        char* current = strtok (line, "=:");
        //prev={0};
        while (current) {
            if(!strcmp(prev,"bandID")){
                conf.bandID = atoi(current);
            }
            else if(!strcmp(prev,"bandStrength")){
                conf.bandStrength = atoi(current);
            }
            else if(!strcmp(prev,"bandLength")){
                conf.bandLength = atoi(current);
            }            
			else if(!strcmp(prev,"bandDistro")){
                conf.bandDistro = atoi(current);
            }
			else if(!strcmp(prev,"bandMean")){
                conf.bandMean = atoi(current);
            }
			else if(!strcmp(prev,"bandStdDev")){
                conf.bandStdDev = atoi(current);
            }
			else if(!strcmp(prev,"packageRadsP")){
                conf.packageRadsP = atoi(current);
            }
			else if(!strcmp(prev,"packageUrgeP")){
                conf.packageUrgeP = atoi(current);
            }
			else if(!strcmp(prev,"packageLeftP")){
                conf.packageLeftP = atoi(current);
            }
			else if(!strcmp(prev,"bandQuantum")){
                conf.bandQuantum = atoi(current);
            }
			else if(!strcmp(prev,"bandParameter")){
                conf.bandParameter = atoi(current);
            }
			else if(!strcmp(prev,"bandAlgorithm")){
                conf.bandAlgorithm = atoi(current);
            }
			else if(!strcmp(prev,"bandScheduler")){
                conf.bandScheduler = atoi(current);
            }
            strcpy(prev, current);
            current = strtok (NULL, "=:");
        }
    }
    fclose(file);
    return conf;
} 

/** Read the band varibles from a specific .conf file
 * 
 * int bandId: which file to respective band
 * returns: a config_t struct with the information in the file
 */
config_t get_config (int bandId) {
	if (bandId == TOP_BAND){
		return get_config_aux(TOP_BAND_CONF_PATH);
	} else if (bandId == MID_BAND){
		return get_config_aux(MID_BAND_CONF_PATH);
	} else if (bandId == BOTTOM_BAND){
		return get_config_aux(BOTTOM_BAND_CONF_PATH);
	} else {
		exit(EXIT_FAILURE);
	}
}

/** Generate a random 0-100 number
 */
int roll100(){
	int lowRange = ZERO;	  
	int hiRange = HUNDRED_PERCENT;
	int percentRoll = rand() % 101;
	return percentRoll;
}

// =============================================== DEFINE PACKAGE ATRIBUTES ===============================================

/** Define the weight of packages according to their priority, assuming radiactive
 * packages are consistently packed, ensures real time scheduling
 */
float measureWeight (int priority, int scheduler, int qtm, int str, int len){
	int laxity = 1;
	if (priority==URGENT){
		return ((rand() % 20)+10.0)/10;
	} else if (priority==RADIOACTIVE) {
		if (scheduler == 4) {
			return (str*(qtm-laxity)*(qtm-laxity))/(len+(roll100()*2));
		} else {
			return ((rand() % 20)+40.0)/10;
		}
	} else {
		return ((rand() % 90)+10.0)/10; // could make all random or semi-random
	}
}

/** Choose on which side of the band the package starts based on the probability 
 * of the package being on the left
 */
short chooseSide (int pLeft){

	int sideRoll = roll100();
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
/*
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
*/

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
double timeOnBand (float pkgWeight, int bandLen, int bandStr){
	double mdf = pkgWeight*bandLen/bandStr;
	double t = sqrt(mdf);
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
void createPackage(int* packageCounter, package_t* newPackage, int bandId){

	config_t conf = get_config(bandId);
	short pkgPriority = packageType(conf.packageRadsP,conf.packageUrgeP);
	float pkgWeight = measureWeight(pkgPriority, conf.bandScheduler, conf.bandQuantum, conf.bandStrength, conf.bandLength);
	short pkgSide = chooseSide(conf.packageLeftP);
	double exeTime = timeOnBand(pkgWeight, conf.bandLength, conf.bandStrength);
				
	newPackage->id = *packageCounter;
	newPackage->weight = pkgWeight;
	newPackage->side = pkgSide;
	newPackage->priority = pkgPriority;	
	newPackage->progress = 0;
	newPackage->speed = conf.bandLength/exeTime;
	newPackage->band = bandId;
	newPackage->on_band = false;
	newPackage->accum_execution_time = 0.0;
	newPackage->current_execution_time = 0.0;
	newPackage->total_execution_time = exeTime;
	newPackage->remaining_time = exeTime;
	*packageCounter  = *packageCounter + 1;
		
	//printf ("Created package %d \n", newPackage->id);
}

/** Prints a package contents
 * 
 * package_t testPackage: the package to be printed out
 */
void checkPackage(package_t* testPackage){
	printf("Checking package id: %d \n", testPackage->id);
	printf("  Package weight: %.2f kg\n", testPackage->weight);
	printf("  Package side: %d \n", testPackage->side);
	printf("  Package piority: %d \n", testPackage->priority);
	printf("  Package progress: %d percent \n", testPackage->progress);
	printf("  Package moves at: %.2f m/s", testPackage->speed);
	printf(" in band: %d \n", testPackage->band);
	printf("  Package has been on the band for %.2f s\n", testPackage->accum_execution_time);
	printf("  Package execution time: %.2f \n", testPackage->total_execution_time);
	printf("  Package has: %f s left on the band \n", testPackage->remaining_time);
}

// Debug config file reading
void checkConfig(config_t file){
	printf("Reading configuration file \n");
	printf("  bandID: %d \n", file.bandID);
	printf("  bandLength: %d m \n", file.bandLength);
	printf("  bandDistro: %d \n", file.bandDistro);
	printf("  bandMean: %d packages \n", file.bandMean);
	printf("  bandStdDev: %d packages \n", file.bandStdDev);
	printf("  packageRadsP: %d percent \n", file.packageRadsP);
	printf("  packageUrgeP: %d percent \n", file.packageUrgeP);
	printf("  packageLeftP: %d percent \n", file.packageLeftP);
	printf("  bandQuantum: %d \n", file.bandQuantum);
	printf("  bandParameter: %d \n", file.bandParameter);
	printf("  bandAlgorithm: %d \n", file.bandAlgorithm);
	printf("  bandScheduler: %d \n", file.bandScheduler);
}
