#include "key_mapping.h"

#include <stdio.h>

static const char *kmn = "keymapping";
static KeyboardMapping km;

KeyboardMapping *get_key_mapping()
{
  return &km;
}

void init_mapping()
{
  memset(&km,0,sizeof(km));
}

int load_key_mapping()
{
    FILE *f = fopen(kmn,"rb");
    if (f != 0)
    {
      fread(&km,sizeof(km),1,f);
      fclose(f);
      if (km.nb_mapping>0)
	{
	  set_key_mapping(km.keys,km.nb_mapping);
	}

    }
    return (f != 0);
}
int save_key_mapping()
{
    FILE *f = fopen(kmn,"wb");
    if (f != 0)
    {
      fwrite(&km,sizeof(km),1,f);

      fclose(f);
    }
    return (f != 0);

}
