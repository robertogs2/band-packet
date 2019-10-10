#ifndef CONSTANTS 
#define CONSTANTS 

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

const short RADIOACTIVE = 2;	// Send in real time
const short URGENT = 1;		    // Send asap
const short NORMAL = 0;		    // Send when possible

const short LEFT_SIDE = 0;
const short RIGHT_SIDE = 1;

const short W_BAND = 0;
const short SIGN_BAND = 1;
const short RANDOM_BAND = 2;

const int CONST_DISTRO = 0;
const int GAUSSIAN_DISTRO = 1;
const int GAMMA_DISTRO = 2;

#endif // CONSTANTS
