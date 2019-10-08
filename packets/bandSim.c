#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "constants.h"

// test Pkg
typedef struct package {
	int id;
	int progress;
	int weight;
} package_t;

/** Calculate how long will the package will be on the band
 * 
 * int pkgWeight: the weight of the package being sent [kg]
 * int bandLen: the length of the transport band [m]
 * int bandStr: the strength of the transport band [N]
 * 
 * return t: the time the package will be on the band [s]
 */
double timeOnBand (int pkgWeight, int bandLen, int bandStr){
	double mdf = pkgWeight*bandLen/bandStr;
	double t = sqrt(mdf);
	return t;
}

int randWeight (int maxWeight) {
	int randNum = (rand() % maxWeight) + 1;
	return randNum;	
}

void testTOB () {
	// create packages
	int wBand_str = 10;
	int wBand_len = 10;
	package_t normal1 = {1, 0, 10};
	
	// calculate Time on band
	for (int i = 0; i<35; i+=10){
		normal1.weight += i;
		double t1 = timeOnBand (normal1.weight, wBand_len, wBand_str);
		printf ("With weigth %d the package will take %f seconds on the band \n", normal1.weight, t1);
	}
	
	for (int j = 0; j<35; j+=10){
		wBand_len += j;
		double t1 = timeOnBand (normal1.weight, wBand_len, wBand_str);
		printf ("With band length %d the package will take %f seconds on the band \n", wBand_len, t1);
	}


	for (int k = 0; k<35; k+=10){
		wBand_str += k;
		double t1 = timeOnBand (normal1.weight, wBand_len, wBand_str);
		printf ("With band strength %d the package will take %f seconds on the band \n", wBand_str, t1);
	}
	
}

int main(int argc, char **argv) {
	srand(time(0));
	const int maxWeight = 100;
	
	// define bands
	int wBand_str = 10;
	int sBand_str = 20;
	int rBand_str = 5;
	
	int wBand_len = 20;
	int sBand_len = 20;
	int rBand_len = 20;
	
	// define packages
	int nPkg = 10;
	package_t allPackages[nPkg];
	
	package_t creator = {-1, -1, -1};
	
	for (int i=nPkg; i>0; --i){
		creator.id = i;
		creator.progress = 0;
		creator.weight = randWeight(maxWeight);
		allPackages[i] = creator;
	}
	
	// put packages on bands
	int p1 = 1;
	double t1, t2, t3;
	t1 = t2 = t3 = -1.0;
	t1 = timeOnBand (allPackages[p1].weight, wBand_len, wBand_str);
	printf ("the package will take %f seconds on the W band \n", t1);
	t2 = timeOnBand (allPackages[p1].weight, sBand_len, sBand_str);
	printf ("the package will take %f seconds on the R band \n", t2);
	t3 = timeOnBand (allPackages[p1].weight, rBand_len, rBand_str);
	printf ("the package will take %f seconds on the S band \n", t3);

	return 0;
}
