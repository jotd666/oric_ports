#include "oric_video.h"
#include "oric_machine.h"
#include "oric_sound.h"
#include "oric_basic.h"
#include "oric_input.h"


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#ifdef __APPLE__
#define INLINE
#else
#define INLINE inline
#endif

#define CURX 617
#define CURY 616
#define MODE0 618
#define CAPLCK 0x20C

typedef struct _ModeParams
{
  T_INT start_address;
  T_INT paper;
  T_INT ink;
  T_INT nb_lines;
  T_INT text_y_start;
} ModeParams;

void print_char_and_advance(ModeParams *p,const T_BYTE tp);

#define MODE0_DOUBLE_LINE_PRINTING 0x40
#define MODE0_PROTECT_COLUMNS_01 0x20   // unsupported
#define MODE0_LAST_CHARACTER_ESC 0x10
#define MODE0_CURSOR_ON 0x1
#define MODE0_KEYCLICK_OFF 0x8
#define MODE0_VDU_ON 0x2

static ModeParams text_params;
static ModeParams hires_params;
static ModeParams hires_text_params;

static T_INT hires_cursor_x = 0;
static T_INT hires_cursor_y = 0;

static T_INT hires_mode = -1;  // invalid so if basic_init() is not called we see it when calling TEXT or HIRES
static T_INT hires_pattern = 0;
static T_INT current_hires_pattern = 0;

static int nb_centis_per_loop=6;

static int loopwait_counter=0;

static void update_paper_and_ink();

static INLINE void mode0_set(int mask)
{
  poke(MODE0,peek(MODE0) | mask);
}
static INLINE void mode0_clr(int mask)
{
  poke(MODE0,peek(MODE0) & ((~mask) & 0xFF));
}
static INLINE void mode0_toggle(int mask)
{
  if (peek(MODE0) & mask)
    {
      mode0_clr(mask);
    }
  else
    {
      mode0_set(mask);
    }
  
}

void set_nb_centis_per_loop(int nb_centis)
{
  if (nb_centis>0)
    {
      nb_centis_per_loop = nb_centis;
    }
  
}

void basic_init()
{
  hires_mode = 0;
  // text screen
  text_params.start_address = 0xBB80;
  poke(CURX,2);
  poke(CURY,1);
  poke(MODE0,MODE0_CURSOR_ON|MODE0_VDU_ON);
  poke(CAPLCK,0x80);
  
  text_params.text_y_start = 1;
  text_params.paper = 7;
  text_params.ink = 0;
  text_params.nb_lines = 28;
  
  // hires screen
  hires_params.start_address = 0xA000;
  hires_cursor_x = 0;
  hires_cursor_y = 0;
  hires_params.text_y_start = 0;
  hires_params.paper = 0;
  hires_params.ink = 7;
  hires_params.nb_lines = 200;

  // 3 text lines (hires status)
  hires_text_params.start_address = 0xBF68;
  hires_text_params.text_y_start = 0;
  hires_text_params.paper = 7;
  hires_text_params.ink = 0;
  hires_text_params.nb_lines = 3;

}
void loopwait()
{
  loopwait_counter++;
  if (loopwait_counter==50)
    {
      loopwait_counter=0;
      oric_wait(nb_centis_per_loop);  // FOR I = 1 TO 10000 takes 13 seconds to execute. We simulate that, but can be changed by code
    }
}

 
static ModeParams *get_text_mode_params()
{
  return hires_mode == 0 ? &text_params : &hires_text_params;
}
static ModeParams *get_mode_params()
{
  return hires_mode == 0 ? &text_params : &hires_params;
}

/*
INLINE T_BYTE *get_start(const ModeParams *p)
{
    return cpu_get_mem() + p->start_address; // (hires_mode == 0 ? 0xBB80 : );
   }*/
#define get_start(p) (cpu_get_mem() + (p)->start_address) 
INLINE T_BYTE *get_text_start()
{
  return get_start(&text_params);
}

