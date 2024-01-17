/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include <Arduino.h>
#include <FastLED.h>
#include <IRremote.hpp>
#include "main.h"

FASTLED_USING_NAMESPACE

#if defined(APPLICATION_PIN)
#define DEBUG_BUTTON_PIN    APPLICATION_PIN // if low, print timing for each received data set
#else
#define DEBUG_BUTTON_PIN   6
#endif


void setup()
{
#if FLASHEND >= 0x3FFF  // For 16k flash or more, like ATtiny1604. Code does not fit in program memory of ATtiny85 etc.
  pinMode(DEBUG_BUTTON_PIN, INPUT_PULLUP);
#endif
  D_SerialBegin(115200);
  D_println(F("Enabling IRRemote receive"));

  // Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  IrReceiver.registerReceiveCompleteCallback(ircallback);

  // D_print(F("Ready to receive IR signals of protocols: "));
  // printActiveIRProtocols(&Serial);
  // D_println(F("at pin " STR(IR_RECEIVE_PIN)));

  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER, DATA_RATE_KHZ(250000)>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  // our default color correction and temperature
  FastLED.setCorrection(TypicalPixelString);
  FastLED.setTemperature(UncorrectedTemperature);

  // start with nothing lit
  FastLED.clear();
  FastLED.show();
}

void loop()
{
  if (add_glitter && !loop_running) {
    fill_solid(leds, NUM_LEDS, current_color);
    addGlitter(80);
    FastLED.show();
  } else if (play_loop && !loop_running) {
    ledloop();
  } else if (play_loop_rainbow && !loop_running) {
    while (play_loop_rainbow) {
      loop_running = true;
      rainbow();
      FastLED.show();
      EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow/
    }
  } else if (play_loop_rainbowglitter && !loop_running) {
    while (play_loop_rainbowglitter) {
      loop_running = true;
      rainbowWithGlitter();
      FastLED.show();
      EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow/
    }
  } else if (play_loop_confetti && !loop_running) {
    while (play_loop_confetti) {
      loop_running = true;
      color = rgb2hsv_approximate(current_color);
      gHue = color.h;
      confetti();
      FastLED.show();
    }
  } else if (play_loop_sinelon && !loop_running) {
    while (play_loop_sinelon) {
      loop_running = true;
      color = rgb2hsv_approximate(current_color);
      gHue = color.h;
      sinelon();
      FastLED.show();
    }
  } else if (play_loop_juggle && !loop_running) {
    while (play_loop_juggle) {
      loop_running = true;
      color = rgb2hsv_approximate(current_color);
      gHue = color.h;
      juggle();
      FastLED.show();
    }
  } else if (play_loop_bpm && !loop_running) {
    while (play_loop_bpm) {
      loop_running = true;
      color = rgb2hsv_approximate(current_color);
      gHue = color.h;
      bpm();
      FastLED.show();
    }
  }
}


/* LED Helper functions*/
void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns );
}

void addGlitter(fract8 chanceOfGlitter)
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}


/* LED functions */

void ledson() {
  fill_solid(leds, NUM_LEDS, CRGB::White);
  FastLED.setBrightness(255);
  FastLED.show();
}

void ledsoff() {
  fadeUsingColor(leds, NUM_LEDS, CRGB::Black);
  fadeToBlackBy(leds, NUM_LEDS, 100);
  FastLED.setBrightness(0);
  FastLED.show();
}

void setcolor(long command) {
    power = true;
    switch (command) {
      case red_full:
        current_color = CRGB::Red;
        break;
      case green_full:
        current_color = CRGB::Lime;
        break;
      case blue_full:
        current_color = CRGB::Blue;
        break;
      case white:
        current_color = CRGB::White;
        break;
      case orange:
        current_color = CRGB::Orange;
        break;
      case green:
        current_color = CRGB::Green;
        break;
      case blue:
        current_color = CRGB::MediumBlue;
        break;
      case pink_one:
        current_color = CRGB::Pink;
        break;
      case red:
        current_color = CRGB::Tomato;
        break;
      case light_blue:
        current_color = CRGB::LightBlue;
        break;
      case purple:
        current_color = CRGB::Purple;
        break;
      case pink_two:
        current_color = CRGB::HotPink;
        break;
      case light_orange:
        current_color = CRGB::Salmon;
        break;
      case teal:
        current_color = CRGB::Teal;
        break;
      case cherry_red:
        current_color = CRGB::Maroon;
        break;
      case baby_blue_one:
        current_color = CRGB::SkyBlue;
        break;
      case yellow:
        current_color = CRGB::Yellow;
        break;
      case dark_blue:
        current_color = CRGB::SteelBlue;
        break;
      case ruby_red:
        current_color = CRGB::MediumVioletRed;
        break;
      case baby_blue_two:
        current_color = CRGB::BlueViolet;
        break;
    }
    fill_solid(leds, NUM_LEDS, current_color);
    FastLED.show();
    printColorBrightness();
}


