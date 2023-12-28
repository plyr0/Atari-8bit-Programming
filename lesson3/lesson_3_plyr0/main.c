#include <atari.h>
#include <stdio.h>
#include <stdlib.h>
#include <peekpoke.h>
#include <string.h>

#define CHARSET_MEM   0x3000
#define CHARSET_MEM2  0x3400
#define DLIST_MEM     0x4000
#define SCREEN_MEM    0x5000

#define JOY_NO_MOVE   0
#define JOY_BIT_LEFT  4
#define JOY_BIT_RIGHT 8

#define MOVING_NONE  0
#define MOVING_LEFT  1
#define MOVING_RIGHT 2

#ifndef CONSOL
#define CONSOL 53279
#define START_BUTTON 6
#endif

#pragma bss-name (push,"ZEROPAGE")
#pragma data-name (push,"ZEROPAGE")
unsigned int xpos;
unsigned int missile_xpos;
unsigned int ufo_xpos;
unsigned char stick;
unsigned char moving_type;
unsigned char ch;
unsigned char i;
unsigned char counter;
unsigned char is_missile_live;
unsigned char is_ufo_live;
unsigned char is_vblank_occured;
#pragma bss-name (pop)
#pragma data-name (pop)
#pragma zpsym ("xpos");
#pragma zpsym ("missile_xpos");
#pragma zpsym ("ufo_xpos");
#pragma zpsym ("stick");
#pragma zpsym ("moving_type");
#pragma zpsym ("ch");
#pragma zpsym ("i");
#pragma zpsym ("counter");
#pragma zpsym ("is_missile_live");
#pragma zpsym ("is_ufo_live");
#pragma zpsym ("is_vblank_occured");

unsigned char internal_string[26];

unsigned char antic4_display_list[] = {
    DL_BLK8,
    DL_BLK8,
    DL_DLI(DL_BLK8),
    DL_LMS(DL_CHR40x8x1),
    0x00,
    0x50,
    DL_CHR40x8x4,
    DL_CHR40x8x4,
    DL_CHR40x8x4,
    DL_CHR40x8x4,
    DL_CHR40x8x4,
    DL_CHR40x8x4,
    DL_CHR40x8x4,
    DL_CHR40x8x4,
    DL_CHR40x8x4,
    DL_CHR40x8x4,
    DL_CHR40x8x4,
    DL_CHR40x8x4,
    DL_CHR40x8x4,
    DL_CHR40x8x4,
    DL_CHR40x8x4,
    DL_CHR40x8x4,
    DL_CHR40x8x4,
    DL_CHR40x8x4,
    DL_CHR40x8x4,
    DL_CHR40x8x4,
    DL_CHR40x8x4,
    DL_CHR40x8x4,
    DL_CHR40x8x4,
    DL_JVB,
    0x00,
    0x40
};

//4x8: 00 01 10 11 color
unsigned char sprite_data[] = {
    0b00111100,
    0b00111100,
    0b00010100,
    0b00010100,
    0b00010100,
    0b10010110,
    0b10101010,
    0b10101010,
};

unsigned char sprite_ufo[] = {
    0b00111100,
    0b11010111,
    0b11010111,
    0b11101011,
    0b11101011,
    0b11010111,
    0b11101011,
    0b00111100,
};

unsigned char sprite_missile[] = {
    0b00111100,
    0b00010100,
    0b00111100,
    0b00010100,
    0b00111100,
    0b00010100,
    0b00101000,
    0b00111100,
};

void erase_sprite(void){
    POKE(SCREEN_MEM + xpos, 0);
    if(is_missile_live){
        POKE(SCREEN_MEM + missile_xpos, 0);
    }
    if(is_ufo_live){
        POKE(SCREEN_MEM + ufo_xpos, 0);
    }
}

void draw_sprite(void){
    POKE(SCREEN_MEM + xpos, 0x21);
    if(is_missile_live){
        POKE(SCREEN_MEM + missile_xpos, 0x7c); // |
    }
    if(is_ufo_live){
        POKE(SCREEN_MEM + ufo_xpos, 0x54); // circle
    }
}

void printinternal(unsigned char* mem, char* str){
    ch = str[0];
    i = 0;
    while(ch){
        if(ch>0 && ch<=0x1F || ch>=0x80 && ch<=0x9F){ 
            ch += 0x40; //Ruszczyc asembler book
        } else if(ch>=0x20 && ch<=0x5F || ch>=0xA0 && ch<=0xDF){
            ch -= 0x20;
        }
        mem[i] = ch;
        ++i;
        ch=str[i];
    }
}

