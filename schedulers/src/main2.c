#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include <time.h> 
#include "linked_list.h"
#include "package_generator.h"

int main (){

    // BASIC DATA FOR PACKAGE GENERATION
    Node_t * head = NULL;
    int bandId = 1;
    int packageCounter = 0;

    // Start package generation
    srand(time(0));
    config_t bandConf = get_config(bandId);

    //checkConfig(bandConf);

    int mean = bandConf.bandMean;          // mean of packages created, cte?
    int stdDev = bandConf.bandStdDev;
    int newPkgs = 5; //randNum(mean, stdDev, bandConf.bandDistro);

      

    for(int i=0; newPkgs>i; ++i){
        package_t newPackage;
        createPackage(&packageCounter, &newPackage, bandId);
        push_back(&head, &newPackage);
    }

    // DEBUG
    //checkPackage(get_at(head, 2));
    

    return 0;
}
