//
// Created by lionheart on 8/10/19.
//

#ifndef SCHEDULERS_LINKED_LIST_H
#define SCHEDULERS_LINKED_LIST_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct package {
  int id;
  float weight;
  short side;
  short priority;
  short progress;
  short band;
  bool on_band;
  short execution_time;
  short usage_time_start;
} package_t;


typedef struct Node{
  package_t * value;
  struct Node * next;
} Node_t;

//prints the ids of the package
void print_list(Node_t * head);
void push_back(Node_t ** head, package_t *value);
void push_front(Node_t ** head, package_t *value);

//returns the id of the popped element
package_t * pop_front(Node_t **head);
package_t * pop_back(Node_t * head);
package_t * remove_at(Node_t **head, int index);

//returns the package at a position
package_t * get_at(Node_t *head, int index);

//sets the package at a position
package_t * set_at(Node_t *head, int index, package_t * value);

//swaps the content of two nodes
void swap(Node_t *head, int index1, int index2);

//returns the length of the list
int get_length(Node_t * head);





#endif //SCHEDULERS_LINKED_LIST_H
