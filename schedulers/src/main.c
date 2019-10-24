#include "../include/linked_list.h"
#include "../include/schedulers.h"
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>
#include "../include/lpthread.h"
#include "../include/package_generator.h"
#include "../include/band_control.h"

char* scheduler_names[5] = {"ROUND_ROBIN", "PRIORITY", "SHORTEST_FIRST", "FIFO", "RTOS"};
char* control_names[3] = {"W", "SIGN", "RANDOM"};


typedef struct ThreadParams{
    int id;
    int side_id;
    enum scheduler_type type;
    enum control_type control;
    double quantum;
    bool running;
} params_t;

char command[256];
char buffer1[256], buffer2[256], buffer3[256];

lpthread_mutex_t mutex_file;

params_t *params_0 = NULL, *params_1 = NULL, *params_2 = NULL;

Node_t * list_packages_0 = NULL, * list_packages_1 = NULL, * list_packages_2 = NULL, * list_packages_3 = NULL,
       * list_packages_4 = NULL, * list_packages_5 = NULL;

int pkg_counter_0 = 0, pkg_counter_1 = 0, pkg_counter_2 = 0, pkg_counter_3 = 0, pkg_counter_4 = 0, pkg_counter_5 = 0;

int INITIALIZED = 0;

int pkg_counters[NUMBER_LISTS];
Node_t** lists[NUMBER_LISTS];
side_controller_t ctrls[NUMBER_BANDS];

void write_file(const char* filename, const char* msg);
char* get_first_packages(int thread_id);
void write_progress(int thread_id, int list_id, short side);
void wait_seconds(double seconds);
void update_progress(package_t* pack, enum scheduler_type type);
int toggle_pause();
int toggle_pause_buttons();
int create_gui();
int process_packages(void* params_ptr);
int package_generation();
void initialize_system();
void initialize_threads();

int main(int argc, char** argv) {
  initialize_system();
  lpthread_t t_pkg_generation;
  if(Lthread_create(&t_pkg_generation, NULL, &package_generation, NULL))
    printf("\nCould not created Thread Package Generation\n");
  //Wait for initial package generation
  while(INITIALIZED == 0);
  initialize_threads();
  Lthread_join(t_pkg_generation, NULL);
  return 0;

}

/*
 * THREAD FUNCTIONS
 */

// Toggles the pause for some thread
int toggle_pause(){
  char key;
  //enable pause
  while(true){
    scanf("%c", &key);
    if(strcmp(&key, "0") == 0){
      params_0->running = !params_0->running;
    }
    else if(strcmp(&key, "1") == 0){
      params_1->running = !params_1->running;
    }
    else if(strcmp(&key, "2") == 0){
      params_2->running = !params_2->running;
    }
  }
  printf("Toggle band: \n");
}

// Toggles coming from the buttons
int toggle_pause_buttons(){
  char key;
  //enable pause
  
  char buffer[3];
  int button_count[3] = {0};
  int limit = 199;

  while(true){
    int fd = open("../../gui/data/buttons.txt", O_RDONLY);
    if(fd > 0){
      int r = read(fd, buffer, 3);
      char button0 = buffer[0];
      char button1 = buffer[1];
      char button2 = buffer[2];
      if(button0 == '1'){
        if(++button_count[0] == limit){
          params_0->running = !params_0->running;
        }
      }
      else{
        button_count[0] = 0;
      }
      if(button1 == '1'){
        if(++button_count[1] == limit){
          params_1->running = !params_1->running;
        }
      }
      else{
        button_count[1] = 0;
      }
      if(button2 == '1'){
        if(++button_count[2] == limit){
          params_2->running = !params_2->running;
        }
      }
      else{
        button_count[2] = 0;
      }
      close(fd);
    }
  }
}


// Function to start the gui
int create_gui(){
  system("python3 ../../gui/gui.py");
  return 0;
}

