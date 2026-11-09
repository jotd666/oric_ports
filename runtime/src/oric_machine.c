#include "oric_machine.h"
#include "oric_video.h"
#include "oric_basic.h"
#include "charset.h"
#include <strings.h>
#include <stdio.h>

#define NB_ITEMS_IN_STACK 0x100




#ifndef NDEBUG
static int turnoff_check_count = 0;
static int debug_level = 0;

static unsigned char code_shadow_memory[0xC000];

const unsigned char *cpu_get_charset()
{
    return charset;
}


void cpu_turnoff_checks(int nb_instructions)
{
  turnoff_check_count = nb_instructions;
}

void check_in_ram(int offset)
{
  if ((offset < 0) || (offset >= 0xC000))
    {
      print_pc_history();
      error("Address ",hex$(offset)," out of RAM PC=",hex$(r.pc)," A=",hex$(r.a)," X=",hex$(r.x),
	    " Y=",hex$(r.y),NULL);
    }
}


void cpu_push_byte(unsigned char b)
{
    my_assert (r.s > 0);

    r.s--;
    stack_start[r.s] = b;
}

unsigned char cpu_pop_byte()
{
    unsigned char rval = 0;

    my_assert(r.s < 0x100);

    rval = stack_start[r.s];

    r.s++;

    return rval;
}

static inline void cpu_dump_status()
{
  if (debug_level & DEBUG_PRINT_INSTRUCTION)
    {
      printf("PC=%04X, A=%02X, X=%02X, Y=%02X, S=1%02X, N=%d, V=%d, B=%d, D=%d, I=%d, Z=%d, C=%d\n",
	     r.pc,r.a,r.x,r.y,r.s,r.n,r.v,r.b,r.d,r.i,r.z,r.c);
    }
}

typedef struct
{
    int address;
    int size;
} MemWatch;

#define MAX_MEMWATCH 50

int memwatch_read_index = 0;
int memwatch_write_index = 0;
MemWatch memwatch_read_array[MAX_MEMWATCH];
MemWatch memwatch_write_array[MAX_MEMWATCH];


static void cpu_memwatch_lookup(const MemWatch *mw,int nb_mw,int offset, int kind, unsigned char value_written);

unsigned char cpu_read_mem(int address)
{
  unsigned char rval;
  check_in_ram(address);
  if (memwatch_read_index > 0)
    {
      cpu_memwatch_lookup(memwatch_read_array,
			  memwatch_read_index,address,MEMWATCH_READ,0);
    }
  rval = m[address];

  if (is_in_io(address))
    {
      address &= 0x30F;  // remove mirroring
      //printf("trying to read from %x\n",address);
      if ((address == VIA_T1CL) || (address == VIA_T2CL))
	{
	  // we consider this as random as we don't really count the cycles

	  rval = cpu_random_8_bit(); //cycle_counter;
	}
    }
  return rval;
}
#ifndef NDEBUG
void cpu_write_mem(unsigned char v, int address)
{

  check_in_ram(address);
  if ((address >= 0x200) || (address < 0x100))
    {
    }
  else
    {
      print_pc_history();
      error("writing to stack area: ",hex$(address),", PC=",hex$(r.pc));
    }


  if (memwatch_write_index > 0)
    {
      cpu_memwatch_lookup(memwatch_write_array,
			  memwatch_write_index,address,
			  MEMWATCH_WRITE,
			  v);
    }
 #endif


   m[address] = v;

}

#endif



void cpu_init()
{


  memset(&r,0,sizeof(r));
  #ifndef NDEBUG
  memset(&code_shadow_memory,0xFF,sizeof(code_shadow_memory));
  #endif
  r.s = 0xFF;
  stack_start = m+0x100;

}




