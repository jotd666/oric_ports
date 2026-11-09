; non-scratch registers are named

planes equr a3
previous_screen_memory equr a4
screen_memory  equr a5
character_memory equr a6
line_properties  equr  d4
amiga_x_times_8     equr d5
oric_x     equr d6
line_address_offset equr d7

; bits for line_properties. First 3 bits match oric text gfx control
B_ALTERNATE_TEXT = 0
B_DOUBLE_HEIGHT = 1
B_FLASHING = 2
; others are "extended" bits used in this implementation
B_INVERSE_VIDEO = 5
B_FORCE_REFRESH = 6
B_CHARACTER_CHANGED = 7


; a0-a2 d0-d3 are scratch registers

AMIGA_ROW_NB_BYTES = 40
	MC68020


DRAW_6_CHARS:MACRO
	move.l	(\1,planes),a2
	move.w	(a2,d1.l),d2
	and.b	#$C0,d2	; clear last 6 bits from the previous graphical data
	or.w	(a1)+,d2		; 16-bit pattern to OR to plane 1
	; move but clear 2 bytes forward too
	; faster than 2 LSL instructions
	swap	d2
	clr.w	d2
	move.l	d2,(a2,d1.l)
	ENDM
	
DRAW_6_CHARS_DOUBLE:MACRO
	DRAW_6_CHARS	\1
	move.l	d2,(AMIGA_ROW_NB_BYTES,a2,d1.l)
	ENDM
	

BLANK_6_CHARS:MACRO
	move.w	(a2),d0
	and.b	#$C0,d0	; clear last 6 bits from the previous graphical data
	or.w	d2,d0
	; faster than 2 LSL instructions
	swap	d0
	clr.w	d0
	move.l	d0,(a2)	; 32 bit move so it draws & clears the following pixels
	ENDM
	
HIRES_BLANK:MACRO
	move.l	(a1)+,a2
	move.w	(a0)+,d2		; 16-bit pattern to OR to plane x
	add.l	d1,a2
	BLANK_6_CHARS
	ENDM

DO_FORCE_REFRESH
	
	XDEF	_video_render
	XDEF	_asm_init
	
	; I found no documentation about parameter passing but figured it out anyway :)
	; uses stack to pass pointers. I save the whole register lot in case the compiler uses it
_asm_init:
	move.l	(4,a7),oric_memory
	move.l	(8,a7),planes_array
	move.l	(12,a7),oric_previous_memory

	movem.l	d0/a0,-(a7)

	; oric memory & screen
	move.l	oric_memory,d0
	add.l	#$09900,d0
	move.l	d0,hires_character_memory_ptr
	add.l	#$A000-$9900,d0
	move.l	d0,hires_memory_ptr
	add.l	#$1500,d0
	move.l	d0,text_character_memory_ptr
	add.l	#$BB80-$B500,d0
	move.l	d0,text_memory_ptr

	move.l	oric_previous_memory(pc),a0
	add.l	#$A000,a0
	move.l	a0,previous_hires_memory_ptr
	add.l	#$1B80,a0
	move.l	a0,previous_text_memory_ptr
	
	move.l	oric_previous_memory(pc),a0
	move.w	#$2FFF,d0
.clr
	move.l	#$55555555,(a0)+
	dbf	D0,.clr
.out
	movem.l	(a7)+,d0/a0
	rts
	
_video_render:
	movem.l	D1-A6,-(a7)
	move.w	flash_counter,d0
	add.w	#1,d0
	cmp.w	#25,d0
	bne.b	.noflip
	move.w	#$F00,$DFF180
	eor.b	#1,flash_bit
	moveq.w	#0,d0
.noflip
	move.w	d0,flash_counter
	
	; bitplanes
	move.l	planes_array,planes
	; now A0 yields oric memory
	; A1 yields pointers on bitplanes	

.video_switch_loop

	move.w	#28*8,nb_remaining_lines
	moveq.l	#0,line_address_offset

	tst.b	hires_flag
	bne.b	.init_hires

	lea	text_memory_ptr(pc),a0
	bra.b	.init_all
.init_hires
	lea	hires_memory_ptr(pc),a0	
.init_all	

	move.l	(a0)+,screen_memory
	move.l	(a0)+,character_memory_ptr
	move.l	(a0)+,previous_screen_memory	

