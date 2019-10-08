#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/constants.h"

const char* path_to_wband_config = "../config/bandW.conf";
const char* path_to_signband_config = "../config/bandSign.conf";
const char* path_to_randband_config = "../config/bandRand.conf";
const char* aux_file = "/etc/server/config.aux";

config_t get_config(const char* conf_path){
    config_t conf;
    FILE* file = fopen(conf_path, "r");
    if(file==NULL){
        printf ("Path to config not avaliable. \n");
        exit(EXIT_FAILURE); 
    }
    char line[256];
    char prev[256];
    while (fgets(line, sizeof(line), file)) {
        char* current = strtok (line, "=:");
        //prev={0};
        while (current) {
            if(!strcmp(prev,"bandID")){
                conf.bandID = atoi(current);
            }
            else if(!strcmp(prev,"bandStrength")){
                conf.bandStrength = atoi(current);
            }
            strcpy(prev, current);
            current = strtok (NULL, "=:");
        }
    }
    fclose(file);
    return conf;
}   

int main() {
    //int buffer_size=512;
    //char buffer[buffer_size];
    //char dateBuffer[buffer_size];

    // Prepare configuration struct
    config_t wBand_config = get_config(path_to_wband_config); //Read the file


    //Copies data to pointer
    config_t* conf_ptr = malloc(sizeof(config_t));
    conf_ptr->bandID=wBand_config.bandID;
    conf_ptr->bandStrength= wBand_config.bandStrength;
    //conf_ptr->bandLength, conf.bandLength;
    //conf_ptr->packageDistro, conf.packageDistro;
    //conf_ptr->packageRadsP, conf.packageRadsP;
    //conf_ptr->packagePrioP, conf.packagePrioP;
    //conf_ptr->packageLeftP, conf.packageLeftP;
    //conf_ptr->bandParameter, conf.bandParameter;

    // Initializes files
    //write_file(aux_file, "");

    printf("W Band is id %d and has a strenth of %d newtons", wBand_config.bandID, wBand_config.bandStrength);
    
    return EXIT_SUCCESS;
}