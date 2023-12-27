#include <atari.h>
#include <peekpoke.h>
#include <stdint.h>
#include <conio.h>
#include <stdio.h>

// #include <stdlib.h>
// #include <string.h>

void wait_for_vblank(void){
    asm("lda $14");
wvb:
    asm("cmp $14");
    asm("beq %g", wvb);
}

uint32_t time, time0;
uint8_t h, m, s, fps;

void compute_time(uint32_t timer)
{
    h = timer / 3600;
    m = timer / 60 % 60;
    s = timer % 60;
}

void main(void)
{
    printf("Enter time: hh mm ss\n");
    scanf("%d", &h);
    scanf("%d", &m);
    scanf("%d", &s);
    time0 = h*3600L + m*60 + s; // L upcasts to long
    time = (PEEK(18) * 65536L + PEEK(19) * 256 + PEEK(20)) / 50;
    time0 = time0 - time;
    compute_time(time0);
    printf("time0: %02d:%02d:%02d\n", h, m, s);
    fps = get_tv() ? 50 : 60;
    printf(get_tv() ? "PAL\n" : "NTSC\n");

    for(;;){
        time = (PEEK(18) * 65536L + PEEK(19) * 256 + PEEK(20)) / fps;
        time += time0;
        compute_time(time);
        gotoxy(12,12);
        printf("%02d:%02d:%02d", h, m, s);
        wait_for_vblank();
    }
}