int process_packages(void* params_ptr){

  params_t *params = (params_t*)params_ptr;
  side_controller_t* ctrl = &ctrls[params->id];
  if(params->type == RTOS){
    //start algorithm
    schedule_real_time(lists[params->side_id]);
    set_usage_time_start(get_at(*lists[params->side_id], 0));
    while(get_length(*lists[params->side_id]) > 0) {
      if (params->running) {
        if (get_at(*lists[params->side_id], 0)->progress >= 100) {
          //pop package and go to other
          pop_front(lists[params->side_id]);
          short sidee = control_band(ctrl);
          params->side_id = (sidee == 0) ? params->id : params->id + 3;
          //if there are packages left
          if (get_length(*lists[params->side_id]) > 0) {
            //set time of new package
            set_usage_time_start(get_at(*lists[params->side_id], 0));
            update_progress(get_at(*lists[params->side_id], 0), RTOS);
            write_progress(params->id, params->side_id, ctrl->last_side);

          }
        } else {
          schedule_real_time(lists[params->side_id]);
          update_progress(get_at(*lists[params->side_id], 0), RTOS);
          write_progress(params->id, params->side_id, ctrl->last_side);
        }
        wait_seconds(0.1);
      }
      else{
        usleep(0.1 * 1000000);
      }
    }
  }

  else if(params->type == ROUND_ROBIN){
    //start algorithm
    set_usage_time_start(get_at(*lists[params->side_id], 0));
    while(get_length(*lists[params->side_id]) > 0){
      if(params->running){
        if(get_at(*lists[params->side_id], 0)->progress >= 100){
          //pop package and go to other
          pop_front(lists[params->side_id]);
          short sidee = control_band(ctrl);
          params->side_id = (sidee == 0) ? params->id:params->id+3;
          //if there are packages left
          if(get_length(*lists[params->side_id]) > 0){
            //set time of new package
            set_usage_time_start(get_at(*lists[params->side_id], 0));
            update_progress(get_at(*lists[params->side_id], 0), ROUND_ROBIN);
            write_progress(params->id, params->side_id, ctrl->last_side);

          }
        }
        else{
          int completed = schedule_round_robin(lists[params->side_id], params->quantum);
          if (completed){
            short sidee = control_band(ctrl);
            params->side_id = (sidee == 0) ? params->id:params->id+3;
            set_usage_time_start(get_at(*lists[params->side_id], 0));
            schedule_round_robin(lists[params->side_id], params->quantum);
          }
          update_progress(get_at(*lists[params->side_id], 0), ROUND_ROBIN);
          write_progress(params->id, params->side_id, ctrl->last_side);
        }
        wait_seconds(0.1);
      }
      else{
        usleep(0.1 * 1000000);
      }
    }
  }
  //non appropriative
  else if(params->type == FIFO || params->type == PRIORITY || params->type == SHORTEST_FIRST){
    //start algorithm
    if(params->type == PRIORITY) {
      schedule_priority(*lists[params->id]);
      schedule_priority(*lists[params->id + NUMBER_BANDS]);
    }
    else if(params->type == SHORTEST_FIRST){
      schedule_shortest_first(*lists[params->id]);
      schedule_shortest_first(*lists[params->id + NUMBER_BANDS]);
    }

    set_usage_time_start(get_at(*lists[params->side_id], 0));
    //while there are packages
    while(get_length(*lists[params->side_id]) > 0) {
      if (params->running) {
        //package is complete
        if (get_at(*lists[params->side_id], 0)->progress >= 100) {
          //pop that package and go for next one

          pop_front(lists[params->side_id]);
          short sidee = control_band(ctrl);
          params->side_id = (sidee == 0) ? params->id : params->id + 3;
          if (get_length(*lists[params->side_id]) > 0) {
            //schedule again
            if (params->type == PRIORITY) schedule_priority(*lists[params->side_id]);
            else if (params->type == SHORTEST_FIRST) schedule_shortest_first(*lists[params->side_id]);
            //if fifo is is not necessary to reschedule
            //set time of new package
            set_usage_time_start(get_at(*lists[params->side_id], 0));
          }
        } else {
          //update progress
          update_progress(get_at(*lists[params->side_id], 0), params->type);
          write_progress(params->id, params->side_id, ctrl->last_side);//params->side);
        }
        wait_seconds(0.1);
      }
      else{
        usleep(0.1 * 1000000);
      }
    }
  }
  return 0;
}

