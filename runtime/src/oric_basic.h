#ifndef _ORIC_BASIC_H
#define _ORIC_BASIC_H

#include <math.h>
#include <strings.h>
#include "oric_video.h"

typedef char T_STRING[255];
typedef char *T_STRING_PTR;
typedef int T_INT;
typedef unsigned char T_BYTE;
typedef float T_NUMERIC;

#define PRINT_COMMA_SEPARATOR (const T_STRING_PTR)-1
#define PRINT_TAB_SEPARATOR (const T_STRING_PTR)-2
#define PRINT_LINEFEED (const T_STRING_PTR)-3
#define ORIC_TRUE -1
#define ORIC_FALSE 0

#define ORICA_NEWLINE 0xCBF0
#define ORICA_PRINT_STRING_AFTER_QUOTE 0xCCB0
#define ORICA_PRINT_CHARACTER 0xCCD9
#define ORICA_PRINT_INTEGER_IN_AX 0xE0C5
#define ORICA_OUTPUT_BYTE_TO_CASSETTE 0xE65E
#define ORICA_READ_BYTE_FROM_TAPE 0xE6C9
#define ORICA_GET_IN_SYNC_WITH_CASSETTE 0xE735
#define ORICA_SET_6522_FOR_CASSETTE 0xE76A
#define ORICA_RESET_CASSETTE_STATUS 0xE93D
#define ORICA_CHECK_FOR_KEY 0xEB78
#define ORICA_TEXT 0xEC21
#define ORICA_HIRES 0xEC33
#define ORICA_DRAW 0xF110
#define ORICA_CURSET 0xF0C8
#define ORICA_PAPER 0xF204
#define ORICA_INK 0xF210
#define ORICA_FILL 0xF268
#define ORICA_CIRCLE 0xF37F
#define ORICA_CLS 0xCCCE
#define ORICA_CHAR 0xF12D
#define ORICA_CURMOV 0xf0fd
#define ORICA_PATTERN 0xF11d

#define ORIC1_PRINT_STRING_AFTER_QUOTE 0xCBED
#define ORIC1_CLS 0xCC0A
#define ORIC1_PRINT_CHARACTER 0xCC12
#define ORIC1_READ_BYTE_FROM_TAPE 0xE630
#define ORIC1_GET_IN_SYNC_WITH_CASSETTE 0xE696
#define ORIC1_SET_6522_FOR_CASSETTE 0xE6CA
#define ORIC1_CHECK_FOR_KEY 0xE905
#define ORIC1_TEXT 0xE9A9
#define ORIC1_HIRES 0xE9BB
#define ORIC1_NEWLINE 0xCB9F
#define ORIC1_PRINT_INTEGER_IN_AX 0xE0C1

enum { DT_INT, DT_NUMERIC, DT_STRING };

// common math functions are directly supported through math.h definitions

// other math functions

T_INT sgn(T_NUMERIC value);

// utility/pure CPU functions / stubbed display/input functions
void print(const T_STRING_PTR first_arg,...);
void print_string(const T_STRING_PTR s); // internal
void input(T_STRING_PTR first_arg,...);
void crlf();
void get(T_STRING_PTR s);
T_INT asc(T_STRING_PTR s);
T_STRING_PTR key$();
T_STRING_PTR left$(T_STRING_PTR value,T_INT size);
T_STRING_PTR right$(T_STRING_PTR value,T_INT size);
T_STRING_PTR mid$(T_STRING_PTR value,T_INT start_offset,T_INT length);
T_STRING_PTR mid2$(T_STRING_PTR value,T_INT start_offset);
T_STRING_PTR hex$(T_INT value);
T_STRING_PTR spc(T_INT nb);
T_STRING_PTR str$(T_NUMERIC value);
T_STRING_PTR chr$(T_INT value);
T_NUMERIC rnd(T_INT seed);
int streq(const T_STRING_PTR s1,const T_STRING_PTR s2);
int strneq(const T_STRING_PTR s1,const T_STRING_PTR s2);
void strcopy(T_STRING_PTR s1,const T_STRING_PTR s2);

void end();
void restore();
void doke(T_INT address, T_INT value);
void poke(T_INT address, T_INT value);
T_BYTE peek(T_INT address);
T_INT deek(T_INT address);
T_INT integer(T_NUMERIC n);
T_NUMERIC val(const T_STRING_PTR txt);
void data_read(const T_STRING_PTR *items,int total_nb_items,int nb_items,...);

void oric_wait(T_INT centis);

// graphical stuff

void hires();
void text();
void cls();
void lores(T_BYTE mode);
void plot(T_INT x, T_INT y, const T_STRING_PTR txt);
void plot_char(T_INT x, T_INT y, const char c);
T_INT pos(T_INT x);
T_BYTE scrn(T_INT x,T_INT y);

void paper(T_INT p);
void ink(T_INT p);

void pattern(T_INT v);
void hires_draw_char(T_INT c,T_INT charset,T_INT fb);
void curmov(T_INT x, T_INT y, T_INT fb);
void curset_from_params();
void curmov_from_params();
void curset(T_INT x, T_INT y, T_INT fb);
void circle(T_INT radius,T_INT fb);
void draw(T_INT delta_x,T_INT delta_y,T_INT fb);
void draw_from_params();
void char_from_params();
void fill(T_INT nb_lines,T_INT nb_bytes,T_INT pattern);

void dim(T_NUMERIC **ptr,T_INT size);
void dim_int(T_INT **ptr,T_INT size);
void dim_str(T_STRING **ptr,T_INT size);
void dim_int_2(T_INT ***ptr,T_INT size1,T_INT size2);
void dim_str_2(T_STRING ***ptr,T_INT size1,T_INT size2);

// sound stuff
void play(T_INT a,T_INT b, T_INT c, T_INT d);
void music(T_INT channel, T_INT octave, T_INT note, T_INT volume);
void sound(T_INT channel,T_INT pitch, T_INT volume);
void zap();
void ping();
void explode();
void shoot();

void rom_call(unsigned int address);

// assembly utility routines frequently called by basic routines
void AP_F89B();  // oric 1 reset charset
void AP_F8D0();  // oric atmos reset charset
void AP_FB10();  // oric atmos kb noise
void AP_F590();  // oric atmos kb read
// internal

#define ILLEGAL_QUANTITY "ILLEGAL QUANTITY ERROR "
#define BAD_SUBSCRIPT "BAD SUBSCRIPT ERROR "
T_BYTE *get_text_start();
T_STRING_PTR alloc_string();
void loopwait();
void set_nb_centis_per_loop(int nb_centis);

void free_pointers(void *array,int nb_items);
void gc_strings();
void error(const T_STRING_PTR message,...);

T_STRING_PTR lowercase(const T_STRING_PTR s);
T_STRING_PTR uppercase(const T_STRING_PTR s);
T_STRING_PTR strconcat(const T_STRING_PTR s1,...);
T_INT strlength(const T_STRING_PTR s);
void basic_init();

#endif
