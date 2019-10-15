//
// Created by lionheart on 8/10/19.
//

#include "../include/linked_list.h"


void print_list(Node_t *head, int property) {
  Node_t * current = head;
  printf("[");
  while(current != NULL){
    if(property == 0)
      printf("%d ", current->value->id);
    else if(property == 1)
      printf("%d ", current->value->priority);
    else if(property == 2)
      printf("%f ", current->value->remaining_time);
    current = current->next;
  }
  printf("]\n");
}

void push_back(Node_t **head, package_t * value) {
  //if list is empty
  Node_t * current = *head;
  if (current == NULL){
    push_front(head, value);
    return;
  }
  //go to end of list
  while(current->next != NULL){
    current = current->next;
  }
  //add new element
  current->next = malloc(sizeof(Node_t));
  current->next->value =  value;
  current->next->next = NULL;
}

void push_front(Node_t **head, package_t * value) {
  Node_t * new_node;
  //allocates memory for new node
  new_node = malloc(sizeof(Node_t));
  new_node->value =  value;
  new_node->next = *head;
  //assigns new head
  *head = new_node;
}

package_t * pop_front(Node_t **head) {
  package_t * retval = NULL;
  Node_t * new_head = NULL;

  //if list is empty
  if(*head == NULL){
    return retval;
  }

  new_head = (*head)->next;
  retval = (*head)->value;
  free(*head);
  *head = new_head;

  return retval;
}

package_t* pop_back(Node_t *head) {
  package_t * retval = NULL;

  if(get_length(head) == 0){
    printf("List is already empty. Cannot pop-back.\n");
    return retval;
  }

  //If there is only one item in the list
  if(head->next == NULL){
    retval = head->value;
    free(head);
    return retval;
  }

  //Go to the second last node
  Node_t * current = head;
  while(current->next->next != NULL){
    current = current->next;
  }

  retval = current->next->value;
  free(current->next);
  current->next = NULL;
  return retval;
}

package_t *remove_at(Node_t **head, int index) {
  if(get_length(*head) <= index || index < 0){
    printf("Cannot remove, index out of range\n");
    return NULL;
  }
  int i = 0;
  package_t * retval = NULL;

  Node_t * current = *head;
  Node_t * temp = NULL;

  //first element
  if(index == 0){
    return pop_front(head);
  }

  for(i = 0; i < index - 1; ++i){
    //the index is out of range
    if(current->next == NULL){
      printf("Index out of range");
      return NULL;
    }
    current = current->next;
  }

  temp = current->next;
  retval = temp->value;
  current->next = temp->next;
  free(temp);

  return retval;
}

int get_length(Node_t *head) {
  Node_t * current = head;
  int retval = 0;
  while(current != NULL){
    current = current->next;
    retval ++;
  }
  return retval;
}

package_t* get_at(Node_t *head, int index) {
  package_t * retval = NULL;

  if(get_length(head) <= index || index < 0){
    printf("Index out of range. Cannot get at %d.\n", index);
    return retval;
  }

  Node_t * current = head;

  for(int i = 0; i < index; ++i){
    current = current->next;
  }
  retval = current->value;

  return retval;
}

package_t *set_at(Node_t *head,  int index, package_t *value) {
  package_t * retval = NULL;

  if(get_length(head) <= index || index < 0){
    printf("Index out of range. Cannot set at %d.\n", index);
    return retval;
  }

  Node_t * current = head;

  for(int i = 0; i < index; ++i){
    current = current->next;
  }
  current->value = value;
  retval = current->value;

  return retval;
}

void swap(Node_t *head, int index1, int index2) {
  int length = get_length(head);
  if(index1 >= length || index1 < 0 || index2 >= length || index2 < 0){
    printf("Index out of range. Cannot swap %d and %d.\n", index1, index2);
    return;
  }

  package_t * temp;
  temp = get_at(head, index1);
  set_at(head, index1, get_at(head,index2));
  set_at(head,index2, temp);

}

package_t *get_by_id(Node_t *head, int id) {
  int length = get_length(head);
  Node_t * current = head;

  for(int i = 0; i < length; ++i){
    if(current->value->id == id) return current->value;
    current = current->next;
  }
  printf("This id is not in the list\n");
  return NULL;
}
