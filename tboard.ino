// Created 26 May 2017

#include "TetrisBoard.h"
#include <OctoWS2811.h>

#define LIGHT_LEVEL 5

#define COLS_LEDs 60 // all of the following params need to be adjusted for screen size
#define ROWS_LEDs 36  // LED_LAYOUT assumed 0 if ROWS_LEDs > 8
#define ROWS_PER_STRIP 12

#define STRIPS ((ROWS_LEDs+ROWS_PER_STRIP-1) / ROWS_PER_STRIP)
#define LEDS_PER_STRIP (COLS_LEDs*ROWS_PER_STRIP)

DMAMEM int displayMemory[LEDS_PER_STRIP*6];
int drawingMemory[LEDS_PER_STRIP*6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(LEDS_PER_STRIP, displayMemory, drawingMemory, config);

TetrisBoard board = TetrisBoard();

int dimmer(int color, int percent) {
  int r = (color & 0xFF0000) >> 16;
  int g = (color & 0x00FF00) >> 8;
  int b = color & 0xFF;

  r = (r*percent)/100;
  g = (g*percent)/100;
  b = (b*percent)/100;

  int output = (r<<16) + (g<<8) + b;
  return(output);
}

void setup()
{
  pinMode(13, OUTPUT);
  leds.begin();
  leds.show();
}

void loop()
{
  // piece type 5 in default rotation, at loc 10x20, color=3
  board.placeTet(5, 2, 20, 3); 	
  Serial.println("placed at 2, 20");
  board.dump();
  board.freezeTet();
  while(1);
  board.dump();
  board.placeTet(4, 10, 20, 2);
  board.dump();
  board.rotTet();
  board.dump();
  bool success = board.moveTet(0, -1);
  if (success) {
    Serial.print("ok: ");
  } else {
    Serial.print("xx: ");
  }
  Serial.println("moved down 1");
  board.dump();
  while(1);
}

int led_map(int x,int y) {
  int output; 

  x = (COLS_LEDs-1) - x;
  y = (ROWS_LEDs-1) - y;
  
  if (y%2 == 1) {
    output = ((y+1)*COLS_LEDs-x-1);
  } else {
    output = (y*COLS_LEDs+x);
  }
  
  return output;
}

void setxy(int x, int y, uint32_t c) {
  leds.setPixel(led_map(x, y), dimmer(c, LIGHT_LEVEL));
}





