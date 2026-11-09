#include "oric_machine.h"
#include "oric_basic.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>



#define POOL_NB_STRINGS 50

static int pool_index=0;
static T_STRING strings_pool[POOL_NB_STRINGS];

T_STRING_PTR alloc_string()
{
  T_STRING_PTR rval = 0;


  rval = strings_pool[pool_index];
  rval[0] = 0;
  pool_index = (pool_index + 1) % POOL_NB_STRINGS;
  return rval;
}

void gc_strings()
{
  pool_index = 0;
  strings_pool[pool_index][0] = 0;
}



static int data_position = 0;

void restore()
{
   data_position = 0;
}
T_INT integer(T_NUMERIC n)
{
  return (T_INT)n;
}

T_STRING_PTR lowercase(const T_STRING_PTR s)
{
  T_STRING_PTR rval = alloc_string();
  int i=0;
  while(s[i]!=0)
    {
      rval[i] = tolower(s[i]);
      i++;
    }
  rval[i] = 0;
  return rval;
}

T_STRING_PTR uppercase(const T_STRING_PTR s)
{
  T_STRING_PTR rval = alloc_string();
  int i=0;
  while(s[i]!=0)
    {
      rval[i] = toupper(s[i]);
      i++;
    }
  rval[i] = 0;
  return rval;
}

void strcopy(T_STRING_PTR s1,const T_STRING_PTR s2)
{
  strcpy(s1,s2);
}

int streq(const T_STRING_PTR s1,const T_STRING_PTR s2)
{
   return !strcmp(s1,s2);
}

int strneq(const T_STRING_PTR s1,const T_STRING_PTR s2)
{
  return strcmp(s1,s2);
}

T_STRING_PTR spc(T_INT nb)
{
  T_STRING_PTR rval = alloc_string();
  my_assert(nb<(int)sizeof(T_STRING)-1);
  memset(rval,' ',nb);
  rval[nb]=0;
  return rval;
}
T_INT asc(T_STRING_PTR s)
{
  return (T_INT)s[0];
  }

T_STRING_PTR left$(T_STRING_PTR value,T_INT size)
{
  T_STRING_PTR rval = alloc_string();
  if (size>(T_INT)strlen(value))
    {
      size = (int)strlen(value);
    }

  strncpy(rval,value,size);
  rval[size]=0;
  return rval;
}
T_STRING_PTR chr$(T_INT value)
{
  T_STRING_PTR rval = alloc_string();
  rval[0] = (char)value;
  rval[1] = 0;

  return rval;
}

T_STRING_PTR right$(T_STRING_PTR value,T_INT size)
{
  T_STRING_PTR rval = alloc_string();
  int l = strlen(value);
  int start_offset = l-size;
  if (start_offset<0)
    {
      start_offset = 0;
    }

  strncpy(rval,value+start_offset,l-start_offset);
  rval[l-start_offset]=0;
  return rval;
}
T_STRING_PTR mid$(T_STRING_PTR value,T_INT start_offset,T_INT length)
{
  T_STRING_PTR rval = alloc_string();
  T_INT vlen = (int)strlen(value);
  if (start_offset<1)
    {
      error("MID$",": ",ILLEGAL_QUANTITY," start_offset = %d < 1",str$(start_offset));
    }


  if (length-start_offset+1>=vlen)
    {
      length = (int)strlen(value)-start_offset+1;
    }
  if (start_offset>vlen)
    {
      length=0;
    }

  strncpy(rval,value+start_offset-1,length);
  rval[length] = 0;
  return rval;
}
T_STRING_PTR mid2$(T_STRING_PTR value,T_INT start_offset)
{
  return mid$(value,start_offset,(int)strlen(value)-start_offset);
}

T_STRING_PTR hex$(T_INT value)
{
  T_STRING_PTR rval = alloc_string();
  if (value<0)
    {
      error(BAD_SUBSCRIPT,"HEX$",NULL);
    }

  sprintf(rval,"#%X",value);
  return rval;
}

T_STRING_PTR str$(T_NUMERIC value)
{
  // adds 1 space in the beginning. Forgetting to emulate this
  // results in very difficult bug hunting (Return of Dr Genius...)
  // => gs.T = peek(0x80);strcopy(gs.str_MO,mid$(str$(gs.T),2,1));
  T_STRING_PTR rval = alloc_string();
  if (value==(int)value)
    {
      sprintf(rval," %d",(int)value);
    }
  else
    {
      sprintf(rval," %f",value);
    }
  return rval;
}

