// list/list.c
// 
// Implementation for linked list.
//
// <Author>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

list_t *list_alloc() { 
  list_t* mylist =  (list_t *) malloc(sizeof(list_t));
  if (mylist != NULL) {
    mylist->head = NULL;
  }
  return mylist;
}

void list_free(list_t *l) {
  if (l != NULL) {
    node_t *current = l->head;
    while (current != NULL) {
      node_t *next = current->next;
      free(current);
      current = next;
    }
    free(l);
   }
}

void list_print(list_t *l) {
  if (l == NULL || l->head == NULL) {
    printf("List is empty.\n");
    return;
  }
  node_t *current = l->head;
  while (current != NULL) {
    printf("%d -> ", current->value);
    current = current->next;
  }
  printf("NULL\n");
}

char * listToString(list_t *l) {
  if (l == NULL || l->head == NULL) {
    char *emptyStr = (char *)malloc(sizeof(char) * 5);
    strcpy(emptyStr, "NULL");
    return emptyStr;
  }
  char* buf = (char *) malloc(sizeof(char) * 1024);
  char tbuf[20];
  buf[0] = '\0';

	node_t* curr = l->head;
  while (curr != NULL) {
    sprintf(tbuf, "%d->", curr->value);
    curr = curr->next;
    strcat(buf, tbuf);
  }
  strcat(buf, "NULL");
  return buf;
}

int list_length(list_t *l) {
  if (l == NULL) {
    return 0;
    }

  int length = 0;
  node_t *current = l->head;
  while (current != NULL) {
    length++;
    current = current->next;
  }
  return length;
  
 }

void list_add_to_back(list_t *l, elem value) {
  if (l == NULL) {
    return;
  }

  node_t *new_node = (node_t *)malloc(sizeof(node_t));
  if (new_node == NULL) {
    fprintf(stderr, "Memory allocation failed.\n");
    exit(EXIT_FAILURE);
  }
  new_node->value = value;
  new_node->next = NULL;

  if (l->head == NULL) {
    l->head = new_node;
    } 
  else {
    node_t *current = l->head;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = new_node;
    }
}

void list_add_to_front(list_t *l, elem value) {
  if (l == NULL) {
    return;
  }

  node_t *new_node = (node_t *)malloc(sizeof(node_t));
  if (new_node == NULL) {
      fprintf(stderr, "Memory allocation failed.\n");
      exit(EXIT_FAILURE);
  }
    new_node->value = value;
    new_node->next = l->head;
    l->head = new_node;

}
void list_add_at_index(list_t *l, elem value, int index) {
  if (l == NULL || index < 1) {
    return;
  }
  if (index == 1) {
    list_add_to_front(l, value);
    return;
  }
  int current_index = 1;
  node_t *current = l->head;
  while (current != NULL) {
    if (current_index + 1 == index) {
      node_t *new_node = (node_t *)malloc(sizeof(node_t));
      if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
      }
      new_node->value = value;
      new_node->next = current->next;
      current->next = new_node;
      return;
    }
    current = current->next;
    current_index++;
  }
}

elem list_remove_from_back(list_t *l) { 
  if (l == NULL || l->head == NULL) {
    return -1; 
  }

  if (l->head->next == NULL) {
    elem value = l->head->value;
    free(l->head);
    l->head = NULL;
    return value;
  }

  node_t *current = l->head;
  while (current->next->next != NULL) {
    current = current->next;
  }

  elem value = current->next->value;
  free(current->next);
  current->next = NULL;
  return value;
  }
elem list_remove_from_front(list_t *l) { 
  if (l == NULL || l->head == NULL) {
    return -1; 
  }
  elem value = l->head->value;
  node_t *temp = l->head;
  l->head = l->head->next;
  free(temp);
  return value;
  }
elem list_remove_at_index(list_t *l, int index) { 
  if (l == NULL || l->head == NULL || index < 1) {
    return -1; 
  }
  if (index == 1) {
    return list_remove_from_front(l);
  }

  int current_index = 1;
  node_t *current = l->head;
  while (current != NULL) {
    if (current_index + 1 == index && current->next != NULL) {
      node_t *temp = current->next;
      elem value = temp->value;
      current->next = temp->next;
      free(temp);
      return value;
    }
    current = current->next;
    current_index++;
  }

    return -1;
      
  }

bool list_is_in(list_t *l, elem value) { 
  if (l == NULL || l->head == NULL) {
    return false; 
  }
  node_t *current = l->head;
  while (current != NULL) {
    if (current->value == value) {
      return true; 
    }
    current = current->next;
  }
  return false;
  
 }
elem list_get_elem_at(list_t *l, int index) { 
  if (l == NULL || l->head == NULL || index < 1) {
    return -1; 
  }
  int current_index = 1;
  node_t *current = l->head;
  while (current != NULL) {
    if (current_index == index) {
      return current->value;
    }
    current = current->next;
    current_index++;
  }
  return -1; 
  }
int list_get_index_of(list_t *l, elem value) { 
  if (l == NULL || l->head == NULL) {
    return -1; 
  }
  int current_index = 1;
  node_t *current = l->head;
  while (current != NULL) {
    if (current->value == value) {
      return current_index;
    }
    current = current->next;
    current_index++;
  }
  
  return -1; 
}

