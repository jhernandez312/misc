#ifndef MAIN_H
#define MAIN_H

#define min(x,y) (x > y ? y : x)
#define max(x,y) (x < y ? y : x)

#include "gba.h"

// TODO: Create any necessary structs

/*
* For example, for a Snake game, one could be:
*
* struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* };
*/
  struct alien {
    int x; // x position
    int y; // y position
    int velX;
    int velY;
    int time;
  };
/*
* Example of a struct to hold state machine data:
*
* struct state {
*   int currentState;
*   int nextState;
* };
*
*/

//Prototypes
unsigned char button_pressed(unsigned short button);
void handle_buttons(struct alien *al);
void updateAlienPosition(struct alien *al);
void delay(int n);

#endif
