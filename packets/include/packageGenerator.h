#ifndef PACKAGE_GENERATOR
#define PACKAGE_GENERATOR

#include <math.h>
#include <time.h> 

// creates a pseudo-random number that follows a distribution
int randNum (int mean, int stdDev, int distro);

// instantiates a package and increments the id counter
void createPackage(int* packageCounter, package_t* newPackage, int pRads, int pUrge, int pLeft, int pSignBand, int pRandomBand);

// prints the values of the package struct
void checkPackage(package_t* testPackage);

#endif // PACKAGE_GENERATOR
