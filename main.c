#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"

// IMAGE HEADERS
#include "images/alien.h"
#include "images/alien2.h"
#include "images/moon.h"
#include "images/playScreen.h"
#include "images/end.h"
#include "images/lose.h"


volatile unsigned short* buttons = (volatile unsigned short*) 0x04000130;

// PROVIDED STATES
enum gba_state {
  START,
  PLAY,
  WIN,
  LOSE,
};

// INFO FOR STATES
struct state {
    enum gba_state gamestate;
    int fuel;
};

// CONSTANTS
int FLOOR_Y = 160-16;
int GRAVITY = 1;
int JUMP = -1;
int fuel = 100;

int loseFLAG = 0;
int winFLAG = 0;
int startFLAG = 0;
int playFLAG = 0;

int buttonWasDown[NBUTTONS] = {0};
int buttonJustReleased[NBUTTONS] = {0};

int main(void) {
  REG_DISPCNT = MODE3 | BG2_ENABLE;
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  // MAKING ALIEN STRUCT

  struct alien al = {70, 0, 0, 0, 0}; // make alien

  //struct state cs, ps;

  // Load initial application state
  enum gba_state state = START;

  while (1) {
    currentButtons = BUTTONS;
      for (int i=0; i < NBUTTONS; i++) {
			  buttonJustReleased[i] = KEY_DOWN(i, BUTTONS) == 0 && buttonWasDown[i];
			  buttonWasDown[i] = KEY_DOWN(i, BUTTONS) != 0;
		}
    /* TODO: */
    // Manipulate the state machine below as needed //
    // NOTE: Call waitForVBlank() before you draw

    switch (state) {
      case START:

        waitForVBlank();
        state = START;

        char buffer[51];

        if (startFLAG == 0) {
          drawFullScreenImageDMA(moon);
          startFLAG = 1;
			    sprintf(buffer, "INSERT COINS TO PLAY");
			    drawString(80, 60, buffer, GREEN);
        }

			  if (buttonJustReleased[BUTTON_START]) {
				  state = PLAY;
        }
			  break;

      case PLAY:
        waitForVBlank();

        state = PLAY;
        // if (playFLAG == 0) {
          drawFullScreenImageDMA(playScreen);
          //drawRectDMA(60, 0, 240, 3, CYAN);
          //playFLAG = 1;
        //}

        drawImageDMA(al.x, al.y, 8, 4, alien2);

        handle_buttons(&al);
        updateAlienPosition(&al);

        // OUT OF FUEL TEXT
        if (fuel > 0 ){
          sprintf(buffer, "FUEL: %d", fuel);
          drawRectDMA(150, 5, 200, 10, BLACK);
          drawString(150, 5, buffer, MAGENTA);
        } else {
          sprintf(buffer, "OUT OF FUEL");
          drawRectDMA(150, 5, 200, 10, BLACK);
          drawString(150, 5, buffer, RED);
        }

        if (al.x < 68 || al.x == 130 ) {
          delay(30);
          state = LOSE;
        }

        //box 1
        if (al.y > 64 && al.y < 89 && al.x < 100 && al.x > 71) {
          delay(30);
          state = LOSE;
        }

        //box 2
        if (al.y > 65 && al.y < 87 && al.x > 118 && al.x < 131) {
          delay(30);
          state = LOSE;
        }

        //box 3
        if (al.y > 124 && al.y < 159 && al.x > 86 && al.x < 126) {
          delay(30);
          state = LOSE;
        }

        //box 4
        if (al.y > 180 && al.y < 214 && al.x > 70 && al.x < 97) {
          delay(30);
          state = LOSE;
        }


        // winning box
        if (al.x == 126 && (al.y >= 213 && al.y <= 228)) {
          delay(30);
          state = WIN;
        }

        if (buttonJustReleased[BUTTON_SELECT]) {
          fuel = 100;
          al.x = 70;
          al.y = 0;
          FLOOR_Y = 160-16;
          playFLAG = 0;
          startFLAG = 0;
          state = START;
        }

        break;

      case WIN:

        waitForVBlank();
        state = WIN;
         if (winFLAG == 0) {
          drawFullScreenImageDMA(end);
			    sprintf(buffer, "end...");
			    drawString(150, 5, buffer, GREEN);

          sprintf(buffer, "CONGRATS!");
          drawRectDMA(48, 30, 200, 15, BLACK);
          drawString(60, 100, buffer, GREEN);
          winFLAG = 1;
        }


			  if (buttonJustReleased[BUTTON_SELECT]) {
          fuel = 100;
          al.x = 70;
          al.y = 0;
          FLOOR_Y = 160-16;
				  state = START;
          winFLAG = 0;
          playFLAG = 0;
          startFLAG = 0;
        }

        // state = ?
        break;

      case LOSE:

        waitForVBlank();
        state = LOSE;

        if (loseFLAG == 0) {
        drawFullScreenImageDMA(lose);
          loseFLAG = 1;
        }

        sprintf(buffer, "SPACESHIP CRASHED!");
        drawCenteredString(48, 30, 200, 15, buffer, RED);
        sprintf(buffer, "You created a two mile crater!");
        drawCenteredString(70, 30, 200, 15, buffer, GREEN);

			  if (buttonJustReleased[BUTTON_SELECT]) {
          fuel = 100;
          al.x = 70;
          al.y = 0;
          FLOOR_Y = 160-16;
				  state = START;
          loseFLAG = 0;
          playFLAG = 0;
          startFLAG = 0;
        }

        break;
    }

    previousButtons = currentButtons; // Store the current state of the buttons
  }

  UNUSED(previousButtons); // You can remove this once previousButtons is used

  return 0;
}


/* handle the buttons which are pressed down */
void handle_buttons(struct alien *al) {

    // if (button_pressed(BUTTON_DOWN)) {
    //     al->x += 1;
    // }
    // if (button_pressed(BUTTON_UP)) {
    //     al->x -= 1;
    // }
    if (button_pressed(BUTTON_RIGHT)) {
      drawRectDMA(al->x - 1, al->y - 1, 3, 3, BLACK);
      al->y += 1;
    }
    if (button_pressed(BUTTON_LEFT)) {
      drawRectDMA(al->x - 1, al->y + 8 , 1, 5, BLACK);
      al->y -= 1;
    }
}

/* this function checks whether a particular button has been pressed */
unsigned char button_pressed(unsigned short button) {
    /* and the button register with the button constant we want */
    unsigned short pressed = *buttons & button;
    /* if this value is zero, then it's not pressed */
    if (pressed == 0) {
        return 1;
    } else {
        return 0;
    }
}
// used to calc gravity and handle boost
void updateAlienPosition(struct alien *al) {

  int isMidAir = al->x != FLOOR_Y;

  if (button_pressed(BUTTON_R) && fuel > 0) {
      al->velX = JUMP;
      al->time = 0;
      fuel--;
  }

    if (isMidAir) {
      al->velX = JUMP + (GRAVITY * al->time);
      al->velX = min(1, al->velX);
      al->time++;
    }

    al->y += al->velY;
    // clamp to screen?
    al->y = min(240-16, al->y);
    al->y = max(0, al->y);

    al->x += al->velX;
    al->x = min(al->x, FLOOR_Y);

}

void delay(int n) {
        // delay for n tenths of a second
        volatile int x = 0;
        for (int i=0; i<n*8000; i++)
                x++;
}

