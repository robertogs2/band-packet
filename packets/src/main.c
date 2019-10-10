#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include <time.h> 
#include "../include/linked_list.h"
#include "../include/packageGenerator.h"

// run one (1) for each band

int main (){

    // READ DATA FROM .conf FILE
    int mean = 10;
    int stdDev = 2;
    int distro = 1;
    int pRadioactive = 20;  //percentage of Radioactive packages
	int pUrgente = 20;		//percentage of Priority packages
    int pLeft = 95;
    int pSignBand = 40;
    int pRandBand = 20;

    // Start package generation
    int packageCounter = 0;
    int newPkgs = 5; //randNum(mean, stdDev, distro);
    Node_t * head = NULL;
    package_t packages[newPkgs];

    for(int i=0; newPkgs>i; ++i){
        createPackage(&packageCounter, &packages[i], pRadioactive, pUrgente, pLeft, pSignBand, pRandBand);
        push_back(&head, &packages[i]);
    }

    checkPackage(get_at(head, 2));

    return 0;
}
