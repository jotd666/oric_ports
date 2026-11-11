#ifndef KEY_MAPPING_H
#define KEY_MAPPING_H
#include "oric_input.h"

typedef struct
{
     KeyMapping keys[100];
    int nb_mapping;
}  KeyboardMapping;

void init_key_mapping();
int save_key_mapping();
int load_key_mapping();

KeyboardMapping *get_key_mapping();

#endif