// Function to generate the packages
int package_generation(){
  while(true){

    for(int j=0; j < NUMBER_BANDS; j++){

      config_t bandConf = get_config(j);
      int mean = bandConf.bandMean;
      int stdDev = bandConf.bandStdDev;
      int newPkgs = randNum(mean, stdDev, bandConf.bandDistro);

      for(int i=0; newPkgs>i; ++i){
        package_t * newPackage = malloc(sizeof(package_t));
        createPackage(&pkg_counters[j], newPackage, j);
        push_back(lists[j], newPackage);
      }
      for(int i=0; newPkgs>i; ++i){
        package_t * newPackage = malloc(sizeof(package_t));
        createPackage(&pkg_counters[j+NUMBER_BANDS], newPackage, j);
        push_back(lists[j+NUMBER_BANDS], newPackage);
      }

    }
    INITIALIZED = 1;
    usleep( 5 * 1000000);
  }
}

// Starts up the system
void initialize_system(){
  printf("Initializing system\n");
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

  srand(time(NULL));

  config_t band_conf_0 = get_config(0);
  config_t band_conf_1 = get_config(1);
  config_t band_conf_2 = get_config(2);

  //write constants to file
  sprintf(buffer1, "%s\n%s %d\n%s\n%s %d\n%s\n%s %d",
          scheduler_names[band_conf_0.bandScheduler], control_names[band_conf_0.bandAlgorithm], band_conf_0.bandParameter,
          scheduler_names[band_conf_1.bandScheduler], control_names[band_conf_1.bandAlgorithm], band_conf_1.bandParameter,
          scheduler_names[band_conf_2.bandScheduler], control_names[band_conf_2.bandAlgorithm], band_conf_2.bandParameter);
  sprintf(buffer2, "../../gui/data/algorithms.txt");
  write_file(buffer2, buffer1);

  //BAND 0

  params_0 = malloc(sizeof(params_t));
  params_0->id = 0;
  params_0->side_id = 0;
  params_0->quantum = band_conf_0.bandQuantum;
  params_0->type = band_conf_0.bandScheduler;
  params_0->control = band_conf_0.bandAlgorithm;
  params_0->running = true;
  init_controller(&ctrls[0], lists[0], lists[3], params_0->control, band_conf_0.bandParameter);

  //BAND 1
  params_1 = malloc(sizeof(params_t));
  params_1->id = 1;
  params_1->side_id = 1;
  params_1->quantum = band_conf_1.bandQuantum;
  params_1->type = band_conf_1.bandScheduler;
  params_1->control = band_conf_1.bandAlgorithm;
  params_1->running = true;
  init_controller(&ctrls[1], lists[1], lists[4], params_1->control, band_conf_1.bandParameter);

  params_2 = malloc(sizeof(params_t));
  params_2->id = 2;
  params_2->side_id = 2;
  params_2->quantum = band_conf_2.bandQuantum;
  params_2->type = band_conf_2.bandScheduler;
  params_2->control = band_conf_2.bandAlgorithm;
  params_2->running = true;

  init_controller(&ctrls[2], lists[2], lists[5], params_2->control, band_conf_2.bandParameter);
}


// Initialices the required threads
void initialize_threads(){

  lpthread_t t_gui;
  if(Lthread_create(&t_gui, NULL, &create_gui, NULL) != 0) printf("\nCould not created Thread GUI\n");
  lpthread_t t_pause;
  if(Lthread_create(&t_pause, NULL, &toggle_pause, NULL) != 0) printf("\nCould not created Thread Pause\n");
  lpthread_t t_pause_hardware;
  if(Lthread_create(&t_pause_hardware, NULL, &toggle_pause_buttons, NULL) != 0) printf("\nCould not created Thread Pause Hardware\n");
  lpthread_t t_id_0;
  if(Lthread_create(&t_id_0, NULL, &process_packages, (void *) params_0) != 0) printf("\nCould not created Thread 0\n");
  lpthread_t t_id_1;
  if(Lthread_create(&t_id_1, NULL, &process_packages, (void *) params_1) != 0) printf("\nCould not created Thread 1\n");
  lpthread_t t_id_2;
   if(Lthread_create(&t_id_2, NULL, &process_packages, (void *) params_2) != 0) printf("\nCould not created Thread 2\n");



  Lthread_join(t_id_0, NULL);
  Lthread_join(t_id_1, NULL);
  Lthread_join(t_id_2, NULL);
  Lthread_join(t_pause_hardware, NULL);
  Lthread_join(t_gui, NULL);
}


