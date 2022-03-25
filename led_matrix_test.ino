
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

// Defining size, and output pins
#define MAX_DEVICES 4
#define CS_PIN 10

// Create a new instance of the MD_Parola class with hardware SPI connection
MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// Messages in strings
uint8_t  curString = 0;
const char *msg[] =
{
  "*** GOLDSCREW ***",
  "Welcome to GoldScrew channel",
  "Like & Share",
  "SUBSCRIBE",
  "Hope you enjoy ^_^",    
  "LIKE",
  "SHARE"
};

// Pause time
const uint16_t PAUSE_TIME = 1000; // in milliseconds

// Sprite Definitions
const uint8_t F_PMAN1 = 6;
const uint8_t W_PMAN1 = 8;
const uint8_t PROGMEM pacman1[F_PMAN1 * W_PMAN1] =  // gobbling pacman animation
{
  0x00, 0x81, 0xc3, 0xe7, 0xff, 0x7e, 0x7e, 0x3c,
  0x00, 0x42, 0xe7, 0xe7, 0xff, 0xff, 0x7e, 0x3c,
  0x24, 0x66, 0xe7, 0xff, 0xff, 0xff, 0x7e, 0x3c,
  0x3c, 0x7e, 0xff, 0xff, 0xff, 0xff, 0x7e, 0x3c,
  0x24, 0x66, 0xe7, 0xff, 0xff, 0xff, 0x7e, 0x3c,
  0x00, 0x42, 0xe7, 0xe7, 0xff, 0xff, 0x7e, 0x3c,
};

const uint8_t F_PMAN2 = 6;
const uint8_t W_PMAN2 = 18;
const uint8_t PROGMEM pacman2[F_PMAN2 * W_PMAN2] =  // ghost pursued by a pacman
{
  0x00, 0x81, 0xc3, 0xe7, 0xff, 0x7e, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
  0x00, 0x42, 0xe7, 0xe7, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
  0x24, 0x66, 0xe7, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
  0x3c, 0x7e, 0xff, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
  0x24, 0x66, 0xe7, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
  0x00, 0x42, 0xe7, 0xe7, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
};

const uint8_t F_ROCKET = 2;
const uint8_t W_ROCKET = 11;
const uint8_t PROGMEM rocket[F_ROCKET * W_ROCKET] =  // rocket
{
  0x18, 0x24, 0x42, 0x81, 0x99, 0x18, 0x99, 0x18, 0xa5, 0x5a, 0x81,
  0x18, 0x24, 0x42, 0x81, 0x18, 0x99, 0x18, 0x99, 0x24, 0x42, 0x99,
};

const uint8_t F_WALKER = 5;
const uint8_t W_WALKER = 7;
const uint8_t PROGMEM walker[F_WALKER * W_WALKER] =  // walking man
{
    0x00, 0x48, 0x77, 0x1f, 0x1c, 0x94, 0x68,
    0x00, 0x90, 0xee, 0x3e, 0x38, 0x28, 0xd0,
    0x00, 0x00, 0xae, 0xfe, 0x38, 0x28, 0x40,
    0x00, 0x00, 0x2e, 0xbe, 0xf8, 0x00, 0x00, 
    0x00, 0x10, 0x6e, 0x3e, 0xb8, 0xe8, 0x00,
};

const uint8_t F_HEART = 5;
const uint8_t W_HEART = 9;
const uint8_t PROGMEM heart[F_HEART * W_HEART] =  // beating heart
{
  0x0e, 0x11, 0x21, 0x42, 0x84, 0x42, 0x21, 0x11, 0x0e,
  0x0e, 0x1f, 0x33, 0x66, 0xcc, 0x66, 0x33, 0x1f, 0x0e,
  0x0e, 0x1f, 0x3f, 0x7e, 0xfc, 0x7e, 0x3f, 0x1f, 0x0e,
  0x0e, 0x1f, 0x33, 0x66, 0xcc, 0x66, 0x33, 0x1f, 0x0e,
  0x0e, 0x11, 0x21, 0x42, 0x84, 0x42, 0x21, 0x11, 0x0e,
};

void setup() {
  Serial.begin(9600);
  
  // Intialize the object
  myDisplay.begin();

  // Set the intensity (brightness) of the display (0-15)
  myDisplay.setIntensity(5);

  // Show scroll Text
  myDisplay.displayScroll(msg[curString], PA_LEFT, PA_SCROLL_LEFT, 100);
}

void loop() {
  
  // Random  
  int ranIdx = random(6);

//  Serial.println(String(ranIdx));
  
  static uint8_t  curFX = 0;
  
  if (myDisplay.displayAnimate()) {

    switch(ranIdx)
    {           
      case 0:
        myDisplay.displayText(msg[6], PA_CENTER, 100, PAUSE_TIME, PA_SPRITE, PA_SPRITE);
        myDisplay.setSpriteData(rocket, W_ROCKET, F_ROCKET, rocket, W_ROCKET, F_ROCKET);
        curString++;        
        break;
      case 1:
        myDisplay.displayScroll(msg[ranIdx], PA_LEFT, PA_SCROLL_LEFT, 100);        
        delay(2000);             
        break;
      case 2:
        myDisplay.displayScroll(msg[ranIdx], PA_RIGHT, PA_SCROLL_RIGHT, 100);        
        delay(2000);             
        break;
      case 3:
        myDisplay.displayText(msg[5], PA_CENTER, 100, PAUSE_TIME, PA_SPRITE, PA_SPRITE);
        myDisplay.setSpriteData(walker, W_WALKER, F_WALKER, walker, W_WALKER, F_WALKER);
        curString++;        
        break;
      case 4:        
        myDisplay.displayScroll(msg[ranIdx], PA_LEFT, PA_SCROLL_LEFT, 100);        
        delay(2000);         
        break;
      case 5:
        myDisplay.displayText(msg[ranIdx], PA_CENTER, 100, PAUSE_TIME, PA_SPRITE, PA_SPRITE);
        myDisplay.setSpriteData(pacman1, W_PMAN1, F_PMAN1, pacman2, W_PMAN2, F_PMAN2);
        curString++;        
        break;
      case 6:        
        myDisplay.displayScroll(msg[ranIdx], PA_CENTER, PA_SCROLL_UP, 100);        
        delay(2000);          
        break;
      default:
        myDisplay.displayText(msg[6], PA_CENTER, 100, PAUSE_TIME, PA_SPRITE, PA_SPRITE);
        myDisplay.setSpriteData(heart, W_HEART, F_HEART, rocket, W_HEART, F_HEART);
        curString++;        
        break;
    }    
  
    // Reset display      
    myDisplay.displayReset();
  } 