/* LED Loop functions */

void rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter()
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy(leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV(gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  uint8_t dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void ledloop() {
  while (play_loop) {
    loop_running = true;
    // Call the current pattern function once, updating the 'leds' array
    gPatterns[gCurrentPatternNumber]();

    // send the 'leds' array out to the actual LED strip
    FastLED.show();
    // insert a delay to keep the framerate modest
    FastLED.delay(1000/FRAMES_PER_SECOND);

    // do some periodic updates
    EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow/
    EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
  }
}


/* IRRemote ISR callback function */
void ircallback() {
  IrReceiver.decode();  // decode command on isr callback
  long command = IrReceiver.decodedIRData.command;
  switch (command) {
    case on_off:
      power = !power;
      add_glitter = false;
      play_loop = false;
      loop_running = false;
      play_loop_rainbow = false;
      play_loop_rainbowglitter = false;
      play_loop_confetti = false;
      play_loop_sinelon = false;
      play_loop_juggle = false;
      play_loop_bpm = false;
      if ( power ) {
        ledson();
      } else {
        ledsoff();
      };
      break;
    case play_pause:
      power = true;
      add_glitter = false;
      loop_running = false;
      play_loop = false;
      play_loop_rainbow = false;
      play_loop_rainbowglitter = false;
      play_loop_confetti = false;
      play_loop_sinelon = false;
      play_loop_juggle = false;
      play_loop_bpm = false;
      break;
    case loop_rainbow:
      power = true;
      add_glitter = false;
      loop_running = false;
      play_loop = false;
      play_loop_rainbow = true;
      play_loop_rainbowglitter = false;
      play_loop_confetti = false;
      play_loop_sinelon = false;
      play_loop_juggle = false;
      play_loop_bpm = false;
      printLoopType(loop_rainbow);
      break;
    case loop_rainbowglitter:
      power = true;
      add_glitter = false;
      loop_running = false;
      play_loop = false;
      play_loop_rainbow = false;
      play_loop_rainbowglitter = true;
      play_loop_confetti = false;
      play_loop_sinelon = false;
      play_loop_juggle = false;
      play_loop_bpm = false;
      printLoopType(loop_rainbowglitter);
      break;
    case loop_confetti:
      power = true;
      add_glitter = false;
      loop_running = false;
      play_loop = false;
      play_loop_rainbow = false;
      play_loop_rainbowglitter = false;
      play_loop_confetti = true;
      play_loop_sinelon = false;
      play_loop_juggle = false;
      play_loop_bpm = false;
      printLoopType(loop_confetti);
      break;
    case loop_sinelon:
      power = true;
      add_glitter = false;
      loop_running = false;
      play_loop = false;
      play_loop_rainbow = false;
      play_loop_rainbowglitter = false;
      play_loop_confetti = false;
      play_loop_sinelon = true;
      play_loop_juggle = false;
      play_loop_bpm = false;
      printLoopType(loop_sinelon);
      break;
    case loop_juggle:
      power = true;
      add_glitter = false;
      loop_running = false;
      play_loop = false;
      play_loop_rainbow = false;
      play_loop_rainbowglitter = false;
      play_loop_confetti = false;
      play_loop_sinelon = false;
      play_loop_juggle = true;
      play_loop_bpm = false;
      printLoopType(loop_juggle);
      break;
    case loop_bpm:
      power = true;
      add_glitter = false;
      loop_running = false;
      play_loop = false;
      play_loop_rainbow = false;
      play_loop_rainbowglitter = false;
      play_loop_confetti = false;
      play_loop_sinelon = false;
      play_loop_juggle = false;
      play_loop_bpm = true;
      printLoopType(play_loop_bpm);
      break;
    case red_full:
    case green_full:
    case blue_full:
    case white:
    case orange:
    case green:
    case blue:
    case pink_one:
    case red:
    case light_blue:
    case purple:
    case pink_two:
    case light_orange:
    case teal:
    case cherry_red:
    case baby_blue_one:
    case yellow:
    case dark_blue:
    case ruby_red:
    case baby_blue_two:
      setcolor(command);
      break;
    case brightness_up:
      if (brightness < 255) {
        brightness += step;
        FastLED.setBrightness(brightness);
        FastLED.show();
        printColorBrightness();
      }
      break;
    case brightness_down:
      if (brightness > 0) {
        brightness -= step;
        FastLED.setBrightness(brightness);
        FastLED.show();
        printColorBrightness();
      }
      break;
    case red_up:
      current_color = leds[ 1 ];
      if (current_color.r < 255) {
        current_color.r += step;
        fill_solid(leds, NUM_LEDS, current_color);
        FastLED.show();
        printColorBrightness();
      }
      break;
    case red_down:
      current_color = leds[ 1 ];
      if (current_color.r > 0) {
        current_color.r -= step;
        fill_solid(leds, NUM_LEDS, current_color);
        FastLED.show();
        printColorBrightness();
      }
      break;
    case green_up:
      current_color = leds[ 1 ];
      if (current_color.g < 255) {
        current_color.g += step;
        fill_solid(leds, NUM_LEDS, current_color);
        FastLED.show();
        printColorBrightness();
      }
      break;
    case green_down:
      current_color = leds[ 1 ];
      if (current_color.g > 0) {
        current_color.g -= step;
        fill_solid(leds, NUM_LEDS, current_color);
        FastLED.show();
        printColorBrightness();
      }
      break;
    case blue_up:
      current_color = leds[ 1 ];
      if (current_color.b < 255) {
        current_color.b += step;
        fill_solid(leds, NUM_LEDS, current_color);
        FastLED.show();
        printColorBrightness();
      }
      break;
    case blue_down:
      current_color = leds[ 1 ];
      if (current_color.b > 0) {
        current_color.b -= step;
        fill_solid(leds, NUM_LEDS, current_color);
        FastLED.show();
        printColorBrightness();
      }
      break;
    case quick:
      add_glitter = true;
      break;
    case slow:  // TODO: Finish
      break;
    case autob:
      power = true;
      loop_running = false;
      play_loop = true;
      play_loop_rainbow = false;
      play_loop_rainbowglitter = false;
      play_loop_confetti = false;
      play_loop_sinelon = false;
      play_loop_juggle = false;
      play_loop_bpm = false;
      printLoopType(play_pause);
    case flash:  // TODO: Finish
      break;
    case jump3:  // TODO: Finish
      break;
    case jump7:  // TODO: Finish
      break;
    case fade3:  // TODO: Finish
      break;
    case fade7:  // TODO: Finish
      break;
    default:
      printCommand();
      break;
  };
  IrReceiver.resume();  // resume receiving for callback
}


/* Print functions  */
void printColorBrightness() {
  current_color = leds[ 1 ];
  D_print("RGB:  ");
  D_print(current_color.r);
  D_print(", ");
  D_print(current_color.g);
  D_print(", ");
  D_print(current_color.b);
  D_print(" BRIGHTNESS:   ");
  D_println(brightness);
}

void printLoopType(long looptype) {
  switch (looptype) {
    case play_pause:
      D_println("LOOP ALL");
      break;
    case loop_rainbow:
      D_println("LOOP RAINBOW");
      break;
    case loop_rainbowglitter:
      D_println("LOOP RAINBOWGLITTER");
      break;
    case loop_confetti:
      D_println("LOOP CONFETTI");
      break;
    case loop_sinelon:
      D_println("LOOP SINELON");
      break;
    case loop_juggle:
      D_println("LOOP JUGGLE");
      break;
    case loop_bpm:
      D_println("LOOP BPM");
      break;
    case loop_fade:
      D_println("LOOP FADE");
      break;
  }
}

void printCommand() {
  D_print(F("Command 0x"));
  D_print(IrReceiver.decodedIRData.command, HEX); // Print new command data
  D_print(F(" RAW-DATA 0x"));
  D_println(IrReceiver.decodedIRData.decodedRawData, HEX); // Print "old" raw data
}
