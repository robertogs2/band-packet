//
// Created by lionheart on 8/10/19.
//

#include "../include/schedulers.h"

void schedule_priority(Node_t *list_packages) {
  if(!check_sorted(list_packages, 0))
    quick_sort(list_packages, 0, get_length(list_packages)-1, PRIORITY);
}

void schedule_shortest_first(Node_t *list_packages) {
  if(!check_sorted(list_packages, 1))
    quick_sort(list_packages, 0, get_length(list_packages)-1, SHORTEST_FIRST);
}

void schedule_real_time(Node_t *list_packages, double limit_time) {
  quick_sort(list_packages, 0, get_length(list_packages)-1, RTOS);
}


int partition(Node_t *list_packages, int low, int high, enum scheduler_type type) {
  double pivot;
  int  i = (low - 1); //index of smaller element
  int j;

  switch(type){
    case PRIORITY:{
      pivot = get_at(list_packages, high)->priority; //pivot
      for(j = low; j <= high ; ++j){
        //If current element is smaller than the pivot
        if(get_at(list_packages, j)->priority < pivot){
          i++; //increment index of smaller element
          swap(list_packages, i, j);
        }
      }
      break;
    }
    case SHORTEST_FIRST:{
      pivot = get_at(list_packages,high)->total_execution_time;
      for(j = low; j <= high ; ++j){
        if(get_at(list_packages, j)->total_execution_time < pivot){
          i++;
          swap(list_packages, i, j);
        }
      }
      break;
    }

    case RTOS:{
      pivot = get_at(list_packages,high)->remaining_time;
      for(j = low; j <= high ; ++j){
        if(get_at(list_packages, j)->remaining_time < pivot){
          i++;
          swap(list_packages, i, j);
        }
      }
      break;
    }
    default:
      printf("Undefined scheduler for quicksort");
      return 0;
  }

  swap(list_packages, i+1, high);
  return i+1;
}

void quick_sort(Node_t *list_packages, int low, int high, enum scheduler_type type) {
  if(low < high){
    //partitioning index, list[p] is now at right place
    int pi = partition(list_packages, low, high, type);
    //separate sort elements before and after partition
    quick_sort(list_packages, low, pi - 1, type);
    quick_sort(list_packages, pi+1, high, type);
  }
}

void schedule_fifo(Node_t *list_packages) {
  //If packages are pushed back to the list then it is currently sorted
}

int schedule_round_robin(Node_t **list_packages, double quantum) {
  double cpu_time_used = get_used_time(get_at(*list_packages,0));
  //time is over go to next package
  if(cpu_time_used > quantum){
    //save execution time
    get_at(*list_packages,0)->accum_execution_time += cpu_time_used;
    //pop front and push back, circular rotation
    push_back(list_packages, pop_front(list_packages));
    //set start time of new package
    set_usage_time_start(get_at(*list_packages,0));
    return 1;
  }
  return 0;
}

void set_usage_time_start(package_t *pack) {
  pack->usage_time_start = clock();
}

double get_used_time(package_t *pack) {
  clock_t current_clock = clock();
  return ((double) (current_clock - pack->usage_time_start)) / CLOCKS_PER_SEC ;
}

bool check_sorted(Node_t *list_packages, int property) {
  int length = get_length(list_packages);
  if(length >= 2){
    for(int i = 1; i < length; ++i){
      if(property == 0){
        if(get_at(list_packages, i)->priority < get_at(list_packages, i-1)->priority){
          return false;
        }
      }
      else if(property == 1) {
        if (get_at(list_packages, i)->remaining_time < get_at(list_packages, i - 1)->remaining_time) {
          return false;
        }
      }
    }
  }
  return true;
}