#ifndef NDEBUG
static void cpu_memwatch_reached(int address, int kind, unsigned char value)
{
    if (kind == MEMWATCH_READ)
    {
	printf("memory address $%04X is being read around $%04X\n",address,r.pc);
    }
    else
    {
	printf("memory address $%04X is being written with value %02X around $%04X\n",address,value,r.pc);
    }
    fflush(stdout);
}
static void cpu_memwatch_lookup(const MemWatch *mw,int nb_mw,int offset, int kind, unsigned char value_written)
{
    int i;
    for (i = 0; i < nb_mw; i++)
    {

	if ((mw[i].address <= offset) &&
	    (mw[i].address + mw[i].size) > offset)
	{
	    cpu_memwatch_reached(offset,kind,value_written);
	}
    }

}
void cpu_reset_charset()
{
  void *oric_ram = cpu_get_mem();

  memcpy(oric_ram+0xB500,charset,sizeof(charset));

}

void cpu_memcopy(int source, int dest, int count)
{
  int i;

  for(i=0;i<count;i++)
    {
      cpu_write_mem(cpu_read_mem(source+i),dest+i);
    }

}
void cpu_memset(int dest, int count, unsigned char value)
{
   int i;
  for(i=0;i<count;i++)
    {
      cpu_write_mem(value,dest+i);
    }


}
#else
void cpu_memcopy(int source, int dest, int count)
{

    memcpy(m+dest,m+source,count);

}
void cpu_memset(int dest, int count, unsigned char value)
{


    memset(m+dest,value,count);

}
#endif

#define UPDATE_CARRY_FROM_LEFT(v) r.c = (v & 0x80) != 0;
#define UPDATE_CARRY_FROM_RIGHT(v) r.c = v & 0x1;

#define ASL(v) UPDATE_CARRY_FROM_LEFT(v); v = (v << 1) & 0xff;cpu_update_nz(v)
#define LSR(v) UPDATE_CARRY_FROM_RIGHT(v); v >>= 1;cpu_update_nz(v)
#define ROR(v) int old_c = r.c; UPDATE_CARRY_FROM_RIGHT(v); v = (v >> 1) + (old_c << 7); cpu_update_nz(v)
#define ROL(v) int old_c = r.c; UPDATE_CARRY_FROM_LEFT(v); v = ((v << 1) + old_c) & 0xFF; cpu_update_nz(v)

void cpu_asl_a()
{
    ASL(r.a);
}

void cpu_lsr_a()
{
    LSR(r.a);
}

void cpu_ror_a()
{
    ROR(r.a);
}
void cpu_rol_a()
{
    ROL(r.a);
}

void cpu_asl(int address)
{
    check_in_ram(address);
    my_assert(!is_in_io(address));

    ASL(m[address]);
}

void cpu_lsr(int address)
{
    check_in_ram(address);
    my_assert(!is_in_io(address));

    LSR(m[address]);
}

void cpu_ror(int address)
{
    check_in_ram(address);
    my_assert(!is_in_io(address));

    ROR(m[address]);

}

void cpu_rol(int address)
{
    check_in_ram(address);
    my_assert(!is_in_io(address));

    ROL(m[address]);
}

void cpu_asl_x(int address)
{
    cpu_asl(compute_indexed_address(address,r.x));
}

void cpu_lsr_x(int address)
{
    cpu_lsr(compute_indexed_address(address,r.x));
}
void cpu_ror_x(int address)
{
    cpu_ror(compute_indexed_address(address,r.x));
}

void cpu_rol_x(int address)
{
    cpu_rol(compute_indexed_address(address,r.x));
}


void cpu_cmp_ind_y(int zp_address)
{
    cpu_cmp(r.a,cpu_read_mem(ADDRESS_16(zp_address) + r.y));
}
void cpu_cmp(unsigned char reg, unsigned char v)
{
    /*     LDA #$01 ;  1 (signed),   1 (unsigned)
    CMP #$FF ; -1 (signed), 255 (unsigned)

A = $01, C = 0, N = 0 (the subtraction result is $01 - $FF = $02), and Z = 0.
    */
    int subs = (int)reg - (int)v;

    r.z = (subs == 0);
    r.c = (subs >= 0);

    if (r.z)
    {
	r.n = 0;
    }
    else
    {
	r.n = (subs & 0x80) >> 7; // most significant bit of the result
    }
}



void cpu_lda_ind_x(unsigned char zp_address)
{
    cpu_set_a(cpu_read_mem(ADDRESS_16(zp_address + r.x)));
}

