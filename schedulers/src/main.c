#include "../include/linked_list.h"
#include "../include/schedulers.h"
#include <pthread.h>
#include <unistd.h>
#include "../include/lpthread.h"

const double QUANTUM = 5;
const double BAND_DISTANCE = 100;

Node_t * list_packages = NULL;


typedef struct ThreadParams{
  enum scheduler_type type;
  double quantum;
  double limit_time;
} params_t;

void wait_seconds(double seconds){
  clock_t start;
  start = clock();
  double elapsed_time = 0;
  while(elapsed_time < seconds){
    elapsed_time = ((double) (clock() - start)) / CLOCKS_PER_SEC ;
  }
}

void update_progress(package_t* pack, enum scheduler_type type){
  if(type == ROUND_ROBIN) pack->current_execution_time = get_used_time(pack) + pack->accum_execution_time;
  else pack->current_execution_time = get_used_time(pack);

  pack->progress = (short) (pack->speed * pack->current_execution_time);
  pack->remaining_time = pack->total_execution_time - pack->current_execution_time;
  printf("Progress of package: %d is %d. Remaining time: %fs/%fs\n", pack->id, pack->progress, pack->remaining_time, pack->total_execution_time);
}


int process_packages(void* params_ptr){
  params_t *params = (params_t*)params_ptr;
  if(params->type == ROUND_ROBIN){
    //start algorithm
    set_usage_time_start(get_at(list_packages,0));
    while(get_length(list_packages) > 0){
      if(get_at(list_packages, 0)->progress >= 100){
        //pop package and go to other
        pop_front(&list_packages);
        //if there are packages left
        if(get_length(list_packages) > 0){
          //set time of new package
          set_usage_time_start(get_at(list_packages, 0));
          print_list(list_packages);
        }
      }
      else{
        schedule_round_robin(&list_packages, params->quantum);
        update_progress(get_at(list_packages, 0), ROUND_ROBIN);
      }
      wait_seconds(0.5);
    }
  }
  //non appropriative
  else if(params->type == FIFO || params->type == PRIORITY || params->type == SHORTEST_FIRST){
    //start algorithm
    set_usage_time_start(get_at(list_packages, 0));
    //while there are packages
    while(get_length(list_packages) > 0){
      //package is complete
      if(get_at(list_packages, 0)->progress >= 100) {
        //pop that package and go for next one
        pop_front(&list_packages);

        if(get_length(list_packages) > 0){
          //schedule again
          if(params->type == PRIORITY) schedule_priority(list_packages);
          else if(params->type == SHORTEST_FIRST) schedule_shortest_first(list_packages);
          //if fifo is is not necessary to reschedule

          //set time of new package
          set_usage_time_start(get_at(list_packages, 0));
        }
      }
      else{
        //update progress
        update_progress(get_at(list_packages, 0), params->type);
      }
      wait_seconds(0.1);
    }
  }
  return 0;
}




