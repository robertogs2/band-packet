#ifndef CONSTANTS 
#define CONSTANTS 

typedef struct package {
		int id;
		float weight;
		short side;
		short priority;
		short progress;
		short band;		
	} package_t;

typedef struct config {
    int bandID;
    int bandStrength;
    int bandLength;
    int packageDistro;
    int packageRadsP;
    int packagePrioP;
    int packageLeftP;
    int bandParameter;
} config_t;

const int HUNDRED_PERCENT = 100;
const int ZERO = 0;

const int RADIOACTIVE = 2;	// Send in real time
const int URGENT = 1;		// Send asap
const int NORMAL = 0;		// Send when possible

const int LEFT_SIDE = 0;
const int RIGHT_SIDE = 1;

const int W_BAND = 0;
const int SIGN_BAND = 1;
const int RANDOM_BAND = 2;

const int CONST_DISTRO = 0;
const int GAUSSIAN_DISTRO = 1;
const int GAMMA_DISTRO = 2;

#endif