.lineloop
	move.l	planes,a1
	tst.b	hires_flag
	bne.b	.hires_line_0
	; clear the first byte of the row
	; first plane
	bsr	.textzero
	; second plane
	bsr	.textzero
	; third plane
	bsr	.textzero
	bra.b	.linecont
	
	; TODO do zero when drawing the first char/attribute if x==0
	; instead of this
.textzero
	move.l	(a1)+,a2
	moveq.l	#7,d3
.textzero_loop_1
	clr.w	(a2,line_address_offset.L)
	add.l	#AMIGA_ROW_NB_BYTES,A2
	dbf	d3,.textzero_loop_1
	rts
.hires_line_0
	move.l	(a1)+,a2
	clr.w	(a2,line_address_offset.L)
	move.l	(a1)+,a2
	clr.w	(a2,line_address_offset.L)
	move.l	(a1)+,a2
	clr.w	(a2,line_address_offset.L)

.linecont
	
	move.l	#$7,background		; init bg+fg	
	bsr	load_pattern_table		; bg+fg pattern table
	clr.l	oric_x
	clr.l	amiga_x_times_8
	clr.l	line_properties
.colloop	

	move.l	normal_video_pattern_table(pc),current_table
	; shifting
	moveq	#0,d1
	move.b	(screen_memory)+,d1
	IFND	DO_FORCE_REFRESH
	; cleared each time a character is processed
	bclr	#B_CHARACTER_CHANGED,line_properties
	btst	#B_FORCE_REFRESH,line_properties
	beq.b	.no_force_refresh
	; just store value in previous memory, no need to compare or such
	; since draw is forced
	move.b	d1,(previous_screen_memory)+
	ENDC

	
	IFD	CHECKS
	tst.b	hires_flag
	beq.b	.nohr
	movem.l	previous_screen_memory,-(a7)
	sub.l	previous_hires_memory_ptr,previous_screen_memory
	tst.l	previous_screen_memory
	bcs.b	.mpb
	cmp.l	#$1FF0,previous_screen_memory
	bcs.b	.mok
.mpb
	; DEBUG
	sub.l	hires_memory_ptr,screen_memory
	bsr	lmb
.mok
	movem.l	(a7)+,previous_screen_memory
.nohr
	ENDC
	
	; tell that the character has changed
	bset	#B_CHARACTER_CHANGED,line_properties
	bset	#B_FORCE_REFRESH,line_properties		; not optimized!!!
	bra.b	.carry_on_char_processing
.no_force_refresh
	cmp.b	(previous_screen_memory)+,d1
	beq.b	.carry_on_char_processing
	; different character: set the flag
	bset	#B_CHARACTER_CHANGED,line_properties
	; and register the change
	move.b	d1,(-1,previous_screen_memory)
.carry_on_char_processing
	; cleared each time a character is processed
	bclr	#B_INVERSE_VIDEO,line_properties
	bclr	#7,d1	; inverse video test
	beq.b	.normal_video
	bset	#B_INVERSE_VIDEO,line_properties
.normal_video	

	tst.b	hires_flag
	beq.b	.text_mode
	; hires but last 3 lines are forced to text
	cmp.w	#3*8+1,nb_remaining_lines
	bcc		.hires_mode
; < D1: character from screen memory
.text_mode
	; attribute?
	cmp.b	#' ',d1
	beq	.draw_blank_text		; optimization: unless space is redefined (which never happens)
	bcc	.character
	bsr	.handle_attribute
	bra	.draw_blank_text

	
.hires_mode:
	; hires: from 32 to 63 nothing is drawn. Test bit 6.
	bclr	#6,d1
	bne.b	.hires_draw
	cmp.b	#$20,d1
	bcc.b	.draw_blank_hires
	; handle attributes
	bsr	.handle_attribute	
	bra.b	.draw_blank_hires
.hires_draw:
	btst	#B_CHARACTER_CHANGED,line_properties
	beq	.next_hires
	; handle flashing
	btst	#B_FLASHING,line_properties
	beq.b	.dogfx
	tst.b	flash_bit
	beq.b	.draw_blank_hires
.dogfx
	bsr	.get_shifted_table_in_a0		; first data is complete blank for the given color
	; d2 can be trashed
	
	; pattern table for pattern in d1
	lea	(a0,d1.w*8),a1	
	; now we can trash d1 (fortunately, so we can reuse text draw macro)
	move.l	amiga_x_times_8,d1
	lsr.l	#3,d1
	add.l	line_address_offset,d1
	; draw 6 chars, once, using 
	DRAW_6_CHARS	0
	DRAW_6_CHARS	4
	DRAW_6_CHARS	8
	bra.b	.next_hires
	
