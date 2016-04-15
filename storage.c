// Copyright 2016 jsaak
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "storage.h"

void storage_init(struct storage_info_t* info, void* heap, uint16_t struct_size, uint16_t max_elements) {
  info->heap = heap;
  info->struct_size = struct_size;
  info->max_elements = max_elements;
  info->counter = 0;
  info->first_data = 0;
  info->last_data = 0;

  uint16_t i;
  for (i=0; i<max_elements; i++) {
    struct storage_node_t* prev =    (struct storage_node_t*)((uint8_t*)heap+(struct_size*(i-1)));
    struct storage_node_t* current = (struct storage_node_t*)((uint8_t*)heap+(struct_size*(i)));
    struct storage_node_t* next =    (struct storage_node_t*)((uint8_t*)heap+(struct_size*(i+1)));
    if (i+1 == max_elements) {  //last
      current->prev = prev;
      current->next = 0;
      info->last_empty = current;
    } else if (i==0) {  //first
      current->prev = 0;
      current->next = next;
      info->first_empty = current;
    } else {
      current->prev = prev;
      current->next = next;
    }
  }
}

static void storage_insert(struct storage_info_t* info, void* new_node, int front) {
  if (info->counter == info->max_elements) {
    return;
  } else {
    info->counter++;
    struct storage_node_t * node_to_memcpy = info->first_empty;

    if (info->first_data == 0) {
      //first data
      info->first_data = info->first_empty;
      info->last_data = info->first_empty;
      info->first_empty = info->first_empty->next;
      info->first_empty->prev = 0;
      info->first_data->next = 0;
      info->first_data->prev = 0;
    } else {
      struct storage_node_t * temp;

      if (front) {
        temp = info->first_data;
        info->first_data->prev = info->first_empty;
        info->first_data = info->first_empty;
      } else {
        temp = info->last_data;
        info->last_data->next = info->first_empty;
        info->last_data = info->first_empty;
      }

      if (info->counter == info->max_elements) {
        //last data
        info->first_empty = 0;
        info->last_empty = 0;
      } else {
        //middle data
        info->first_empty = info->first_empty->next;
        info->first_empty->prev = 0;
      }

      if (front) {
        info->first_data->prev = 0;
        info->first_data->next = temp;
      } else {
        info->last_data->prev = temp;
        info->last_data->next = 0;
      }
    }

    //copy the struct except the first two pointer
    memcpy(&node_to_memcpy[1],
        ((uint8_t*)new_node) + 2*sizeof(void*),
        info->struct_size - 2*sizeof(void*));
  }
}


void storage_insert_front(struct storage_info_t* info, void* new_node) {
  storage_insert(info, new_node, 1);
}

void storage_push(struct storage_info_t* info, void* new_node) {
  storage_insert(info, new_node, 0);
}

void storage_remove(struct storage_info_t* info, struct storage_node_t* x) {
  if (info->counter == 0) {
    return;
  } else {
    //remove from data
    if (x->prev == 0) { //first
      info->first_data = x->next;
    } else {
      ((struct storage_node_t*)(x->prev))->next = x->next;
    }

    if (x->next == 0) { //last
      info->last_data = x->prev;
    } else {
      ((struct storage_node_t*)(x->next))->prev = x->prev;
    }

    //add to empty
    if (info->last_empty == 0) {
      info->last_empty = x;
      info->first_empty = x;
      x->next = 0;
      x->prev = 0;
    } else {
      info->last_empty->next = x;
      x->prev = info->last_empty;
      x->next = 0;
      info->last_empty = x;
    }
    info->counter--;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////
// $ gcc storage.c -DSTORAGE_TEST
// $ ./a.out
//
//

#ifdef STORAGE_TEST

#include <stdio.h>
#define DD_SIZE 4

typedef struct mqttsn_node_t {
  struct mqttsn_node_t* next;  //always the first because of how storage works
  struct mqttsn_node_t* prev;
  uint8_t data[30];
  uint16_t length;
  uint16_t dst;
} mqttsn_node_t;

void storage_inspect(struct storage_info_t* info) {
  int i;
  printf("counter: %02d fd: 0x%06x  ld: 0x%06x  fe: 0x%06x  le: 0x%06x\n",
      info->counter,info->first_data,info->last_data,info->first_empty,info->last_empty);

  for(i=0; i<info->max_elements; i++) {
    struct mqttsn_node_t* current = (struct mqttsn_node_t*)((uint8_t*)info->heap+(info->struct_size*(i)));
    printf("  loc: 0x%06x  prev: 0x%06x  next: 0x%06x  data: %s\n",
        current,
        current->prev,
        current->next,
        current->data);
  }
}

static struct mqttsn_node_t heap[DD_SIZE];
static struct storage_info_t info;

void storage_test() {
  storage_init(&info,&heap,sizeof(mqttsn_node_t),DD_SIZE);
  storage_inspect(&info);

  struct mqttsn_node_t temp;

  memcpy(temp.data,"Hello 1",8);
  storage_push(&info,&temp);
  storage_inspect(&info);

  memcpy(temp.data,"Hello 2",8);
  storage_push(&info,&temp);
  storage_inspect(&info);

  memcpy(temp.data,"Hello 3",8);
  storage_insert_front(&info,&temp);
  storage_inspect(&info);

  memcpy(temp.data,"Hello 4",8);
  storage_insert_front(&info,&temp);
  storage_inspect(&info);
/*
  struct mqttsn_node_t* storage_heap = (struct mqttsn_node_t*) info.heap;

  storage_remove(&info, (struct storage_node_t *) &storage_heap[1]);
  memcpy(&storage_heap[1].data,"-------",8);
  storage_inspect(&info);

  storage_remove(&info, (struct storage_node_t *) &storage_heap[2]);
  memcpy(&storage_heap[2].data,"-------",8);
  storage_inspect(&info);

  memcpy(temp.data,"Hello 4",8);
  storage_push(&info,&temp);
  storage_inspect(&info);

  memcpy(temp.data,"Hello 5",8);
  storage_push(&info,&temp);
  storage_inspect(&info);

  memcpy(temp.data,"Hello 6",8);
  storage_push(&info,&temp);
  storage_inspect(&info);

  //storage_remove(&info, (struct storage_node_t *) &storage_heap[2]);
  //memcpy(&storage_heap[2].data,"-------",8);
  //storage_inspect(&info);
*/
}

int main(int argc, char** argv) {
  storage_test();
}
#endif
