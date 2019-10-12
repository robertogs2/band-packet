#include "../include/linked_list.h"
#include "../include/schedulers.h"
#include <pthread.h>
#include <unistd.h>
#include "../include/lpthread.h"
#include "../include/package_generator.h"
#include "../include/band_control.h"

char* scheduler_names[5] = {"ROUND_ROBIN", "PRIORITY", "SHORTEST_FIRST", "FIFO", "RTOS"};
char* side_names[2] = {"LEFT", "RIGHT"};

const double QUANTUM = 1;
const double BAND_DISTANCE = 100;
char command[256];
char buffer1[256];
char buffer2[256];

lpthread_mutex_t mutex_file;

Node_t * list_packages_0 = NULL;
Node_t * list_packages_1 = NULL;
Node_t * list_packages_2 = NULL;
Node_t * list_packages_3 = NULL;
Node_t * list_packages_4 = NULL;
Node_t * list_packages_5 = NULL;

int pkg_counter_0 = 0;
int pkg_counter_1 = 0;
int pkg_counter_2 = 0;
int pkg_counter_3 = 0;
int pkg_counter_4 = 0;
int pkg_counter_5 = 0;

int pkg_counters[6];
Node_t** lists[6];

side_controller_t ctrls[3];

typedef struct ThreadParams{
  int id;
  int side_id;
  enum scheduler_type type;
  double quantum;
  double limit_time;
  short side;
} params_t;

void write_file(const char* filename, const char* msg){
  Lmutex_lock(&mutex_file);
  sprintf(command, "echo '%s' > '%s'", msg, filename);
  system(command);
  Lmutex_unlock(&mutex_file);
}

void write_progress(int thread_id, int list_id, short side){
  //write progress to file
  sprintf(buffer1, "%d\n%d\n%d\n%d", get_at(*lists[list_id], 0)->id,
          get_at(*lists[list_id], 0)->progress, get_at(*lists[list_id], 0)->priority, side);
  sprintf(buffer2, "../../gui/data/band_%d.txt", thread_id);
  write_file(buffer2, buffer1);
}

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

  //printf("Progress of package: %d is %d. Remaining time: %fs/%fs\n", pack->id, pack->progress, pack->remaining_time, pack->total_execution_time);
}

int create_gui(){
  system("python3 ../../gui/gui.py");
  return 0;
}

int process_packages(void* params_ptr){

  params_t *params = (params_t*)params_ptr;
  side_controller_t ctrl = ctrls[params->id]; 
  if(params->type == ROUND_ROBIN){
    printf("Algorithm %s for thread %d ", scheduler_names[params->type], params->id);
    print_list(*lists[params->side_id]);
    //start algorithm
    set_usage_time_start(get_at(*lists[params->side_id], 0));
    while(get_length(*lists[params->side_id]) > 0){
      if(get_at(*lists[params->side_id], 0)->progress >= 100){
        //pop package and go to other
        pop_front(lists[params->side_id]);
        //if there are packages left
        if(get_length(*lists[params->side_id]) > 0){
          //set time of new package
          set_usage_time_start(get_at(*lists[params->side_id], 0));

          print_list(*lists[params->side_id]);
        }
      }
      else{
        schedule_round_robin(lists[params->side_id], params->quantum);
        update_progress(get_at(*lists[params->side_id], 0), ROUND_ROBIN);
        write_progress(params->id, params->side_id, params->side);
      }
      wait_seconds(0.1);
    }
  }
  //non appropriative
  else if(params->type == FIFO || params->type == PRIORITY || params->type == SHORTEST_FIRST){
    //start algorithm
    if(params->type == PRIORITY) schedule_priority(*lists[params->side_id]);
    else if(params->type == SHORTEST_FIRST) schedule_shortest_first(*lists[params->side_id]);

    printf("Algorithm %s for thread %d ", scheduler_names[params->type], params->id);
    print_list(*lists[params->side_id]);

    set_usage_time_start(get_at(*lists[params->side_id], 0));
    //while there are packages
    while(get_length(*lists[params->side_id]) > 0){
      //package is complete
      if(get_at(*lists[params->side_id], 0)->progress >= 100) {
        //pop that package and go for next one

        pop_front(lists[params->side_id]);
        short sidee = control_band(&ctrl);
        params->side_id = (sidee == 0) ? params->id:params->id+3;
        printf("Side %d\n", params->side_id);
          print_list(*lists[0]);
          print_list(*lists[3]);
        if(get_length(*lists[params->side_id]) > 0){
          //schedule again
          if(params->type == PRIORITY) schedule_priority(*lists[params->side_id]);
          else if(params->type == SHORTEST_FIRST) schedule_shortest_first(*lists[params->side_id]);
          //if fifo is is not necessary to reschedule
          //set time of new package
          set_usage_time_start(get_at(*lists[params->side_id], 0));
        }
      }
      else{
        //update progress
        update_progress(get_at(*lists[params->side_id], 0), params->type);
        write_progress(params->id, params->side_id, ctrl.last_side);//params->side);
      }
      wait_seconds(0.1);
    }
  }
  return 0;
}


