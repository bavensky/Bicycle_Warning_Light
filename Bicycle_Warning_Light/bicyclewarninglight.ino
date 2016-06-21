#include <Adafruit_NeoPixel.h>

#define PIN 3
#define R_LASER 5
#define L_LASER 6
#define BUTTON 8

Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIN, NEO_GRB + NEO_KHZ800);

byte buttonState = 0;
int mode = 0;

void setup() {
  strip.begin();
  strip.show();
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(R_LASER, OUTPUT);
  pinMode(L_LASER, OUTPUT);
}

void loop() {
  buttonState = digitalRead(BUTTON);
  if (buttonState == 0) {
    delay(200);
    mode++;
  }

  if (mode == 0)  {
    colorWipe(strip.Color(0, 0, 0), 0); // Red
    digitalWrite(R_LASER, LOW);
    digitalWrite(L_LASER, LOW);
  }
  if (mode == 1)  {
    colorWipe(strip.Color(127, 0, 0), 0); // Red
    digitalWrite(R_LASER, HIGH);
    digitalWrite(L_LASER, HIGH);
  }
  if (mode == 2)
    theaterChase(strip.Color(127, 0, 0), 50);
  if (mode == 3)
    mode = 0;
}

void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    buttonState = digitalRead(BUTTON);
    if (buttonState == 0) {
      delay(200);
      mode++;
    }
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;
  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      buttonState = digitalRead(BUTTON);
      if (buttonState == 0) {
        delay(200);
        mode++;
      }
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  buttonState = digitalRead(BUTTON);
  if (buttonState == 0) {
    delay(200);
    mode++;
  }
  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    buttonState = digitalRead(BUTTON);
    if (buttonState == 0) {
      delay(200);
      mode++;
    }
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    buttonState = digitalRead(BUTTON);
    if (buttonState == 0) {
      delay(200);
      mode++;
    }
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
        buttonState = digitalRead(BUTTON);
        if (buttonState == 0) {
          delay(200);
          mode++;
        }
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        buttonState = digitalRead(BUTTON);
        if (buttonState == 0) {
          delay(200);
          mode++;
        }
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