T_BYTE scrn(T_INT x,T_INT y)
{
  const T_BYTE *ts = get_text_start();
  if (x<0 || x>39)
    {
      error(ILLEGAL_QUANTITY,"SCRN x =",str$(x),NULL);
    }
  if (y<0 || y>26)
    {
      error(ILLEGAL_QUANTITY,"SCRN y =",str$(y),NULL);
    }
  
  return ts[x+(y+1)*40];

}

static INLINE T_BYTE *get_current_char_pos(const ModeParams *p)
{
   return get_start(p)+peek(CURX)+peek(CURY)*40;
}

static void carriage_return()
{
  poke(CURX,2);

}

static void linefeed(ModeParams *p)
{
  
  int i;
  poke(CURY,peek(CURY)+1);
  if (peek(CURY)==p->nb_lines)
    {
      T_BYTE *ts = get_start(p)+40*p->text_y_start;
      T_INT nb_lines = (p->nb_lines-p->text_y_start-1);
      poke(CURY,peek(CURY)-1);
      // scroll
      for (i=0;i<nb_lines;i++)
	{
	  memcpy(ts,ts+40,40);
	  ts += 40;
	}
      memset(ts+2,32,38); // clear last line preserve attributes
      
    }

}
static void next_char(ModeParams *p)
{
  poke(CURX,peek(CURX)+1);

  if (peek(CURX)==40)
    {
      crlf(p);
    }
}
static void prev_char()
{
  const int offset = 2;  // added for waydor
  int cx = peek(CURX)-1;
  int cy = peek(CURY);
  if (cx == offset-1)
    {
      if (cy > 0)
	{
 
	  cy--;
	  cx=39;
	}   
      else
	{
	  cx = offset;
	}
    }
  poke(CURX,cx);
  poke(CURY,cy);

}
void rom_call(unsigned int address)
{
  int param1 = deek(0x2E1);
  int param2 = deek(0x2E3);
 
  
  switch(address)
    {
      case ORICA_PRINT_CHARACTER:
	case ORIC1_PRINT_CHARACTER:
	 {
	 ModeParams *p = get_text_mode_params();
	  print_char_and_advance(p,r.a);
	  break;
	}
      case ORICA_PRINT_INTEGER_IN_AX:
	case ORIC1_PRINT_INTEGER_IN_AX:
	{
	  int value = r.x + (r.a<<8);  // ATM signed isn't supported (missing test case)
	  print(str$(value),NULL);
	  }
	break;
      case ORICA_PRINT_STRING_AFTER_QUOTE:
	case ORIC1_PRINT_STRING_AFTER_QUOTE:
	{
	  int i = 0, address = (r.y << 8) + r.a;
	   ModeParams *p = get_text_mode_params();
	  
	  while(1)
	    {
	      T_BYTE c = peek(address+i);
	      if (c==0) break;
	      print_char_and_advance(p,c);
	      i++;
	    }
	  break;
	}
	
      case ORICA_CHECK_FOR_KEY:
      case ORIC1_CHECK_FOR_KEY:
	{
	  /* This routine checks whether
 a new key is ready to be processed. If there is, the 
     Ascii char for it is loaded 
    into A and #02DF cleared. If N=0 at exit then no new 
	     key has been received. 
	   */
	  
	  r.a = key$()[0];
	  r.n = !!r.a;
	  break;
	}	
      case ORICA_NEWLINE:
     case ORIC1_NEWLINE:
	crlf();
	break;
      case  ORICA_TEXT:
     case  ORIC1_TEXT:
	text();
	break;
      case ORICA_HIRES:
      case ORIC1_HIRES:
	hires();
	break;
      case ORICA_DRAW:
	draw_from_params();
	break;
	case ORICA_PAPER:
	paper(param1);
	break;
      case ORICA_INK:
	ink(param1);
	break;
      case ORICA_FILL:
	fill(param1,param2,deek(0x2E5));
	break;
      case ORICA_CURSET:
	curset_from_params();
	break;
      case ORICA_CURMOV:
	curmov_from_params();
	break;
      case ORICA_CIRCLE:	
	circle(param1,param2);
	break;
      case ORICA_CLS:
      case ORIC1_CLS:
	cls();
	break;
      case ORICA_PATTERN:
	pattern(param1);
	break;
      case ORICA_CHAR:
	char_from_params();
	break;
	case 0xF89B:
	AP_F89B();  // oric 1 reset charset
	break;
	case 0xF8D0:
	AP_F8D0();  // oric atmos reset charset
	break;
	case 0xFB10:
	AP_FB10();  // oric atmos kb noise
	break;
	case 0xF590:
	AP_F590();  // oric atmos kb read
	break;
      case 0xF88F:
	// hard reset: quit program
	quit();
	break;
      default:
	print_pc_history();
	 error("ROM CALL: unsupported address ",hex$(address),NULL);
	break;
    }
  
  }