int main() {
  printf("Hello, World!\n");

  package_t packages[6];

  for(int i = 0; i < 5; ++i){
    packages[i].id = i;
    packages[i].progress = 0;
    packages[i].accum_execution_time = 0;
    push_back(&list_packages, &packages[i]);
  }

  packages[5].id = 5;

  print_list(list_packages);

  packages[0].priority = 5;
  packages[1].priority = 2;
  packages[2].priority = 3;
  packages[3].priority = 1;
  packages[4].priority = 4;

  packages[5].priority = 1;

  packages[0].speed = 5;
  packages[1].speed = 10;
  packages[2].speed = 15;
  packages[3].speed = 20;
  packages[4].speed = 25;

  packages[5].speed = 70;

  packages[0].total_execution_time = BAND_DISTANCE / packages[0].speed;
  packages[1].total_execution_time = BAND_DISTANCE / packages[1].speed;
  packages[2].total_execution_time = BAND_DISTANCE / packages[2].speed;
  packages[3].total_execution_time = BAND_DISTANCE / packages[3].speed;
  packages[4].total_execution_time = BAND_DISTANCE / packages[4].speed;
  packages[5].total_execution_time = BAND_DISTANCE / packages[5].speed;

//  packages[0].remaining_time = 34;
//  packages[1].remaining_time = 5;
//  packages[2].remaining_time = 3;
//  packages[3].remaining_time = 50;
//  packages[4].remaining_time = 7;

  packages[0].remaining_time = packages[0].total_execution_time;
  packages[1].remaining_time = packages[1].total_execution_time;
  packages[2].remaining_time = packages[2].total_execution_time;
  packages[3].remaining_time = packages[3].total_execution_time;
  packages[4].remaining_time = packages[4].total_execution_time;
  packages[5].remaining_time = packages[5].total_execution_time;

  printf("Schedule by Round Robin\n");
  print_list(list_packages);

  lpthread_t t_id_0;
  params_t *params = malloc(sizeof(params_t));
  params->quantum = QUANTUM;
  params->type = ROUND_ROBIN;

  if(Lthread_create(&t_id_0, NULL, &process_packages, (void *) params) != 0)
    printf("\nCould not created Thread 0\n");

  push_back(&list_packages, &packages[5]);

  Lthread_join(t_id_0, NULL);

//  printf("Schedule by FIFO\n");
//  print_list(list_packages);
//
//
//
//  printf("Schedule by priority\n");
//  schedule_priority(list_packages);
//  print_list(list_packages);
//  //3 1 2 4 0
//
//
//  printf("Schedule by shortest first\n");
//  schedule_shortest_first(list_packages);
//  print_list(list_packages);
//  //4 3 2 0 1
//
//
//
//
//  printf("Schedule by 'real time'\n");
//  schedule_real_time(list_packages,50);
//  print_list(list_packages);
//  //2 1 4 0 3


//  lpthread_t t_id_0;
//  params_t *params = malloc(sizeof(params_t));
//  params->quantum = QUANTUM;
//  params->type = SHORTEST_FIRST;
//
//  if(Lthread_create(&t_id_0, NULL, &process_packages, (void *) params) != 0)
//    printf("\nCould not created Thread 0\n");
//
//  Lthread_join(t_id_0, NULL);







//
//  pop_back(list_packages);
//
//  printf("%d\n", get_length(list_packages));
//  push_back(&list_packages, &packages[0]);
//  push_front(&list_packages, &packages[1]);
//  push_back(&list_packages, &packages[1]);
//  print_list(list_packages);
//  //printf("Value removed: %d \n",remove_at(&list_packages, 2)->id);
//  push_front(&list_packages, &packages[2]);
//  push_front(&list_packages, &packages[3]);
//  printf("%d\n", get_length(list_packages));
//  print_list(list_packages);
//  printf("Get Value: %d\n",get_at(list_packages, 0)->id);
//  printf("Get Value: %d\n",get_at(list_packages, 1)->id);
//  printf("Get Value: %d\n",get_at(list_packages, 2)->id);
//  printf("Get Value: %d\n",get_at(list_packages, 3)->id);
//  printf("Get Value: %d\n",get_at(list_packages, 4)->id);
//  print_list(list_packages);
//  printf("Set Value: %d\n",set_at(list_packages, 0, &packages[4])->id);
//  printf("Set Value: %d\n",set_at(list_packages, 1, &packages[4])->id);
//  printf("Set Value: %d\n",set_at(list_packages, 2, &packages[4])->id);
//  printf("Set Value: %d\n",set_at(list_packages, 3, &packages[4])->id);
//  printf("Set Value: %d\n",set_at(list_packages, 4, &packages[3])->id);
//  set_at(list_packages, 5, &packages[4]);
//  print_list(list_packages);
//  swap(list_packages, 3, 4);
//  print_list(list_packages);


  return 0;
}