#include <atari.h>
#include <stdio.h>
#include <stdlib.h>
#include <peekpoke.h>
#include <string.h>

#define CHARSET_MEM  0x3000
#define DLIST_MEM    0x4000
#define SCREEN_MEM   0x5000

#pragma bss-name (push,"ZEROPAGE")
#pragma data-name (push,"ZEROPAGE")
unsigned char i;
unsigned char counter;
unsigned char is_vblank_occured;
#pragma bss-name (pop)
#pragma data-name (pop)
#pragma zpsym ("i");
#pragma zpsym ("counter");
#pragma zpsym ("is_vblank_occured");

unsigned char antic4_display_list[] = {
    DL_BLK8,
    DL_BLK8,
    DL_DLI(DL_BLK8),
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

// *************** IRQ ***********************************************************
void dli_routine(void)
{
	asm("pha"); asm("txa"); asm("pha"); asm("tya"); asm("pha");

    for(i=0x00; i<=192; i++){
        ANTIC.wsync = 1;
        GTIA_WRITE.colbk = i + counter;
    }
    GTIA_WRITE.colbk = 0;
    
	asm("pla"); asm("tay"); asm("pla"); asm("tax"); asm("pla"); asm("rti");
}

void vbi_routine(void)
{
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
	OS.vdslst = &dli_routine;
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
    memcpy((void *)DLIST_MEM, antic4_display_list, sizeof(antic4_display_list));
    OS.sdlst = (void *)DLIST_MEM;

    init_vbi_dli();
    counter=0;
	
    for(;;){
        wait_for_vblank();
        ++counter;
    }
}