void update_sprite(void){
    if( MOVING_RIGHT == moving_type && xpos<40*23+39){
        ++xpos;
    } else if( MOVING_LEFT == moving_type && xpos>40*23){
        --xpos;
    }

    if(is_missile_live){
        if(missile_xpos < 80){
            is_missile_live = 0;
        } else {
            missile_xpos -= 40;
        }
    }
    
    if(ufo_xpos == 79){
        ufo_xpos = 40;
    } else {
        if(!(counter%8))
            ++ufo_xpos;
    }

    if(is_missile_live && missile_xpos == ufo_xpos){
        is_ufo_live = 0;
        is_missile_live = 0;
        printinternal((unsigned char*)SCREEN_MEM, "Gratulacje UFO zniszczone");
    }
}

void handle_input(void){
    stick = (PIA.porta & 0x0f);
    stick = stick ^ 0x0f;

	if (stick == JOY_BIT_RIGHT)
	{
		moving_type = MOVING_RIGHT;
	} else if (stick == JOY_NO_MOVE)
	{
		moving_type = MOVING_NONE;
	} else if (stick == JOY_BIT_LEFT)
	{
		moving_type = MOVING_LEFT;
	}

    if(0 == GTIA_READ.trig0 && !is_missile_live){
        is_missile_live = 1;
        missile_xpos = xpos-40;
    }
}

void init_antic()
{
    memcpy((void *)DLIST_MEM, antic4_display_list, sizeof(antic4_display_list));
    OS.sdlst = (void *)DLIST_MEM;

    OS.color1 = 0xA3;
    OS.color2 = 0x26;
    OS.color3 = 0xE1;
    OS.color4 = 0;

    memcpy((void *)CHARSET_MEM, (void *)0xE000, 0x0400); // copy system charset
    memcpy((void *)(CHARSET_MEM + 8 * 0x21), sprite_data, 8); // replace one char in the copy
    memcpy((void *)(CHARSET_MEM + 8 * 0x7c), sprite_missile, 8); // replace one char in the copy
    OS.chbas = CHARSET_MEM >> 8; // let the os use new charset

    memcpy((void *)CHARSET_MEM2, (void *)0xE000, 0x0400); // copy system charset
    memcpy((void *)(CHARSET_MEM2 + 8 * 0x54), sprite_ufo, 8); // replace one char in the copy
}

void init_vars()
{
    POKE(SCREEN_MEM + xpos, 0);
    counter=0;
    xpos = 40 * 23 + 19;
    moving_type = MOVING_NONE;
    is_missile_live = 0;
    is_ufo_live = 1;
    ufo_xpos = 40;
    printinternal((unsigned char *)SCREEN_MEM, "Zniscz UFO               ");
}

// *************** IRQ ***********************************************************
void dli_routine(void)
{
	asm("pha"); asm("txa"); asm("pha"); asm("tya"); asm("pha");

	GTIA_WRITE.colbk = 0x00;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.chbase = CHARSET_MEM2 >> 8;
    GTIA_WRITE.colpf0 = 0x08;
    GTIA_WRITE.colpf1 = 0x58;
    GTIA_WRITE.colpf2 = 0xFF;
    GTIA_WRITE.colpf3 = 0x08;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.chbase = CHARSET_MEM >> 8;
    GTIA_WRITE.colpf0 = 0x45;
    GTIA_WRITE.colpf1 = 0xA3;
    GTIA_WRITE.colpf2 = 0x26;
    GTIA_WRITE.colpf3 = 0xE1;

    for(i=0x80; i<=0x8E; i++){
        ANTIC.wsync = 1;
        ANTIC.wsync = 1;
        ANTIC.wsync = 1;
        ANTIC.wsync = 1;
        ANTIC.wsync = 1;
        ANTIC.wsync = 1;
        ANTIC.wsync = 1;
        ANTIC.wsync = 1;
        ANTIC.wsync = 1;
        ANTIC.wsync = 1;
        GTIA_WRITE.colbk = i;
    }
    GTIA_WRITE.colbk = 0;

	asm("pla"); asm("tay"); asm("pla"); asm("tax"); asm("pla"); asm("rti");
}

void vbi_routine(void)
{
    OS.vdslst = &dli_routine;
	is_vblank_occured = 1;
	asm("pla"); asm("tay"); asm("pla"); asm("tax"); asm("pla"); asm("rti");
}

void wait_for_vblank_clock(void) {
	asm("lda $14");
wvb:
	asm("cmp $14");
	asm("beq %g", wvb);
}

void init_vbi_dli(void)
{
	wait_for_vblank_clock();
	OS.vvblki = &vbi_routine;

	ANTIC.nmien = NMIEN_DLI | NMIEN_VBI; // start the DLI + VBI with the NMIEN register
}

void wait_for_vblank(void)
{
	while (is_vblank_occured == 0)
	{
	};
	is_vblank_occured = 0;
}

// *************** MAIN *********************************************************
void main(void)
{
    init_antic();
    init_vars();
    init_vbi_dli();
	
    for(;;){
        wait_for_vblank();
        if(is_ufo_live){
            handle_input();
            erase_sprite();
            update_sprite();
            draw_sprite();
        } else if(START_BUTTON == PEEK(CONSOL)){
            init_vars();
        }
        ++counter;
    }
}
