//
// Created by lionheart on 8/10/19.
//

#include "../include/schedulers.h"

//gets index in which the element first appears, assuming ordered list
int get_start_index(Node_t * list_packages, int element){
  int length = get_length(list_packages);
  int index = -1;
  for(int i =0; i < length; i++){
    if(get_at(list_packages, i)->priority == element) {
      index = i;
      break;
    }
  }
  return index;
}

//gets index in which the element given changes, assuming list is ordered
int get_end_index(Node_t *list_packages, int start){
  int length = get_length(list_packages);
  int index = start;
  for(int i =index+1; i < length; i++){
    //start to count
    if(get_at(list_packages, i)->priority != get_at(list_packages, i-1)->priority){
      index = i;
      break;
    }
    if(i == length -1) index = length;
  }
  return index;
}

void schedule_priority(Node_t *list_packages) {
  if(!check_sorted(list_packages, 0)){
   // printf("Rescheduling priority\n");
    quick_sort(list_packages, 0, get_length(list_packages)-1, PRIORITY);
//    print_list(list_packages,0);
//    print_list(list_packages,1);
  }

}

void schedule_shortest_first(Node_t *list_packages) {
  if(!check_sorted(list_packages, 1)) {
    //printf("Rescheduling shortest first\n");
    quick_sort(list_packages, 0, get_length(list_packages) - 1, SHORTEST_FIRST);
//    print_list(list_packages,0);
//    print_list(list_packages,2);
  }
}

int schedule_real_time(Node_t **list_packages) {
  int org_pkg_id = get_at(*list_packages, 0)->id;
  int changed = 0;
  bool sorted_priority = true, sorted_time_0 =true, sorted_time_1=true, sorted_time_2=true;
  double cpu_time_used;
  //for changing element
  sorted_priority = check_sorted(*list_packages, 0); //1 if it is sorted in priority order

  if(!sorted_priority){
   // printf("Rescheduling priority for RTOS\n");
    //To give priority to hard time
    quick_sort(*list_packages, 0, get_length(*list_packages)-1, PRIORITY);
    if(get_at(*list_packages, 0)->id != org_pkg_id){
      //change
      changed = 1;
      //save execution time
      cpu_time_used = get_used_time(get_by_id(*list_packages,org_pkg_id));
      get_by_id(*list_packages,org_pkg_id)->accum_execution_time += cpu_time_used;
    }
  }

  //already ordered by priority
  int start_0 =  get_start_index(*list_packages, 0), end_0 = -1;
  int start_1 =  get_start_index(*list_packages, 1), end_1 = -1;
  int start_2 =  get_start_index(*list_packages, 2), end_2 = -1;

  //if the element exists
  if(start_0 != -1){
    end_0 = get_end_index(*list_packages, start_0);
    sorted_time_0 = check_sorted_range(*list_packages, 1, start_0, end_0-1);
  }
  if(start_1 != -1){
    end_1 = get_end_index(*list_packages, start_1);
    sorted_time_1 = check_sorted_range(*list_packages, 1, start_1, end_1-1);
  }
  if(start_2 != -1){
    end_2 = get_end_index(*list_packages, start_2);
    sorted_time_2 = check_sorted_range(*list_packages, 1, start_2, end_2-1);
  }

  //needs time sorting and priority sorting was ok
  if(!sorted_time_0 || !sorted_time_1 || !sorted_time_2){

    //printf("Rescheduling time for RTOS\n");
    if(!sorted_time_0) quick_sort(*list_packages, start_0, end_0-1, RTOS);
    if(!sorted_time_1) quick_sort(*list_packages, start_1, end_1-1, RTOS);
    if(!sorted_time_2) quick_sort(*list_packages, start_2, end_2-1, RTOS);

    if(get_at(*list_packages, 0)->id != org_pkg_id && sorted_priority){
      //change
      changed = 1;
      //save execution time
      cpu_time_used = get_used_time(get_by_id(*list_packages,org_pkg_id));
      get_by_id(*list_packages,org_pkg_id)->accum_execution_time += cpu_time_used;
    }
  }
  if(changed){
    //set start time of new package
    set_usage_time_start(get_at(*list_packages,0));
  }
  return changed;
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

bool check_sorted_range(Node_t *list_packages, int property, int low, int high) {
  int length = get_length(list_packages);
  int range = high - low;
  if(range >= 1){
    for(int i = low+1; i <= high; ++i){
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











