#include "../include/linked_list.h"
#include "../include/schedulers.h"

const double QUANTUM = 2;

int main() {
  printf("Hello, World!\n");

  Node_t * head = NULL;

  package_t packages[6];

  for(int i = 0; i < 5; ++i){
    packages[i].id = i;
    push_back(&head, &packages[i]);
  }

  print_list(head);

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
  schedule_priority(head);
  print_list(head);
  //3 1 2 4 0

  printf("Schedule by shortest first\n");
  schedule_shortest_first(head);
  print_list(head);
  //2 3 0 1 4

  printf("Schedule by 'real time'\n");
  schedule_real_time(head,50);
  print_list(head);
  //2 1 4 0 3



//
//  pop_back(head);
//
//  printf("%d\n", get_length(head));
//  push_back(&head, &packages[0]);
//  push_front(&head, &packages[1]);
//  push_back(&head, &packages[1]);
//  print_list(head);
//  //printf("Value removed: %d \n",remove_at(&head, 2)->id);
//  push_front(&head, &packages[2]);
//  push_front(&head, &packages[3]);
//  printf("%d\n", get_length(head));
//  print_list(head);
//  printf("Get Value: %d\n",get_at(head, 0)->id);
//  printf("Get Value: %d\n",get_at(head, 1)->id);
//  printf("Get Value: %d\n",get_at(head, 2)->id);
//  printf("Get Value: %d\n",get_at(head, 3)->id);
//  printf("Get Value: %d\n",get_at(head, 4)->id);
//  print_list(head);
//  printf("Set Value: %d\n",set_at(head, 0, &packages[4])->id);
//  printf("Set Value: %d\n",set_at(head, 1, &packages[4])->id);
//  printf("Set Value: %d\n",set_at(head, 2, &packages[4])->id);
//  printf("Set Value: %d\n",set_at(head, 3, &packages[4])->id);
//  printf("Set Value: %d\n",set_at(head, 4, &packages[3])->id);
//  set_at(head, 5, &packages[4]);
//  print_list(head);
//  swap(head, 3, 4);
//  print_list(head);


  return 0;
}