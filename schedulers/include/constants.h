#ifndef CONSTANTS 
#define CONSTANTS 

const int HUNDRED_PERCENT = 100;
const int ZERO = 0;

const short RADIOACTIVE = 0;	// Send in real time
const short URGENT = 1;		    // Send asap
const short NORMAL = 2;		    // Send when possible

const short LEFT_SIDE = 0;
const short RIGHT_SIDE = 1;

const short W_BAND = 0;
const short SIGN_BAND = 1;
const short RANDOM_BAND = 2;

const int CONST_DISTRO = 0;
const int GAUSSIAN_DISTRO = 1;
const int GAMMA_DISTRO = 2;

const char* WBAND_CONF_PATH = "../config/bandW.conf";
const char* SBAND_CONF_PATH = "../config/bandSign.conf";
const char* RBAND_CONF_PATH = "../config/bandRand.conf";
const char* AUX_FILE_PATH = "../config/config.aux";

#endif // CONSTANTS
