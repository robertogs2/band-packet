//
// Created by lionheart on 8/10/19.
//

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef SCHEDULERS_LINKED_LIST_H
#define SCHEDULERS_LINKED_LIST_H


typedef struct config {
    int bandID;
    int bandStrength;
    int bandLength;
    int bandDistro;
    int bandMean;
    int bandStdDev;
    int packageRadsP;
    int packageUrgeP;
    int packageLeftP;
    int bandQuantum;
    int bandParameter;
    int bandAlgorithm;
    int bandScheduler;
} config_t;

typedef struct package {
  int id;
  float weight;
  short side;
  short priority;
  short progress;
  double speed;
  short band;
  bool on_band;
  double accum_execution_time;
  double current_execution_time;
  double total_execution_time;
  double remaining_time; //total_execution_time - current_execution_time
  clock_t usage_time_start;
} package_t;


typedef struct Node{
  package_t * value;
  struct Node * next;
} Node_t;

//prints the ids of the package
void print_list(Node_t * head, int property);
void push_back(Node_t ** head, package_t *value);
void push_front(Node_t ** head, package_t *value);

//returns the id of the popped element
package_t * pop_front(Node_t **head);
package_t * pop_back(Node_t * head);
package_t * remove_at(Node_t **head, int index);

//returns the package at a position
package_t * get_at(Node_t *head, int index);

//returns the position of a package
package_t * get_by_id(Node_t *head, int id);


//sets the package at a position
package_t * set_at(Node_t *head, int index, package_t * value);

//swaps the content of two nodes
void swap(Node_t *head, int index1, int index2);

//returns the length of the list
int get_length(Node_t * head);





#endif //SCHEDULERS_LINKED_LIST_H