void package_generation(){
  int bandId = 0;
  int counter_list = 0;

  for(int j=0; j < 6; j++){

    config_t bandConf = get_config(bandId);
    int mean = bandConf.bandMean;          // mean of packages created, cte?
    int stdDev = bandConf.bandStdDev;         // variation of packages created (max 1/4 of mean), cte?
    printf("Mean %d, StdDev: %d", mean, stdDev);
    int newPkgs = randNum(mean, stdDev, bandConf.bandDistro);
    printf("Quantity generated: %d for band id %d: %s\n", newPkgs, bandId, side_names[counter_list]);

    for(int i=0; newPkgs>i; ++i){
      package_t * newPackage = malloc(sizeof(package_t));
      createPackage(&pkg_counters[j], newPackage, bandId);
      push_back(lists[j], newPackage);
    }

    if(counter_list >= 1){
      bandId++;
      counter_list = 0;
    }
    else{
      counter_list ++;
    }
  }
}


void initialize_system(){
  printf("Initializing system :(\n");
  Lmutex_init(&mutex_file, NULL);
  // Left
  lists[0] = &list_packages_0;
  lists[1] = &list_packages_1;
  lists[2] = &list_packages_2;
  //Right
  lists[3] = &list_packages_3;
  lists[4] = &list_packages_4;
  lists[5] = &list_packages_5;

  pkg_counters[0] = pkg_counter_0;
  pkg_counters[1] = pkg_counter_1;
  pkg_counters[2] = pkg_counter_2;
  pkg_counters[3] = pkg_counter_3;
  pkg_counters[4] = pkg_counter_4;
  pkg_counters[5] = pkg_counter_5;

  init_controller(&ctrls[0], lists[0], lists[3], RANDOM_BAND);
  init_controller(&ctrls[1], lists[1], lists[4], W_BAND);
  init_controller(&ctrls[2], lists[2], lists[5], RANDOM_BAND);


  srand(time(NULL));
}
int main() {

  initialize_system();
  package_generation();

  print_list(*lists[0]);
  print_list(*lists[1]);
  print_list(*lists[2]);
  print_list(*lists[3]);
  print_list(*lists[4]);
  print_list(*lists[5]);



//
//  package_t packages_0[6];
//  package_t packages_1[6];
//  package_t packages_2[6];
//
//  for(int i = 0; i < 5; ++i){
//    packages_0[i].id = i;
//    packages_0[i].progress = 0;
//    packages_0[i].accum_execution_time = 0;
//    push_back(lists[0], &packages_0[i]);
//    packages_1[i].id = i;
//    packages_1[i].progress = 0;
//    packages_1[i].accum_execution_time = 0;
//    push_back(lists[1], &packages_1[i]);
//    packages_2[i].id = i;
//    packages_2[i].progress = 0;
//    packages_2[i].accum_execution_time = 0;
//    push_back(lists[2], &packages_2[i]);
//  }
//
//  packages_0[5].id = 5;
//
//  print_list(*lists[0]);
//
//  packages_0[0].priority = 3;
//  packages_0[1].priority = 2;
//  packages_0[2].priority = 2;
//  packages_0[3].priority = 1;
//  packages_0[4].priority = 1;
//
//  packages_0[5].priority = 1;
//
//  packages_0[0].speed = 5;
//  packages_0[1].speed = 10;
//  packages_0[2].speed = 15;
//  packages_0[3].speed = 20;
//  packages_0[4].speed = 35;
//
//  packages_0[5].speed = 70;
//
//  packages_0[0].total_execution_time = BAND_DISTANCE / packages_0[0].speed;
//  packages_0[1].total_execution_time = BAND_DISTANCE / packages_0[1].speed;
//  packages_0[2].total_execution_time = BAND_DISTANCE / packages_0[2].speed;
//  packages_0[3].total_execution_time = BAND_DISTANCE / packages_0[3].speed;
//  packages_0[4].total_execution_time = BAND_DISTANCE / packages_0[4].speed;
//  packages_0[5].total_execution_time = BAND_DISTANCE / packages_0[5].speed;
//
//  packages_0[0].remaining_time = packages_0[0].total_execution_time;
//  packages_0[1].remaining_time = packages_0[1].total_execution_time;
//  packages_0[2].remaining_time = packages_0[2].total_execution_time;
//  packages_0[3].remaining_time = packages_0[3].total_execution_time;
//  packages_0[4].remaining_time = packages_0[4].total_execution_time;
//  packages_0[5].remaining_time = packages_0[5].total_execution_time;
//
//  packages_1[0].priority = 2;
//  packages_1[1].priority = 2;
//  packages_1[2].priority = 3;
//  packages_1[3].priority = 1;
//  packages_1[4].priority = 1;
//
//  packages_1[5].priority = 1;
//
//  packages_1[0].speed = 80;
//  packages_1[1].speed = 90;
//  packages_1[2].speed = 85;
//  packages_1[3].speed = 90;
//  packages_1[4].speed = 85;
//
//  packages_1[5].speed = 70;
//
//  packages_1[0].total_execution_time = BAND_DISTANCE / packages_1[0].speed;
//  packages_1[1].total_execution_time = BAND_DISTANCE / packages_1[1].speed;
//  packages_1[2].total_execution_time = BAND_DISTANCE / packages_1[2].speed;
//  packages_1[3].total_execution_time = BAND_DISTANCE / packages_1[3].speed;
//  packages_1[4].total_execution_time = BAND_DISTANCE / packages_1[4].speed;
//  packages_1[5].total_execution_time = BAND_DISTANCE / packages_1[5].speed;
//
//  packages_1[0].remaining_time = packages_1[0].total_execution_time;
//  packages_1[1].remaining_time = packages_1[1].total_execution_time;
//  packages_1[2].remaining_time = packages_1[2].total_execution_time;
//  packages_1[3].remaining_time = packages_1[3].total_execution_time;
//  packages_1[4].remaining_time = packages_1[4].total_execution_time;
//  packages_1[5].remaining_time = packages_1[5].total_execution_time;
//
//  packages_2[0].priority = 3;
//  packages_2[1].priority = 2;
//  packages_2[2].priority = 3;
//  packages_2[3].priority = 1;
//  packages_2[4].priority = 2;
//
//  packages_2[5].priority = 1;
//
//  packages_2[0].speed = 35;
//  packages_2[1].speed = 40;
//  packages_2[2].speed = 45;
//  packages_2[3].speed = 50;
//  packages_2[4].speed = 55;
//
//  packages_2[5].speed = 70;
//
//  packages_2[0].total_execution_time = BAND_DISTANCE / packages_2[0].speed;
//  packages_2[1].total_execution_time = BAND_DISTANCE / packages_2[1].speed;
//  packages_2[2].total_execution_time = BAND_DISTANCE / packages_2[2].speed;
//  packages_2[3].total_execution_time = BAND_DISTANCE / packages_2[3].speed;
//  packages_2[4].total_execution_time = BAND_DISTANCE / packages_2[4].speed;
//  packages_2[5].total_execution_time = BAND_DISTANCE / packages_2[5].speed;
//
//  packages_2[0].remaining_time = packages_2[0].total_execution_time;
//  packages_2[1].remaining_time = packages_2[1].total_execution_time;
//  packages_2[2].remaining_time = packages_2[2].total_execution_time;
//  packages_2[3].remaining_time = packages_2[3].total_execution_time;
//  packages_2[4].remaining_time = packages_2[4].total_execution_time;
//  packages_2[5].remaining_time = packages_2[5].total_execution_time;
//
//  //printf("Schedule by Round Robin\n");
//
//
  lpthread_t t_gui;
//  lpthread_t t_gui;
  if(Lthread_create(&t_gui, NULL, &create_gui, NULL) != 0)
    printf("\nCould not created Thread GUI\n");
//
  lpthread_t t_id_0;
  params_t *params_0 = malloc(sizeof(params_t));
  params_0->id = 0;
  params_0->side_id = 0;
  params_0->quantum = QUANTUM;
  params_0->type = FIFO;
  params_0->side = 1;
//
  if(Lthread_create(&t_id_0, NULL, &process_packages, (void *) params_0) != 0)
    printf("\nCould not created Thread 0\n");

 // lpthread_t t_id_1;
 // params_t *params_1 = malloc(sizeof(params_t));
 // params_1->id = 1;
 // params_1->side_id = 1;
 // params_1->quantum = QUANTUM;
 // params_1->type = PRIORITY;
 // params_1->side = 0;

 // if(Lthread_create(&t_id_1, NULL, &process_packages, (void *) params_1) != 0)
 //   printf("\nCould not created Thread 1\n");

 // lpthread_t t_id_2;
 // params_t *params_2 = malloc(sizeof(params_t));
 // params_2->id = 2;
 // params_2->side_id = 2;
 // params_2->quantum = QUANTUM;
 // params_2->type = FIFO;
 // params_2->side = 1;

 // if(Lthread_create(&t_id_2, NULL, &process_packages, (void *) params_2) != 0)
 //   printf("\nCould not created Thread 2\n");

 //push_back(&list_packages_0, &packages_0[5]);

  Lthread_join(t_id_0, NULL);

  // Lthread_join(t_id_1, NULL);
  // Lthread_join(t_id_2, NULL);
  Lthread_join(t_gui, NULL);

  return 0;
}