void print(const T_STRING_PTR first_arg,...)
{
  switch((long long)first_arg)
    {
      case 0:
      case (long long)PRINT_LINEFEED: 
	
	crlf();
	break;
      case (long long)PRINT_COMMA_SEPARATOR: 
	do
	  {
	    print_string(" ");
	  }
	  while(peek(CURX) % 5 != 0);
	break;
      case (long long)PRINT_TAB_SEPARATOR: 
	break;
      default:
  
	{
	  va_list ap;
	  va_start(ap,first_arg);
	  print_string(first_arg);
	  while(1)
	    {
	      T_STRING_PTR na = va_arg(ap,T_STRING_PTR);
	      if(na==0)break;
	      switch ((long long)na)
		{
		  case (long long)PRINT_LINEFEED: 
		    crlf();
		    break;
		  case (long long)PRINT_COMMA_SEPARATOR: 
		    do
		      {
			print_string(" ");
		      }
		      while(peek(CURX) % 5 != 0);
		    break;
		  case (long long)PRINT_TAB_SEPARATOR: break;
		  default:
		    print_string(na);
		}
	    }
	  va_end(ap);
	  break;
	}
    }
}


void print_char(ModeParams *p,const T_BYTE t)
{
  T_BYTE *ts = get_current_char_pos(p);
  if (peek(MODE0) & MODE0_VDU_ON)
    {
      *ts = t;
      if (peek(MODE0) & MODE0_DOUBLE_LINE_PRINTING)
	{
	  int last_line = (peek(CURY)==p->nb_lines-1);
	  linefeed(p);
	  ts = get_current_char_pos(p);
	  *ts = t;
	  if (!last_line)
	    {
	      poke(CURY,peek(CURY)-1);
	    }
      
	}
    }
}