void cpu_lda_ind_y(unsigned char zp_address)
{
    cpu_set_a(cpu_read_mem(ADDRESS_16(zp_address) + r.y));
}
void cpu_sta_ind_x(unsigned char zp_address)
{
    int address = ADDRESS_16(zp_address + r.x);

    cpu_write_mem(r.a,address);
}

void cpu_sta_ind_y(unsigned char zp_address)
{
    int address = ADDRESS_16(zp_address) + r.y;
    cpu_write_mem(r.a,address);
}

void cpu_illegal()
{
    printf("Illegal instruction\n");
    exit(1);
}






int cycle_counter = 0;
int cycle_period = 0x100;
#ifndef NDEBUG
#define HISTORY_DEPTH 10
static int last_pc[HISTORY_DEPTH];
static int last_pc_index = 0;

char *get_pc_history()
{
  char *r = malloc(HISTORY_DEPTH*6 + 1); // 4 chars + $ + linefeed + nul terminator
  char *rc = r;
  int i,j;
  for (i = 0; i < HISTORY_DEPTH; i++)
    {
      j = (i + last_pc_index) % HISTORY_DEPTH;
      sprintf(rc,"$%04x\n",last_pc[j]);
      rc += 6;
    }
  *rc = '\0';
  return r;
}

#else

char *get_pc_history()
{
  return strdup("*no PC history in release mode*");
}

#endif
void print_pc_history()
{
  puts("History of program counter prior to the crash:\n\n");
  char *r = get_pc_history();
  puts(r);
  free(r);
}
void cpu_add_cycles(int ncycles,int isize)
{

  #ifndef NDEBUG
  int i;
  cpu_dump_status();
    if (turnoff_check_count>0)
    {
      turnoff_check_count--;
    }
  else
    {
      if (debug_level & DEBUG_DETECT_SMC)
	{
	  int smc=-1;
	  for (i=r.pc;i<r.pc+isize;i++)
	    {
	      if (code_shadow_memory[i]!=m[i])
		{
		  smc=i;
		  break;
		}

	    }

	  if (smc!=-1)
	    {
	      if ((debug_level & DEBUG_DETECT_SMC_FATAL) == DEBUG_DETECT_SMC_FATAL)
		{
		  error("location ",hex$(smc)," has been modified (SMC). Before: ",hex$(code_shadow_memory[smc])," After: ",hex$(m[smc]),NULL);
		}
	      else
		{
		  printf("location $%04x has been modified. Before %02x, after %02x\n",smc,code_shadow_memory[smc],m[smc]);
		}

	    }


	}


    }



  last_pc[last_pc_index] = r.pc;
  last_pc_index = (last_pc_index+1) % (sizeof(last_pc)/sizeof(*last_pc));
 #endif

  r.pc += isize;

    cycle_counter += ncycles;
    if (cycle_counter > cycle_period)
    {
      cycle_counter -= cycle_period;
    }
}


void cpu_bit(unsigned char v)
{
    r.n = (v & 0x80) >> 7;
    r.v = (v & 0x40) >> 6;

    int res = v & r.a;

    r.z = res == 0;
}




// this is some legacy stuff
int trace_level = 0;

void cpu_trace(const char *format)
{
    if (trace_level)
    {
	printf("%s",format);fflush(stdout);
    }
}

#ifndef NDEBUG


void cpu_add_memwatch(int address, int size, int kind)
{
    MemWatch *mw = 0;

    switch (kind)
    {
      case MEMWATCH_READ:
	mw = memwatch_read_array + memwatch_read_index;
	memwatch_read_index++;
	break;
      case MEMWATCH_WRITE:
	mw = memwatch_write_array + memwatch_write_index;
	memwatch_write_index++;
	break;
    }

    mw->address = address;
    mw->size = size;
}

void cpu_set_debug_level(int p_debug_level)
{
  debug_level = p_debug_level;
  if (debug_level & DEBUG_DETECT_SMC)
    {
      // copy memory into shadow memory
      memcpy(code_shadow_memory,cpu_get_mem(),0xC000);
    }

}


void cpu_dump_memory(const char *dump_name)
{
    FILE *f;

    f = fopen(dump_name,"wb");

    if (f != 0)
    {
	fwrite(m,sizeof(m),1,f);
	fclose(f);
    }
}
#endif
