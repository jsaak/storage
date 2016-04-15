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

#include <stdio.h>
#define DD_SIZE 4

typedef struct PACK mqttsn_node_t {
  struct mqttsn_node_t* next;  //next should be always the first 
  struct mqttsn_node_t* prev;  //prev should be always the second
  uint8_t data[30];            //the rest is up to you
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

  struct mqttsn_node_t* storage_heap = (struct mqttsn_node_t*) info.heap;

  storage_remove(&info, (struct storage_node_t *) &storage_heap[1]);
  memcpy(&storage_heap[1].data,"-------",8);
  storage_inspect(&info);

  storage_remove(&info, (struct storage_node_t *) &storage_heap[2]);
  memcpy(&storage_heap[2].data,"-------",8);
  storage_inspect(&info);

  memcpy(temp.data,"Hello 5",8);
  storage_push(&info,&temp);
  storage_inspect(&info);

  memcpy(temp.data,"Hello 6",8);
  storage_push(&info,&temp);
  storage_inspect(&info);

  memcpy(temp.data,"Hello 7",8);
  storage_push(&info,&temp);
  storage_inspect(&info);

  //storage_remove(&info, (struct storage_node_t *) &storage_heap[2]);
  //memcpy(&storage_heap[2].data,"-------",8);
  //storage_inspect(&info);
}

int main(int argc, char** argv) {
  storage_test();
}