void print_char_and_advance(ModeParams *p,const T_BYTE tp)
{
  /*
  0 Null
1 Copy CTRL-A
2
3 Break CTRL-C
4 Double line printing CTRL-D
5
6 Keyclick CTRL-F
7 Bell (PING) CTRL-G
8 Backspace (Cursor left) CTRL-H
9 Cursor right CTRL-1
10 Line feed (Cursor down) CTRL-J
11 Cursor up CTRL-K
12 Clear screen CTRL-L
13 RETURN CTRL-M
14 Clear line CTRL-N
15 Disable screen CTRL-0
16
17 Cursor CTRL-Q
18
19 Screen CTRL-S
20 Caps (upper case) CTRL-T
24 Cancel line CTRL-X*/
  T_BYTE t = tp;
  int forward = 0;
  
  switch(t)
    {
      case 10:
	linefeed(p);
	break;
      case 13:
	carriage_return(p);
	break;
      case 12:
	cls();
	break;
      case 14:
	 // clear line
	{
	  int i,old_cursor=peek(CURX);
	  for (i=old_cursor;i<40;i++)
	    {
	      print_char_and_advance(p,' ');	      
	    }
	  poke(CURX,old_cursor);
	  }
	  break;
      case 17:
	mode0_toggle(MODE0_CURSOR_ON);
	break;
      case 19:
	mode0_toggle(MODE0_VDU_ON);
	break;
      case 20:
	if (peek(CAPLCK)==0x80)
	  {
	    poke(CAPLCK,0);
	  }
	else
	  {
	    poke(CAPLCK,0x80);
	  }
	    
	memset(get_text_start()+35,7,peek(CAPLCK) ? 7 : 0);
	break;
      case 30: // HOME
	poke(CURX,2);
	poke(CURY,p->text_y_start);
	

	break;
      case 4:
	mode0_toggle(MODE0_DOUBLE_LINE_PRINTING);
	break;
      case 6:
	mode0_toggle(MODE0_KEYCLICK_OFF);
	break;
      case 7:
	ping();
	break;
      case 8:	
	prev_char();
	break; // backspace
      case 127:
	
	prev_char();
	print_char(p, ' ');
	    
	break; // backspace

      case 27:
	mode0_set(MODE0_LAST_CHARACTER_ESC);
	break;
      default:
	forward = 1;
	if (t>31)
	  {
	    // "inverse video" paper&ink&other attributes
	    // treat them directly, and ignore ESCAPE if any
	    if ((t>127) && (t<152))
	      {
		t -= 128;
		mode0_clr(MODE0_LAST_CHARACTER_ESC);
	      }

	    if (peek(MODE0) & MODE0_LAST_CHARACTER_ESC)
	      {
		mode0_clr(MODE0_LAST_CHARACTER_ESC);
		print_char(p, t-64);
	      }
	    else
	      {
		
		print_char(p, t);
	      }
	  }
	
    }
  if ((!(peek(MODE0) & MODE0_LAST_CHARACTER_ESC)) && forward)
    {
      next_char(p);
    }

  
}


void crlf()
{
  ModeParams *p = get_text_mode_params();
  carriage_return(p);
  linefeed(p);
}

void print_string(const T_STRING_PTR t)
{
  int lt = (int)strlen(t);
  int i;
  ModeParams *p = get_text_mode_params();

  for (i=0;i<lt;i++)
    {
      print_char_and_advance(p,(T_BYTE)(t[i]));
    }
}

T_INT pos(T_INT m)
{
  if (m!=0)
    {
      error(ILLEGAL_QUANTITY,"POS",NULL);
    }
  return peek(CURX);
}

void input(T_STRING_PTR first_arg,...)
{
  // first count nb params
  ModeParams *p = get_text_mode_params();
  int i,nb_params = 0;
  va_list ap;
  va_start(ap,first_arg);
  int ok=0;
  char current_key = 0;
  T_STRING s;
  T_STRING_PTR cs,output_string = first_arg;
  // compute the number of params
  while(1)
    {
      T_STRING_PTR na = va_arg(ap,T_STRING_PTR);
      if(na==0)break;
      nb_params++;
    }
  va_end(ap);

  // not a simple INPUT A$
  if (nb_params>0)
    {
      print_string(first_arg);
      va_start(ap,first_arg);
      for (i=0;i<nb_params-1;i++)
	{
	  T_STRING_PTR na = va_arg(ap,T_STRING_PTR);
	  print_string(na);
	}
      output_string = va_arg(ap,T_STRING_PTR);
      va_end(ap);
    }
  cs = output_string;
  print_string("? ");
  // wait for all keys up

  while(1)
    {
      
      int full = (cs-output_string) == (sizeof(T_STRING)-1);
      get(s);
      current_key = s[0];
      gc_strings();

      *cs = current_key;
      
      switch (current_key)
	{
	  case SDLK_RETURN:
	    if (cs>output_string)
	      {
		*cs = 0;
		ok = 1;
		crlf();
	      }
	    break;
	  case 9:
	  case 11:
	  case 10:
	    // arrows: do nothing
	    break;
	  case 127:
	  case SDLK_BACKSPACE:
	    if (cs>output_string)
	      {
		*cs = 0;
		cs--;
		prev_char(p);
		print_char(p,' ');
	      }
	    break;
	  default:
	    if (!full)
	      {
		print_char(p,*cs);
		next_char(p);
		cs++;
	      }
	    break;
	}
	  
      if (ok)
	{break;}
	  
	  
	  
	}
    
    
}

