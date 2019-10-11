#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include <time.h> 
#include "../include/linked_list.h"
#include "../include/packageGenerator.h"

int main (){

    // GLOBAL DATA FOR PACKAGE GENERATION
    int mean = 20;          // mean of packages created
    int stdDev = 5;         // variation of packages created (max 1/4 of mean)
    int pSignBand = 40;     // percentage of packages sent to Sign controlled band
    int pRandBand = 20;     // percentage of packages sent to Random controlled band  

    // Start package generation
    int packageCounter = 0;
    int newPkgs = 5; //randNum(mean, stdDev, w.packageDistro);
    Node_t * head = NULL;
    package_t packages[newPkgs];

    srand(time(0));  

    for(int i=0; newPkgs>i; ++i){
        createPackage(&packageCounter, &packages[i], pSignBand, pRandBand);
        push_back(&head, &packages[i]);
    }

    checkPackage(get_at(head, 2));

    return 0;
}
