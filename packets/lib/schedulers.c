//
// Created by lionheart on 8/10/19.
//

#include "../include/schedulers.h"

void schedule_priority(Node_t *list_packages) {
  quick_sort(list_packages, 0, get_length(list_packages)-1, PRIORITY);
}

void schedule_shortest_first(Node_t *list_packages) {
  quick_sort(list_packages, 0, get_length(list_packages)-1, SHORTEST_FIRST);
}

void schedule_real_time(Node_t *list_packages, double limit_time) {
  quick_sort(list_packages, 0, get_length(list_packages)-1, RTOS);
}


int partition(Node_t *list_packages, int low, int high, enum scheduler_type type) {
  short pivot;
  int  i = (low - 1);
  int j;

  switch(type){
    case PRIORITY:{
      pivot = get_at(list_packages, high)->priority;
      for(j = low; j <= high ; ++j){
        if(get_at(list_packages, j)->priority < pivot){
          i++;
          swap(list_packages, i, j);
        }
      }
      break;
    }
    case SHORTEST_FIRST:{
      pivot = get_at(list_packages,high)->execution_time;
      for(j = low; j <= high ; ++j){
        if(get_at(list_packages, j)->execution_time < pivot){
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
    int pi = partition(list_packages, low, high, type);
    quick_sort(list_packages, low, pi - 1, type);
    quick_sort(list_packages, pi+1, high, type);
  }
}