void get(T_STRING_PTR s)
{
  if (s!=NULL)
    {
      s[1] = 0;
      s[0] = read_key_event(1,peek(CAPLCK)==0);
      poke(0x35,s[0]);
    }
  else
    {
      // just wait for key (debug/when we don't care about the value)
      read_key_event(1,0);
    }
  
}

T_STRING_PTR key$()
{
  T_STRING_PTR s=alloc_string();
  s[0] = read_key_nonblocking(peek(CAPLCK)==0);
  s[1] = 0;
  return s;
}


void lores(T_BYTE mode)
{
  int i;
  T_BYTE *start = get_text_start();
  text();
  
  for (i=text_params.text_y_start;i<text_params.nb_lines;i++)
    {
      start[i*40] = 8+mode;
    }
  
}
	
void pattern(T_INT value)
{
  if (value<0 || value>255)
    {
      error(ILLEGAL_QUANTITY,"PATTERN",0);
    }
  hires_pattern = value;
}

void cls()
{
  ModeParams *p = get_text_mode_params();
  //print_params.double_line_printing = 0;
  T_BYTE *start = get_start(p)+40*p->text_y_start;  // don't clear the "status" bar
  memset(start, ' ', (p->nb_lines-p->text_y_start)*40);
  poke(CURX, 2);
  poke(CURY, p->text_y_start);
  
  update_paper_and_ink(p);

}

static INLINE T_INT pixel_address(T_INT x,T_INT y)
{
  return hires_params.start_address + x/6 + y*40;
}


static int set_point(T_INT nx, T_INT ny, T_INT fb)
{
  int rval = 0;
    if (nx>=0 && nx<240 && ny>=0 && ny<200)
    {
      rval = 1;
      hires_cursor_x = nx;
      hires_cursor_y = ny;
      
      switch(fb)
	{
	  case 3:
	    break;
	  case 0:
	  case 2:
	  case 1:
	    {
		  // mark pixel
	      T_INT pa = pixel_address(nx,ny);
	      	      
	      T_BYTE new_value = peek(pa);
	      if (new_value & 0x40)
		{
		  int remainder_bit = 1<<(5-(nx % 6));
		  if (fb==0) 
		    { 
		      new_value &= ((!remainder_bit) & 0x3F) | 0x40;
		    }
		  else if (fb==1)
		    {
		      new_value |= remainder_bit;
		    }
		  else if (fb==2)
		    {
		      new_value ^= remainder_bit;
		    }
		  poke(pa,new_value);
		}
	      
	      break;
	    }
	  default:
	    error("CURSET",": unsupported fb mode",str$(fb),0);
	    break;
	}
    }
  return rval;
}

T_INT point(T_INT nx, T_INT ny)
{
  if (nx>=0 && nx<240 && ny>=0 && ny<200)
    {
      T_INT pa = pixel_address(nx,ny);
      int remainder_bit = 1<<(5-(nx % 6));
      T_BYTE new_value = peek(pa);
      return new_value & remainder_bit ? ORIC_TRUE : ORIC_FALSE;
    }
  else
    {
      error(ILLEGAL_QUANTITY,"POINT",str$(nx),str$(ny),0);
      return ORIC_FALSE;
    }
  
      
}


void curset(T_INT nx, T_INT ny, T_INT fb)
{
  if (!set_point(nx,ny,fb))
    {
      error("CURSET",": ",ILLEGAL_QUANTITY," ",str$(nx)," ",str$(ny),0);
    }
  
  T_INT pa = pixel_address(nx,ny);
	      
  poke(0x21A,ny); // CURY  used by l'Aigle d'Or!!
  poke(0x219,nx); // CURX  used by l'Aigle d'Or!!
  doke(0x10,pa);  //CURSET sets pixel address in 0x10, used by l'Aigle d'Or!!
  

}