.draw_blank_hires	
	btst	#B_CHARACTER_CHANGED,line_properties
	beq.b	.next_hires

	bsr	.get_shifted_table_in_a0		; first data is complete blank for the given color
	move.l	amiga_x_times_8,d1
	lsr.l	#3,d1					; divided by 8
	add.l	line_address_offset,d1
	move.l	planes,a1
	; first plane
	HIRES_BLANK
	; second plane
	HIRES_BLANK
	; third plane
	HIRES_BLANK

.next_hires	
	add.l	#6,amiga_x_times_8
	addq.l	#1,oric_x
	cmp.l	#40,oric_x
	bne	.colloop
	
	add.l	#AMIGA_ROW_NB_BYTES,line_address_offset
	subq.w	#1,nb_remaining_lines
	; cannot reach max nb lines as start of the loop takes control when there are
	; less than 24 / 8 lines to process
	bra	.lineloop


	
.handle_attribute:
	btst	#B_CHARACTER_CHANGED,line_properties
	beq.b	.no_attr_change
	; attribute change means full redraw of the line
	bset	#B_FORCE_REFRESH,line_properties
.no_attr_change
	; attribute
	move.w	d1,d0
	lsr.b	#3,d0
	lea	.jump_table(pc),a0
	move.l	(a0,d0.w*4),a0
	jmp	(a0)
.jump_table
	dc.l	.foreground
	dc.l	.charset
	dc.l	.background
	dc.l	.gfxmode
.foreground:
	; foreground: update
	move.b	d1,foreground+1
	bsr	load_pattern_table
	rts
.background:
	; background: update
	sub.b	#16,d1
	move.b	d1,background+1
	bsr	load_pattern_table
	rts
.charset
	; other attributes: lores1, flashing, double height
	; just update flags
	and.b	#7,d1
	and.b	#~7,line_properties
	or.b	d1,line_properties
	btst	#B_FLASHING,d1
	beq.b	.noflashing
	; flashing means that rest of the line needs refresh, even if
	; no changes have happened
	bset	#B_FORCE_REFRESH,line_properties
.noflashing
	rts
.gfxmode
	cmp.b	#28,d1
	bcc.b	.to_hires_mode
	; if wasn't in text mode, no need to switch
	tst.b	hires_flag
	beq.b	.no_text_switch
	; text mode: restart display from row 0 with proper parameters
	clr.b	hires_flag
	;;move.l	#.out,(a7)
	move.l	#.video_switch_loop,(a7)
.no_text_switch
	rts
.to_hires_mode
	; switch to hires mode
	; here the text cursor is 0 (remaining: 26*8 lines)
	st.b	hires_flag
	; skip to next frame so everything is recomputed from scratch
	move.l	#.out,(a7)
	rts
		
.character
	btst	#B_CHARACTER_CHANGED,line_properties
	beq	.next_hires
	btst	#B_FLASHING,line_properties
	beq.b	.dochar
	tst.b	flash_bit
	beq	.draw_blank_text	
.dochar
	sub.b	#$20,d1	; character offset
	bsr	.get_shifted_table_in_a0
	; d2 can be trashed
	move.l character_memory_ptr(pc),character_memory
	btst	#B_ALTERNATE_TEXT,line_properties
	beq.b	.stdtext
	add.w	#128,d1	; LORES1
.stdtext
	lsl.w	#3,d1
	add.w	d1,character_memory
	
	move.l	amiga_x_times_8,d1
	lsr.l	#3,d1
	add.l	line_address_offset,d1
	
	btst	#B_DOUBLE_HEIGHT,line_properties
	bne	.double
	moveq.l	#7,d3
	moveq.w	#0,d0
.textchar_loop
; loop 8 times
	move.b	(character_memory)+,d0	; 0->31 code
	lea	(a0,d0.w*8),a1	
	DRAW_6_CHARS	0
	DRAW_6_CHARS	4
	DRAW_6_CHARS	8
	add.l	#AMIGA_ROW_NB_BYTES,d1
	dbf	d3,.textchar_loop

