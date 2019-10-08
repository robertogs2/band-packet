#include "../include/linked_list.h"

int main() {
  printf("Hello, World!\n");

  Node_t * head = NULL;
  package_t packages[5];

  for(int i = 0; i < 5; ++i){
    packages[i].id = i;
  }

  pop_back(head);

  printf("%d\n", get_length(head));
  push_back(&head, &packages[0]);
  push_front(&head, &packages[1]);
  push_back(&head, &packages[1]);
  print_list(head);
  //printf("Value removed: %d \n",remove_at(&head, 2)->id);
  push_front(&head, &packages[2]);
  push_front(&head, &packages[3]);
  printf("%d\n", get_length(head));
  print_list(head);
  printf("Get Value: %d\n",get_at(head, 0)->id);
  printf("Get Value: %d\n",get_at(head, 1)->id);
  printf("Get Value: %d\n",get_at(head, 2)->id);
  printf("Get Value: %d\n",get_at(head, 3)->id);
  printf("Get Value: %d\n",get_at(head, 4)->id);
  print_list(head);
  printf("Set Value: %d\n",set_at(head, 0, &packages[4])->id);
  printf("Set Value: %d\n",set_at(head, 1, &packages[4])->id);
  printf("Set Value: %d\n",set_at(head, 2, &packages[4])->id);
  printf("Set Value: %d\n",set_at(head, 3, &packages[4])->id);
  printf("Set Value: %d\n",set_at(head, 4, &packages[3])->id);
  set_at(head, 5, &packages[4]);
  print_list(head);
  swap(head, 3, 4);
  print_list(head);


  return 0;
}