void curmov(T_INT x, T_INT y, T_INT fb)
{
  int nx = hires_cursor_x + x;
  int ny = hires_cursor_y + y;
  curset(nx,ny,fb);

}
static void init_current_pattern()
{
  current_hires_pattern = hires_pattern;
}
static void next_current_pattern()
{
  // ROL
  current_hires_pattern = ((current_hires_pattern<<1) & 0xFF) | ((current_hires_pattern>>7)&0x1);
}


static T_INT signed_deek(T_INT address)
{
  T_INT rval = deek(address);
  if (rval>32767)
    {
      rval -= 65536;
    }  
  return rval;
}

void curset_from_params()
{
  curset(deek(0x2E1),deek(0x2E3),deek(0x2E5));
}

void curmov_from_params()
{
  curmov(deek(0x2E1),deek(0x2E3),deek(0x2E5));
}

void draw_from_params()
{
  T_INT dx=signed_deek(0x2E1);
  T_INT dy=signed_deek(0x2E3);
  T_INT fb=deek(0x2E5);
  draw(dx,dy,fb);
}
void char_from_params()
{
  T_INT c=deek(0x2E1);
  T_INT charset=deek(0x2E3);
  T_INT fb=deek(0x2E5);
  hires_draw_char(c,charset,fb);
}

// Bresenham line algorithm adapted from fragkakis Java implementation
// (see https://github.com/fragkakis/bresenham/tree/master/src/main/java/org/fragkakis)

static void plot_line(int x0,int y0, int x1,int y1,int fb)
{
  int local_fb = fb;

  int dx = abs(x1 - x0);
  int dy = abs(y1 - y0);
  int sx = x0 < x1 ? 1 : -1;
  int sy = y0 < y1 ? 1 : -1;
  int err = dx-dy;
  int e2;
  int currentX = x0;
  int currentY = y0;
  for(;;)
    {
      curset(currentX,currentY,local_fb);

      if (currentX == x1 && currentY == y1) 
	{
	break;
	}
      // advance pattern
      if ((fb!=3)&&(hires_pattern!=255))
	{
	  next_current_pattern();
	  local_fb = current_hires_pattern&1 ? fb : 3;
	}
      
      e2 = 2*err;
      if(e2 > -dy) 
	{
	  err -= dy;
	  currentX += sx;
	}
      if(e2 < dx) 
	{
	  err += dx;
	  currentY += sy;
	}
    }
}



void draw(T_INT delta_x,T_INT delta_y,T_INT fb)
{
  T_INT saved_x = hires_cursor_x;
  T_INT saved_y = hires_cursor_y;
  T_INT to_x = saved_x+delta_x;
  T_INT to_y = saved_y+delta_y;
  init_current_pattern();
  if (to_x<0 || to_x>239) 
    { 
      error(ILLEGAL_QUANTITY,"DRAW", " curx=",str$(hires_cursor_x),
	    " to x=",str$(to_x),0);
    }

  if (to_y<0 || to_y>199) {    
    error(ILLEGAL_QUANTITY,"DRAW", " cury=",str$(hires_cursor_y),
	  " to y=",str$(to_y),0);
  }
  
  if (delta_x==0)
    {
      T_INT y_step = sgn(delta_y);
      int local_fb=fb;
      // horizontal line
      int i=saved_y;
      while (i!=to_y)
	{
	  if ((fb!=3)&&(hires_pattern!=255))
	    {
	      next_current_pattern();
	      local_fb = (current_hires_pattern&1) ? fb : 3;
	    }
	  
	  curset(saved_x,i,local_fb);
	  i+=y_step;

	}
    }
  else
    {      
      plot_line(saved_x,saved_y,to_x,to_y,fb);
    }
  curset(to_x,to_y,3);

}