.next
	add.l	#6,amiga_x_times_8
	addq.l	#1,oric_x
	cmp.l	#40,oric_x
	bne	.colloop


	
	add.l	#AMIGA_ROW_NB_BYTES*8,line_address_offset
	subq.w	#8,nb_remaining_lines
	bmi.b	.out
	bne	.lineloop
.out




	movem.l	(a7)+,D1-A6
	moveq	#0,d0
	rts
	
	
.get_shifted_table_in_a0:
	move.l	oric_x,d2
	and.l	#3,d2	; modulus 4
	add.l	d2,d2	; *512
	lsl.l	#8,d2
	move.l	current_table(pc),a0
	; a0 points to the row of 64 words matching the shifting & bg/fg combination
	add.l	d2,a0
	rts
	
.draw_blank_text
	btst	#B_CHARACTER_CHANGED,line_properties
	beq.b	.next
	
	bsr.b	.get_shifted_table_in_a0		; first data is complete blank for the given color
	move.l	amiga_x_times_8,d1
	lsr.l	#3,d1					; divided by 8
	add.l	line_address_offset,d1
	move.l	planes,a1
	; first plane
	bsr	.textblank	
	; second plane
	bsr	.textblank
	; third plane
	bsr	.textblank
	bra.b	.next
	
	
	
.textblank
	move.l	(a1)+,a2
	move.w	(a0)+,d2		; 16-bit pattern to OR to plane x
	moveq.l	#7,d3
	add.l	d1,a2
.textblank_loop_1
	BLANK_6_CHARS
	add.l	#AMIGA_ROW_NB_BYTES,A2
	dbf	d3,.textblank_loop_1
	rts
	


	
.double
	moveq.w	#0,d0
	moveq.l	#3,d3
	btst.b	#3,nb_remaining_lines+1
	beq.b	.textchar_loop_double
	addq.l	#4,character_memory		; lower part

.textchar_loop_double
; loop 4 times
	move.b	(character_memory)+,d0	; 0->31 code
	lea	(a0,d0.w*8),a1	
	DRAW_6_CHARS_DOUBLE	0
	DRAW_6_CHARS_DOUBLE	4
	DRAW_6_CHARS_DOUBLE	8
	add.l	#AMIGA_ROW_NB_BYTES*2,d1
	dbf	d3,.textchar_loop_double
	bra	.next
	
; < (bg/fg variables below)
; > updates current_table register
; much faster than old version, tables are pre-computed
; so computing for both normal & inverse is faster than old
; way of computing normal
load_pattern_table:
	; pre-loads inverse vid table too
	move.w	background(pc),d0
	lsl.w	#3,d0
	or.w	foreground(pc),d0
	lea	_normal_video,a0
	lsl.w	#2,d0
	move.l	(a0,d0.w),normal_video_pattern_table
	add.l	#64*4,d0	; followed by inverse vid
	move.l	(a0,d0.w),inverse_video_pattern_table
	btst	#B_INVERSE_VIDEO,line_properties
	bne.b	.inverse
	move.l	normal_video_pattern_table(pc),current_table
	rts
.inverse
	; inverse video
	move.l	inverse_video_pattern_table(pc),current_table
	rts
		

	
lmb1

.loop
	move.w	#$0F0,$DFF180
	btst	#6,$BFE001
	bne.b	.loop
	rts
	
lmb
	btst	#6,$BFE001
	beq.b	lmb
.loop
	move.w	#$0F0,$DFF180
	btst	#6,$BFE001
	bne.b	.loop
	rts
	

background:
	dc.w	0
foreground:
	dc.w	0
nb_remaining_lines
	dc.w	0
	
oric_memory
	dc.l	0
planes_array
	dc.l	0
oric_previous_memory
	dc.l	0
current_table
	dc.l	0
normal_video_pattern_table
	dc.l	0
inverse_video_pattern_table
	dc.l	0
	
	; 3 values leave
text_memory_ptr
	dc.l	0
text_character_memory_ptr:
	dc.l	0
previous_text_memory_ptr
	dc.l	0
	
hires_memory_ptr:
	dc.l	0
hires_character_memory_ptr:
	dc.l	0
previous_hires_memory_ptr
	dc.l	0

character_memory_ptr:
	dc.l	0

hires_flag
	dc.b	0
	dc.b	0
flash_counter
	dc.w	0
flash_bit
	dc.b	0
	even