#ifndef _STORAGE_H_
#define _STORAGE_H_

#include <stdint.h>
#include <string.h>

//some environments define PACK macro to avoid struct member alignments
//for Atmel PACK macro
//#include "sysTypes.h"

#ifndef PACK
#define PACK __attribute__ ((packed))
#endif

//fake node type to be able to use ->next and ->prev
typedef struct PACK storage_node_t {
  void* next;
  void* prev;
} storage_node_t;

typedef struct storage_info_t {
  void* heap;
  uint8_t struct_size;
  uint16_t max_elements;
  uint16_t counter;
  struct storage_node_t* first_empty;
  struct storage_node_t* first_data;
  struct storage_node_t* last_empty;
  struct storage_node_t* last_data;
} storage_info_t;

void storage_init(struct storage_info_t* info, void* heap_pointer, uint16_t struct_size, uint16_t max_elements);
void storage_push(struct storage_info_t* info, void* new_node);
void storage_insert_front(struct storage_info_t* info, void* new_node);
void storage_remove(struct storage_info_t* info, struct storage_node_t* x);
void storage_inspect(struct storage_info_t* info);

#endif // _STORAGE_H_
