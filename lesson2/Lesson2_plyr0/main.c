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

#ifndef CONSOL
#define CONSOL 53279
#define START_BUTTON 6U
#endif

unsigned int xpos;
unsigned int missile_xpos;
unsigned int ufo_xpos;
unsigned char stick;
unsigned char move_type;
unsigned char ch;
unsigned char i;
unsigned char counter;
unsigned char internal_string[26];
struct
{
    unsigned char missile_live : 1;
    unsigned char ufo_live : 1;
} flags;

unsigned char antic4_display_list[] = {
    DL_BLK8,
    DL_BLK8,
    DL_BLK8,
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

// 01 01 01 01 - 0=pfc1 1=pfc2
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

void wait_for_vblank(void){
    asm("lda $14");
wvb:
    asm("cmp $14");
    asm("beq %g", wvb);
}

void erase_sprite(void){
    POKE(SCREEN_MEM + xpos, 0);
    if(flags.missile_live){
        POKE(SCREEN_MEM + missile_xpos, 0);
    }
    if(flags.ufo_live){
        POKE(SCREEN_MEM + ufo_xpos, 0);
    }
}

void draw_sprite(void){
    POKE(SCREEN_MEM + xpos, 0x21);
    if(flags.missile_live){
        POKE(SCREEN_MEM + missile_xpos, 0x7c); // |
    }
    if(flags.ufo_live){
        POKE(SCREEN_MEM + ufo_xpos, 0x54); // circle
    }
}

void printinternal(unsigned char* mem, char* str){
    ch = str[0];
    i = 0;
    while(ch){
        if(ch>=0 && ch<=0x1F || ch>=0x80 && ch<=0x9F){ 
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
    if( MOVE_TYPE_RIGHT == move_type && xpos<40*23+39){
        ++xpos;
    } else if( MOVE_TYPE_LEFT == move_type && xpos>40*23){
        --xpos;
    }
    // else if( MOVE_TYPE_UP == move_type){
    //     xpos-=40;
    // } else if( MOVE_TYPE_DOWN == move_type){
    //     xpos+=40;
    // }

    if(flags.missile_live){
        if(missile_xpos < 80){
            flags.missile_live = 0;
        } else {
            missile_xpos -= 40;
        }
    }
    
    if(ufo_xpos == 79){
        ufo_xpos = 40;
    } else {
        if(!(counter%3))
            ++ufo_xpos;
    }

    if(flags.missile_live && missile_xpos == ufo_xpos){
        flags.ufo_live = 0;
        flags.missile_live = 0;
        printinternal((unsigned char*)SCREEN_MEM, "Gratulacje UFO zniszczone");
    }
}

void handle_input(void){
    stick = OS.stick0;
    if(STICK_RIGHT == stick){
        move_type = MOVE_TYPE_RIGHT;
    } else if(STICK_NOMOVE == stick){
        move_type = MOVE_TYPE_NONE;
    } else if(STICK_LEFT == stick){
        move_type = MOVE_TYPE_LEFT;
    }
    //  else if(STICK_UP == st){
    //     move_type = MOVE_TYPE_UP;
    // } else if(STICK_DOWN == st){
    //     move_type = MOVE_TYPE_DOWN;
    // }

    if(0 == OS.strig0 && !flags.missile_live){
        flags.missile_live = 1;
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
    memcpy((void *)(CHARSET_MEM + 8 * 0x54), sprite_ufo, 8); // replace one char in the copy
    memcpy((void *)(CHARSET_MEM + 8 * 0x7c), sprite_missile, 8); // replace one char in the copy
    OS.chbas = CHARSET_MEM >> 8; // let the os use new charset
}

void init_vars()
{
    POKE(SCREEN_MEM + xpos, 0);
    counter=0;
    xpos = 40 * 23 + 19;
    move_type = MOVE_TYPE_NONE;
    flags.missile_live = 0;
    flags.ufo_live = 1;
    ufo_xpos = 40;
    printinternal((unsigned char *)SCREEN_MEM, "Zniscz UFO               ");
}

void main(void)
{
    init_antic();
    init_vars();
    for(;;){
        wait_for_vblank();
        if(flags.ufo_live){
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
