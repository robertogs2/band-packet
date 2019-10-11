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
    int mean = 20;          // mean of packages created, cte?
    int stdDev = 5;         // variation of packages created (max 1/4 of mean), cte?
    int packageCounter = 0;

    // Start package generation
    config_t bandConf = get_config(bandId);
    int newPkgs = randNum(mean, stdDev, bandConf.bandDistro);
    package_t packages[newPkgs];

    srand(time(0));  

    for(int i=0; newPkgs>i; ++i){
        createPackage(&packageCounter, &packages[i], bandId);
        push_back(&head, &packages[i]);
    }

    // DEBUG
    checkPackage(get_at(head, 2));

    return 0;
}
