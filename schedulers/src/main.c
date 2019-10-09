#include "../include/linked_list.h"
#include "../include/schedulers.h"
#include <pthread.h>
#include <unistd.h>


const double QUANTUM = 5;
Node_t * list_packages = NULL;


typedef struct ThreadParams{
  enum scheduler_type type;
  double quantum;
  double limit_time;
} params_t;

void wait(double seconds){
  clock_t start;
  start = clock();
  double elapsed_time = 0;
  while(elapsed_time < seconds){
    elapsed_time = ((double) (clock() - start)) / CLOCKS_PER_SEC ;
  }
}

void* process_packages(void* params_ptr){
  params_t *params = (params_t*)params_ptr;
  if(params->type == ROUND_ROBIN){
    //start algorithm
    set_usage_time_start(get_at(list_packages,0));
    while(get_length(list_packages) > 0){
      schedule_round_robin(&list_packages, params->quantum);
      wait(0.5);
    }
  }
  return 0;
}




int main() {
  printf("Hello, World!\n");

  

  package_t packages[6];

  for(int i = 0; i < 5; ++i){
    packages[i].id = i;
    push_back(&list_packages, &packages[i]);
  }

  print_list(list_packages);

  packages[0].priority = 5;
  packages[1].priority = 2;
  packages[2].priority = 3;
  packages[3].priority = 1;
  packages[4].priority = 4;

  packages[0].execution_time = 30;
  packages[1].execution_time = 40;
  packages[2].execution_time = 2;
  packages[3].execution_time = 5;
  packages[4].execution_time = 50;

  packages[0].remaining_time = 34;
  packages[1].remaining_time = 5;
  packages[2].remaining_time = 3;
  packages[3].remaining_time = 50;
  packages[4].remaining_time = 7;

  printf("Schedule by priority\n");
  schedule_priority(list_packages);
  print_list(list_packages);
  //3 1 2 4 0

  printf("Schedule by shortest first\n");
  schedule_shortest_first(list_packages);
  print_list(list_packages);
  //2 3 0 1 4

  printf("Schedule by 'real time'\n");
  schedule_real_time(list_packages,50);
  print_list(list_packages);
  //2 1 4 0 3

  printf("Circular rotation\n");
  push_back(&list_packages, pop_front(&list_packages));
  print_list(list_packages);

  pthread_t t_id_0;
  params_t *params = malloc(sizeof(params_t));
  params->quantum = QUANTUM;
  params->type = ROUND_ROBIN;


  if(pthread_create(&t_id_0, NULL, &process_packages, (void *) params) != 0)
    printf("\nCould not created Thread 0\n");

  pthread_join(t_id_0, NULL);


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