//
// Created by Jimena Salas Alfaro on 8/10/19.
//

#ifndef SCHEDULERS_SCHEDULERS_H
#define SCHEDULERS_SCHEDULERS_H

#include "linked_list.h"
#include <time.h>

enum scheduler_type{
  ROUND_ROBIN,
  PRIORITY,
  SHORTEST_FIRST,
  FIFO,
  RTOS
};


/**
 * Function to validate if quantum is already over for Round Robin
 * @param pack package to evaluate
 * @return true if time is over, else false.
 */
double get_used_time(package_t* pack);

/**
 * Sets time when a package begins to being used by Round Robin scheduler
 * @param pack
 */
void set_usage_time_start(package_t* pack);

/**
 * Schedule a list of packages using priority policy
 * @param list_packages
 */
void schedule_priority(Node_t* list_packages);

/**
 * Schedule a list of packages using Round Robin policy
 * @param list_packages
 */
int schedule_round_robin(Node_t** list_packages, double quantum);

/**
 * Schedule a list of packages using shortest first policy
 * @param list_packages
 */
void schedule_shortest_first(Node_t* list_packages);

/**
 * Schedule a list of packages using FIFO policy
 * @param list_packages
 */
void schedule_fifo(Node_t* list_packages);


/**
 * Schedule a list of packages using RTOS policy
 * @param list_packages
 */
int schedule_real_time(Node_t** list_packages);



int partition(Node_t* list_packages, int low, int high, enum scheduler_type type);
void quick_sort(Node_t* list_packages, int low, int high, enum scheduler_type type);
int get_start_index(Node_t * list_packages, int element);
int get_end_index(Node_t *list_packages, int start);
bool check_sorted(Node_t* list_packages, int property); //0 priority, 1 remaining time
bool check_sorted_range(Node_t *list_packages, int property, int low, int high);



#endif //SCHEDULERS_SCHEDULERS_H
