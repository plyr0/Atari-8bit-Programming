#include <atari.h>
#include <stdio.h>
#include <stdlib.h>
#include <peekpoke.h>

unsigned char number;
unsigned char i;
unsigned char guess;
unsigned char atariClock;

void main(void)
{
    for(;;){
        printf("Lets play guessing number game 0-99\n");
        scanf("%i", &guess); //time of user input is the source of entropy
        
        atariClock = PEEK(0x14);
	    srand(atariClock);
        number = rand() % 100;

        for(i=0; i<2; i++)
        {    
            if (guess < number){
                printf("Incorrect: %i try bigger\n", guess);
            } else if (guess > number){
                printf("Incorrect: %i try smaller\n", guess);
            } else {
                printf("Correct: %i You won!\n", number);
                printf("Lets play guessing number game 0-99\n");
            }
            scanf("%i", &guess);
        }
        printf("You lost, number was: %i\n", number);
    }
}