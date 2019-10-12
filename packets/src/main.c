#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include <time.h> 
#include "../include/linked_list.h"
#include "../include/packageGenerator.h"

int main (){

    // BASIC DATA FOR PACKAGE GENERATION
    Node_t * head = NULL;
    int bandId = 1;
    int packageCounter = 0;

    // Start package generation
    config_t bandConf = get_config(bandId);
    int mean = bandConf.bandMean;          // mean of packages created, cte?
    int stdDev = bandConf.bandStdDev;         // variation of packages created (max 1/4 of mean), cte?
    int newPkgs = randNum(mean, stdDev, bandConf.bandDistro);

    srand(time(0));  

    for(int i=0; newPkgs>i; ++i){
        package_t newPackage;
        createPackage(&packageCounter, &newPackage, bandId);
        push_back(&head, &newPackage);
    }

    // DEBUG
    checkPackage(get_at(head, 2));

    return 0;
}
