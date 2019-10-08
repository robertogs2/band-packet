//
// Created by Jimena Salas Alfaro on 8/10/19.
//

#ifndef SCHEDULERS_SCHEDULERS_H
#define SCHEDULERS_SCHEDULERS_H

#include "linked_list.h"

const double QUANTUM = 2;


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
bool get_used_time(package_t* pack);

/**
 * Sets time when a package begins to being used by Round Robin scheduler
 * @param pack
 */
void set_usage_time_start(package_t* pack);



#endif //SCHEDULERS_SCHEDULERS_H
