#ifndef PACKAGE_GENERATOR
#define PACKAGE_GENERATOR



// reads band info from a conf file
config_t get_config(int bandId);

// creates a pseudo-random number that follows a distribution
int randNum (int mean, int stdDev, int distro);

// instantiates a package and increments the id counter
void createPackage(int* packageCounter, package_t* newPackage, int bandId);

// prints the values of the package struct
void checkPackage(package_t* testPackage);

#endif // PACKAGE_GENERATOR
