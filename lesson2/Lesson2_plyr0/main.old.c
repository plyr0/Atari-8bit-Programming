#include <atari.h>
#include <stdio.h>
#include <stdlib.h>
#include <peekpoke.h>
#include <string.h>

#define CHARSET_MEM  0x3000
#define DLIST_MEM    0x4000
#define SCREEN_MEM   0x5000

#define STICK_RIGHT  7
#define STICK_LEFT   11
#define STICK_DOWN   13
#define STICK_UP     14
#define STICK_NOMOVE 15

#define MOVE_TYPE_NONE  0
#define MOVE_TYPE_LEFT  1
#define MOVE_TYPE_RIGHT 2
#define MOVE_TYPE_UP    3
#define MOVE_TYPE_DOWN  4

unsigned int xpos;
unsigned char stick;
unsigned char move_type;

unsigned char antic4_display_list[] = {
    DL_BLK8,
    DL_BLK8,
    DL_BLK8,
    DL_LMS(DL_CHR40x8x4),
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

// 01 01 01 01 - 0=pfc1 1=pfc2
unsigned char sprite_data[] = {
    0b00010100,
    0b01010101,
    0b00101000,
    0b00101000,
    0b00101000,
    0b01010101,
    0b01010101,
    0b01010101
};

void wait_for_vblank(void){
    asm("lda $14");
wvb:
    asm("cmp $14");
    asm("beq %g", wvb);
}

void erase_sprite(void){
    POKE(SCREEN_MEM + xpos, 0);
}

void draw_sprite(void){
    POKE(SCREEN_MEM + xpos, 0x21);
}

void update_sprite(void){
    if( MOVE_TYPE_RIGHT == move_type){
        ++xpos;
    } else if( MOVE_TYPE_LEFT == move_type){
        --xpos;
    }else if( MOVE_TYPE_UP == move_type){
        xpos-=40;
    } else if( MOVE_TYPE_DOWN == move_type){
        xpos+=40;;
    }

    //++xpos;
    //xpos %=(42*20);
}

void handle_input(void){
    stick = OS.stick0;
    if(STICK_RIGHT == stick){
        move_type = MOVE_TYPE_RIGHT;
    } else if(STICK_NOMOVE == stick){
        move_type = MOVE_TYPE_NONE;
    } else if(STICK_LEFT == stick){
        move_type = MOVE_TYPE_LEFT;
    } else if(STICK_UP == stick){
        move_type = MOVE_TYPE_UP;
    } else if(STICK_DOWN == stick){
        move_type = MOVE_TYPE_DOWN;
    }
}

void main(void)
{
    xpos=0;
    move_type=MOVE_TYPE_NONE;

    memcpy((void*)DLIST_MEM, antic4_display_list, sizeof(antic4_display_list));
    OS.sdlst = (void*)DLIST_MEM;

    //OS.color0 = 
    OS.color1 = 0xA3;
    OS.color2 = 0x26;
    OS.color3 = 0xE1;
    OS.color4 = 0;

    memcpy((void*)CHARSET_MEM, (void*)0xE000, 0x0400);

    memcpy((void*)(CHARSET_MEM + 8 * 0x21), sprite_data, 8);
    OS.chbas = CHARSET_MEM >> 8;

    
    for(;;){
        wait_for_vblank();
        handle_input();
        erase_sprite();
        update_sprite();
        draw_sprite();
    }
}