/*
 * GUI FUNCTIONS
 */

// Writes to a file
void write_file(const char* filename, const char* msg){
  Lmutex_lock(&mutex_file);
  sprintf(command, "echo '%s' > '%s'", msg, filename);
  system(command);
  Lmutex_unlock(&mutex_file);
}

// returns the first packages from a band in each side
char* get_first_packages(int thread_id){
  //structure id:priority,id:priority...
  //initialize buffer
  sprintf(buffer3, "%s","");
  int lenght_l = get_length(*lists[thread_id]);
  int lenght_r = get_length(*lists[thread_id + NUMBER_BANDS]);
  int limit_l, limit_r, i = 0;
  if(lenght_l >= PACKAGES_TO_SHOW) limit_l = PACKAGES_TO_SHOW;
  else limit_l = lenght_l;
  if(lenght_r >= PACKAGES_TO_SHOW) limit_r = PACKAGES_TO_SHOW;
  else limit_r = lenght_r;

  //left list
  if(lenght_l > 0){
    for(; i < limit_l; ++i){
      sprintf(buffer3, "%s%d:%d,", buffer3,get_at(*lists[thread_id], i)->id,  get_at(*lists[thread_id], i)->priority);
    }
  }
  for(; i < (PACKAGES_TO_SHOW); ++i){
    sprintf(buffer3, "%s-:-,", buffer3);
  }
  //division
  sprintf(buffer3, "%sRemaining: %d\n", buffer3, lenght_l);
  //right list
  i = 0;
  if(lenght_r > 0){
    for(; i < limit_r; ++i){
      sprintf(buffer3, "%s%d:%d,", buffer3,get_at(*lists[thread_id+NUMBER_BANDS], i)->id,  get_at(*lists[thread_id+NUMBER_BANDS], i)->priority);
    }
  }
  for(; i < (PACKAGES_TO_SHOW); ++i){
    sprintf(buffer3, "%s-:-,", buffer3);
  }
  //printf("%s\n",buffer3);
  sprintf(buffer3, "%sRemaining: %d", buffer3, lenght_r);
  return buffer3;
}

// Writes the progress of a band into a file 
void write_progress(int thread_id, int list_id, short side){
  //write progress to file
  int packet_priority = get_at(*lists[list_id], 0)->priority;
  int band_sign = side;
  int progress = get_at(*lists[list_id], 0)->progress;
  int packet_id = get_at(*lists[list_id], 0)->id;
  int band = thread_id;
  sprintf(buffer1, "%d\n%d\n%d\n%d\n%s", packet_id,
          progress, packet_priority, side,
          get_first_packages(thread_id));
  sprintf(buffer2, "../../gui/data/band_%d.txt", band);
  write_file(buffer2, buffer1);
}

// Updates the progress for a given band
void update_progress(package_t* pack, enum scheduler_type type){
  //printf("%s\n", "updating");
  if(type == ROUND_ROBIN || type == RTOS) pack->current_execution_time = get_used_time(pack) + pack->accum_execution_time;
  else pack->current_execution_time = get_used_time(pack);

  pack->progress = (short) (pack->speed * pack->current_execution_time);
  pack->remaining_time = pack->total_execution_time - pack->current_execution_time;

  //printf("Progress of package: %d is %d. Remaining time: %fs/%fs\n", pack->id, pack->progress, pack->remaining_time, pack->total_execution_time);
}


/*
 * AUX FUNCTIONS
 */

// Wait function for a second amount

void wait_seconds(double seconds){
  clock_t start;
  start = clock();
  double elapsed_time = 0;
  while(elapsed_time < seconds){
    elapsed_time = ((double) (clock() - start)) / CLOCKS_PER_SEC ;
  }
}
