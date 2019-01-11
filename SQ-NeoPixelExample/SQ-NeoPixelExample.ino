#include <Adafruit_NeoPixel.h>

const int PIN = 6;
const int NUM_LEDS = 240;
const int BRIGHTNESS =  25;
const int NeoPixelType = NEO_GRB; // NEO_GRB or NEO_GRBW

const int wipeDelay = 0;
const int whiteOverRainbowWait = 1;   // org 20
const int whiteOverRainbowSpeed = 1;  // org 75
const int pulseWhiteDelay = 0;
const int rainbowToWhiteDelay = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NeoPixelType + NEO_KHZ800);

int gamma[] = 
{
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 
};

void setup() 
{
  Serial.begin(115200);
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() 
{
  // Some example procedures showing how to display to the pixels:
  Serial.println("All Red!");
  colorWipe(strip.Color(255, 0, 0), wipeDelay);
  
  Serial.println("All Green!");
  colorWipe(strip.Color(0, 255, 0), wipeDelay);
  
  Serial.println("All Blue!");
  colorWipe(strip.Color(0, 0, 255), wipeDelay);
  
  Serial.println("All White!");
  colorWipe(getWhiteColor(255), wipeDelay);

  Serial.println("White Over Rainbow!");
  whiteOverRainbow(whiteOverRainbowWait,whiteOverRainbowSpeed,5);  

  Serial.println("Pulse White!");
  for (int i=0; i<2; i++)
    pulseWhite(pulseWhiteDelay); 

  //Serial.println("Full White!");
  //fullWhite();
  //delay(2000);

  Serial.println("Rainbow Fade To White!");
  rainbowFade2White(3,3,1);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) 
{
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void pulseWhite(uint8_t wait) 
{
  for(int j = 0; j < 256 ; j++)
  {
    for(uint16_t i=0; i<strip.numPixels(); i++) 
    {
      strip.setPixelColor(i, getWhiteColor(gamma[j]) );
    }
    strip.show();
    delay(wait);
  }
  
  for(int j = 255; j >= 0 ; j--)
  {
    for(uint16_t i=0; i<strip.numPixels(); i++) 
    {
      strip.setPixelColor(i, getWhiteColor(gamma[j]) );
    }
    strip.show();
  }
}


void rainbowFade2White(uint8_t wait, int rainbowLoops, int whiteLoops) 
{
  float fadeMax = 100.0;
  int fadeVal = 0;
  uint32_t wheelVal;
  int redVal, greenVal, blueVal;

  for(int k = 0 ; k < rainbowLoops ; k ++)
  {
    // 5 cycles of all colors on wheel
    for(int j=0; j<256; j++) 
    {
      for(int i=0; i< strip.numPixels(); i++) 
      {
        wheelVal = Wheel(((i * 256 / strip.numPixels()) + j) & 255);
      
        redVal   = red(wheelVal)   * float(fadeVal/fadeMax);
        greenVal = green(wheelVal) * float(fadeVal/fadeMax);
        blueVal  = blue(wheelVal)  * float(fadeVal/fadeMax);
      
        strip.setPixelColor( i, strip.Color( redVal, greenVal, blueVal ) );
      }
      
      //First loop, fade in!
      if(k == 0 && fadeVal < fadeMax-1) 
        fadeVal++;
      else if(k == rainbowLoops - 1 && j > 255 - fadeMax )
        fadeVal--; //Last loop, fade out!
    
      strip.show();
      delay(wait);
    }
  }
  delay(500);

  for(int k = 0 ; k < whiteLoops ; k ++)
  {
    for(int j = 0; j < 256 ; j++)
    {
      for(uint16_t i=0; i < strip.numPixels(); i++) 
      {
        strip.setPixelColor(i, getWhiteColor(i));
      }
      strip.show();
    }
    delay(2000);
    for(int j = 255; j >= 0 ; j--)
    {
      for(uint16_t i=0; i < strip.numPixels(); i++) 
      {
          strip.setPixelColor(i, getWhiteColor(gamma[j]));
      }
      strip.show();
    }
  }
  delay(500);
}

void whiteOverRainbow(uint8_t wait, uint8_t whiteSpeed, uint8_t whiteLength ) 
{
  if(whiteLength >= strip.numPixels()) 
    whiteLength = strip.numPixels() - 1;

  int head = whiteLength - 1;
  int tail = 0;

  int loops = 3;
  int loopNum = 0;

  static unsigned long lastTime = 0;

  while(true)
  {
    for(int j=0; j<256; j++) 
    {
      for(uint16_t i=0; i<strip.numPixels(); i++) 
      {
        if((i >= tail && i <= head) || (tail > head && i >= tail) || (tail > head && i <= head) )
          strip.setPixelColor(i, getWhiteColor(255) );
        else
          strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
      }

      if(millis() - lastTime > whiteSpeed) 
      {
        head++;
        tail++;
        if(head == strip.numPixels())
          loopNum++;
        lastTime = millis();
      }

      if(loopNum == loops) return;
    
      head%=strip.numPixels();
      tail%=strip.numPixels();
      strip.show();
      delay(wait);
    }
  }
}

void fullWhite() 
{
    for(uint16_t i=0; i<strip.numPixels(); i++) 
    {
        strip.setPixelColor(i, getWhiteColor(255) );
    }
    strip.show();
}


// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) 
{
  uint16_t i, j;
  
  // 5 cycles of all colors on wheel
  for(j=0; j<256 * 5; j++) 
  {
    for(i=0; i< strip.numPixels(); i++) 
    {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) 
{
  uint16_t i, j;
  for(j=0; j<256; j++) 
  {
    for(i=0; i<strip.numPixels(); i++) 
    {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) 
{
  WheelPos = 255 - WheelPos;
  
  if(WheelPos < 85) 
  {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3,0);
  }
  
  if(WheelPos < 170) 
  {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3,0);
  }

  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0,0);
}

uint8_t red(uint32_t c) 
{
  return (c >> 8);
}

uint8_t green(uint32_t c) 
{
  return (c >> 16);
}

uint8_t blue(uint32_t c) 
{
  return (c);
}

// return white color value
uint32_t getWhiteColor(int gamma)
{
  if (NeoPixelType == NEO_GRBW)
    return strip.Color(0, 0, 0, gamma);
  else
    return strip.Color(gamma, gamma, gamma);
}