void hires_draw_char(T_INT c,T_INT charset,T_INT fb)
{
  T_BYTE *char_address = cpu_get_mem()+0x9800+(c+128*charset)*8;
  int i,j;
  T_INT saved_x=hires_cursor_x;
  T_INT saved_y=hires_cursor_y;
  
  for (i = 0;i<8;i++)
    {
      T_BYTE d = char_address[i];
      for (j = 0;j < 6;j++)
	{
	  if ((d>>j) & 1)
	    {
	      set_point(saved_x+5-j,saved_y+i,fb);
	    }
	      
	}
      
    }
  set_point(saved_x,saved_y,3);
  
}

void fill(T_INT nb_lines,T_INT nb_bytes,T_INT pattern)
{
  int j;
 
  if ((hires_cursor_y+nb_lines<=hires_params.nb_lines) && (hires_cursor_x+nb_bytes*6 <= 240) && nb_bytes>=0 && nb_lines>=0)
    {
       int oric_address = hires_params.start_address+hires_cursor_x/6 + hires_cursor_y*40;;
      T_BYTE *start_mem = cpu_get_mem() + oric_address;
      
      
      for (j=0;j<nb_lines*40;j+=40)
	{
	  poke(oric_address+j,pattern); // so video knows it has to refresh
	  memset(start_mem+j+1,pattern,nb_bytes-1); // so it's super fast (as opposed to lame loop)
	}
    }
  else
    {
      error(ILLEGAL_QUANTITY,"FILL"," nb lines= ",str$(nb_lines),
	    " nb bytes= ",str$(nb_bytes)," curx=",str$(hires_cursor_x),
	    " cury=",str$(hires_cursor_y),0);
    }
  // increase y cursor by the number of filled lines. This is an undocumented feature
  // but it works like this
  hires_cursor_y += nb_lines;
  
}

void circle(T_INT radius,T_INT fb)
{
  T_INT saved_x = hires_cursor_x;
  T_INT saved_y = hires_cursor_y;
  float step = (0.25*90)/radius; // empiric, if increase step the "macrame" oric effect appears
  float angle = 0;
  while (angle<180.0)
    {
      float radangle = angle*M_PI/180.0;
      int delta_x = radius * cos(radangle);
      int delta_y = radius * sin(radangle);
      set_point(saved_x+delta_x,saved_y+delta_y,fb);
      set_point(saved_x+delta_x,saved_y-delta_y,fb);
      angle += step;
    }
  set_point(saved_x,saved_y,3); // restore curset
  
  }

void update_paper_and_ink(const ModeParams *p)
{
   
  T_INT i;
  for (i = p->text_y_start; i < p->nb_lines;i++)
    {
      T_INT offset = p->start_address + i*40;
      poke(offset,p->paper + 16);
      poke(offset+1,p->ink);
    }
}
void paper(T_INT paper)
{
  ModeParams *p = get_mode_params();
  p->paper = paper;
  poke(0x26B,paper+16);
  update_paper_and_ink(p);
  if (hires_mode)
    {
      update_paper_and_ink(&hires_text_params);
    }
}

void ink(T_INT ink)
{
  ModeParams *p = get_mode_params();
  p->ink = ink;
  poke(0x26C,ink);
  update_paper_and_ink(p);
  if (hires_mode)
    {
      update_paper_and_ink(&hires_text_params);
    }
}

void AP_F89B()
{
  reset_charset();
}

void AP_F8D0()
{
  reset_charset();
}

void dim(T_NUMERIC **ptr,T_INT size)
{
  if (*ptr==0)
    {
      *ptr = malloc(size*sizeof(T_NUMERIC));
      memset(*ptr,0,size*sizeof(T_NUMERIC));
    }
  
}

void dim_int(T_INT **ptr,T_INT size)
{
  if (*ptr==0)
    {
      *ptr = malloc(size*sizeof(T_INT));
      memset(*ptr,0,size*sizeof(T_INT));
    }
  
}
void dim_int_2(T_INT ***ptr,T_INT size1,T_INT size2)
{
  if (*ptr==0)
    {
      int i;
      *ptr = (T_INT**)malloc(size1*sizeof(T_INT**));
      for (i=0;i<size1;i++)
	{
	  (*ptr)[i] = malloc(size2*sizeof(T_INT));
	  memset((*ptr)[i],0,size2*sizeof(T_INT));
	}
      
    }
  
}

