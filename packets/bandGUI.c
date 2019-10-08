#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// THIS IS NOT SUPPOSED TO BE THE PROGRAM'S GUI

/** Calculate how long will the package will be on the band
 * 
 * int pkgWeight: the weight of the package being sent [kg]
 * int bandLen: the length of the transport band [m]
 * int bandStr: the strength of the transport band [N]
 * 
 * return t: the time the package will be on the band [s]
 */
double timeOnBand (int pkgWeight, int bandLen, int bandStr){
	double mdf = pkgWeight*bandLen|bandStr;
	double t = sqrt(mdf);
	return t;
}

void printPackage (int pComplete, int bandLen){
	int sideA = 25-bandLen/2;
	int sideB = 2;
	printf("|");
    for (int i = sideA; i>0; --i) {
        printf(" ");
        sideB++;
    }
    
    double posPkg = (bandLen*(100-pComplete)/100);
    for (int i = bandLen; i>0; --i) {
		if (i == posPkg){
			printf("0"); //■
		} else {
			printf(" ");
		}
        sideB++;
    }
    for (int i = 49-sideB; i>0; --i) {
        printf(" ");
        
    }
    printf("|\n");
}

void printBand (int bandLen){
	int sideA = 25-bandLen/2;
	int sideB = 2;
	printf("|");
    for (int i = sideA; i>0; --i) {
        printf(" ");
        sideB++;
    }
    for (int i = bandLen; i>0; --i) {
        printf("X");
        sideB++;
    }
    for (int i = 49-sideB; i>0; --i) {
        printf(" ");
        
    }
    printf("|\n");
    printf("|                                               |\n");
}

int main(int argc, char **argv) {
    
    int bandWlen = 35;
    int bandSlen = 10;
    int bandRlen = 15;
    
    printf("|||||||||||||||||||||||||||||||||||||||||||||||||\n");
    printf("|             BAND SIMULATOR 2019               |\n");
    printf("|||||||||||||||||||||||||||||||||||||||||||||||||\n");
    printf("|                                               |\n");
    printPackage (20, bandWlen);
	printBand(bandWlen);
	printPackage (50, bandSlen);
	printBand(bandSlen);
	printPackage (90, bandRlen);
	printBand(bandRlen);
	printf("|||||||||||||||||||||||||||||||||||||||||||||||||\n");

	return 0;
}