T_NUMERIC val(const T_STRING_PTR txt)
{
  T_NUMERIC rval = 0;
  if (txt[0]=='#')
    {
      int ival=0;
      sscanf(txt+1,"%x",&ival);
      rval = ival;
    }
  else
    {
      sscanf(txt,"%f",&rval);
    }

   return rval;
}

T_INT sgn(T_NUMERIC value)
{
  return value==0 ? 0 : (value < 0 ? -1 : 1);
}

void doke(T_INT address, T_INT value)
{
    poke(address,value & 0xFF);
    poke(address+1,value >> 8);
}
void poke(T_INT address, T_INT value)
{
  if (value>255)
    {
      error("POKE: ",ILLEGAL_QUANTITY," (value) ",hex$(address),str$(value),0);

    }
  if (address<0 || address>0xFFFF)
    {
       error("POKE: ",ILLEGAL_QUANTITY," (address) ",hex$(address),str$(value),0);

    }
  if (address>0xBFFF)
    {
       error("POKE: ",ILLEGAL_QUANTITY," (ROM address) ",hex$(address),str$(value),0);

    }

    cpu_write_mem(value,address);
}
T_BYTE peek(T_INT address)
{
  if (address<0 || address>0xFFFF)
    {
       error("PEEK: ",ILLEGAL_QUANTITY," (address) ",hex$(address),0);

    }
  if (address>0xBFFF)
    {
       error("PEEK: ",ILLEGAL_QUANTITY," (ROM address) ",hex$(address),0);
    }
    return cpu_read_mem(address);
}

T_INT deek(T_INT address)
{
    return cpu_read_mem(address) + (((T_INT) cpu_read_mem(address+1)) << 8);
}

void end()
{
  SDL_Quit();
  exit(0);
}

T_INT strlength(const T_STRING_PTR s)
{
  return (T_INT)strlen(s);
}


T_STRING_PTR strconcat(const T_STRING_PTR s1,...)
{
  T_STRING_PTR rval = alloc_string();
  unsigned int lr = (int)strlen(s1);
  strcat(rval,s1);
  va_list ap;
  va_start(ap,s1);
  while  (1)
    {
      T_STRING_PTR na = va_arg(ap,T_STRING_PTR);
      if (na==0)
	{
	  break;
	}
      lr += strlen(na);
      if (lr > sizeof(T_STRING))
	{
	  error("string overflow",0);
	}

      strcat(rval,na);
    }

  va_end(ap);
  return rval;
}

void data_read(const T_STRING_PTR *items,int total_nb_items,int nb_items,...)
{
  int i;
  const T_STRING_PTR *item_ptr = items+data_position;
  va_list ap;
  va_start(ap,nb_items);

  for (i=0;i<nb_items;i++)
    {
      if (data_position>=total_nb_items)
	{
	  error("OUT OF DATA ERROR: POS = ",str$(data_position),0);
	}

      int dt = va_arg(ap,int);
      switch(dt)
	{
	  case DT_INT:
	    {
	      T_INT *na = va_arg(ap,T_INT *);
	      *na = atoi(*item_ptr);
	      break;
	    }
	  case DT_NUMERIC:
	    {
	      T_NUMERIC *na = va_arg(ap,T_NUMERIC *);
	      *na = atof(*item_ptr);
	      break;
	    }
	  case DT_STRING:
	    {
	      T_STRING_PTR na = va_arg(ap,T_STRING_PTR);
	      strcpy(na,*item_ptr);
	      break;
	    }
	}
      item_ptr++;
      data_position++;

    }


}

void error(const T_STRING_PTR message,...)
{
  T_STRING tmp,msg;
  va_list ap;
  va_start(ap,message);
  sprintf(msg,"error: %s",message);

  while(1)
    {
      T_STRING_PTR na = va_arg(ap,T_STRING_PTR);
      if(na==0)
	{
	  break;
	}
      else
	{
	  sprintf(tmp,"%s",na);
	  strcat(msg,tmp);

	}
      va_end(ap);

    }
  print_string("? ");
  print_string(msg);
  print_string("\n");
  fatal_error(msg);

}


T_NUMERIC rnd(T_INT seed)
{
  static T_NUMERIC old_rnd_value = 0;

  if (seed)
    {
      old_rnd_value = (rand()/(float)RAND_MAX);
    }

    return old_rnd_value;

  }


void oric_wait(T_INT centis)
{
    SDL_Delay(centis*10);
}