void dim_str(T_STRING **ptr,T_INT size)
{
  if (*ptr==0)
    {
      *ptr = malloc(size*sizeof(T_STRING));
      memset(*ptr,0,size*sizeof(T_STRING));
    }
  
}

void dim_str_2(T_STRING ***ptr,T_INT size1,T_INT size2)
{
  if (*ptr==0)
    {
      int i;
      *ptr = malloc(size1*sizeof(T_STRING *));
      for (i=0;i<size1;i++)
	{
	  (*ptr)[i] = malloc(size2*sizeof(T_STRING));
	  memset((*ptr)[i],0,size2*sizeof(T_STRING));
	}
     
    }
  
}

void free_pointers(void *array,int nb_items)
{
  int **p = (int **)array;
  int i;
  for (i = 0;i<nb_items;i++)
    {
      int *z = p[i];
      if (z!=0)
	{
	  free(z);
	  *z=0;
	}
    }
  }

void hires()
{
  unsigned char *mem = cpu_get_mem();
    if (hires_mode==-1)
    {
      error("basic_init() was not called",0);
    }

  if (!hires_mode)
    {
      // transfer charset
      memcpy(mem + 0x9800, mem + 0xB400, 0xB80-0x400);
      // reset pattern
      pattern(255);
    }
  memset(mem + 0xA000, 64, hires_params.nb_lines*40); // hires cls
  int i; // force screen update by writing in the lines
  for (i = 0;i<hires_params.nb_lines;i++)
    {
      poke(0xA000 + 40*i,64);
    }
  
  poke(0xBB80,0x1F);  // so VDU switches to hires

  hires_mode = 1;
  poke(0x21F,1);
  hires_text_params.ink = text_params.ink;
  hires_text_params.paper = text_params.paper;
  // wait for a refresh
  SDL_Delay(100);
  cls();
}

void text()
{

  unsigned char *mem = cpu_get_mem();
  poke(0x21F,0);
  if (hires_mode==-1)
    {
      error("basic_init() was not called",0);
    }
  
  if (hires_mode)
    {
      // Transfer charset back
      memcpy(mem + 0xB400, mem + 0x9800, 0xB80-0x400);
      // reset print params all to default
      mode0_clr(MODE0_LAST_CHARACTER_ESC);
      //memset(&print_params,0,sizeof(print_params));
      hires_mode = 0; // BEFORE cls!!
      cls();
    }
  else
    {
      hires_mode = 0;  
    }
  poke(0xBB80,' '); 
  memset(mem + 0xBB81, ' ', 34); // text cls for the status bar
  
  memcpy(get_text_start()+35,"\007CAPS",5);

  SDL_Delay(100);
}


void plot(T_INT x, T_INT y, const T_STRING_PTR txt)
{
    memcpy(get_text_start() + x + (y+1)*40,txt,strlen(txt));
}
void plot_char(T_INT x, T_INT y, const char c)
{
    get_text_start()[x + (y+1)*40] = c;
}

void sound(T_INT channel,T_INT pitch, T_INT volume)
{
  // TODO
  (void)channel;
  (void)pitch;
  (void)volume;
  
  //printf("SOUND %d,%d,%d\n",channel,pitch,volume);
}

void play(T_INT a,T_INT b, T_INT c, T_INT d)
{
  (void)b;
  (void)c;
  (void)d;
  
    if (a == 0)
    {
	T_INT i;
	for (i = 1; i < 4; i++)
	{
	    sound_stop_music(i);
	}
    }

}
void music(T_INT channel, T_INT octave, T_INT note, T_INT volume)
{
  (void)volume;
    sound_play_music(channel,octave,note);
}

void zap()
{
}

void ping()
{
}

void explode()
{
}
void shoot()
{
}
void AP_FB10()
{
  
}
void AP_F590()
{
  read_key_nonblocking(0);

}

