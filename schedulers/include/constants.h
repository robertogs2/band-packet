#ifndef CONSTANTS 
#define CONSTANTS 

const int HUNDRED_PERCENT = 100;
const int ZERO = 0;

const short RADIOACTIVE = 2;	// Send in real time
const short URGENT = 1;		    // Send asap
const short NORMAL = 0;		    // Send when possible

const short LEFT_SIDE = 0;
const short RIGHT_SIDE = 1;

const short TOP_BAND = 0;
const short MID_BAND = 1;
const short BOTTOM_BAND = 2;

const int CONST_DISTRO = 0;
const int GAUSSIAN_DISTRO = 1;
const int GAMMA_DISTRO = 2;

const char* TOP_BAND_CONF_PATH = "../config/top_band.conf";
const char* MID_BAND_CONF_PATH = "../config/mid_band.conf";
const char* BOTTOM_BAND_CONF_PATH = "../config/bottom_band.conf";
const char* AUX_FILE_PATH = "../config/config.aux";

#endif // CONSTANTS
