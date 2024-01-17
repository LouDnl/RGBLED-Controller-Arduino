#ifndef NanoLed_h
#define NanoLed_h

#include "pixeltypes.h"

/* Debugging */
#define DEBUG            1    // SET TO 0 OUT TO REMOVE TRACES
#define SERIAL_BAUD 115200

#if DEBUG
#define D_SerialBegin(...) Serial.begin(__VA_ARGS__);
#define D_print(...)    Serial.print(__VA_ARGS__)
#define D_write(...)    Serial.write(__VA_ARGS__)
#define D_println(...)  Serial.println(__VA_ARGS__)
#else
#define D_SerialBegin(bauds)
#define D_print(...)
#define D_write(...)
#define D_println(...)
#endif

/* LEDS */
#define LED_TYPE        WS2801
#define DATA_PIN             8
#define CLOCK_PIN            9
#define COLOR_ORDER        BGR
#define DATA_RATE       250000
#define NUM_LEDS            96  // 128

#define UPDATES_PER_SECOND 100
#define BRIGHTNESS          95
#define BRIGHTNESS_MAX     255
#define BRIGHTNESS_MIN       0
#define GLITTER_PERCENTAGE  80
#define N_SECONDS           10
#define N_MILLIS            20
#define FRAMES_PER_SECOND  120
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
CRGB leds[NUM_LEDS];

/* IR RECEIVER */
#define IR_RECEIVE_PIN      2 // To be compatible with interrupt example, pin 2 is chosen here.
#define IR_SEND_PIN         3
#define TONE_PIN            4
#define APPLICATION_PIN     5
#define ALTERNATIVE_IR_FEEDBACK_LED_PIN 6 // E.g. used for examples which use LED_BUILDIN for example output.
#define _IR_TIMING_TEST_PIN 7

/* Pre declare functions */

// led strip
void nextPattern();
void rainbow();
void addGlitter(fract8 chanceOfGlitter);
void rainbowWithGlitter();
void confetti();
void sinelon();
void juggle();
void bpm();
void fadeColor();
void flashColor();
void setcolor(long command);

void ledson();
void ledsoff();
void ledloop();

void printColorBrightness();
void printLoopType(long looptype);
void printCommand();

// ir remote
void ircallback();


/* Pre declare vars */

// led strip vars
uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };

// struct rgb_t
// {
//     uint8_t red;
//     uint8_t green;
//     uint8_t blue;
// };

const LEDColorCorrection corrections[5] =
{
    TypicalSMD5050,
    TypicalLEDStrip,
    Typical8mmPixel,
    TypicalPixelString,
    UncorrectedColor,
};

const ColorTemperature temperatures[20] =
{
    Candle,
    Tungsten40W,
    Tungsten100W,
    Halogen,
    CarbonArc,
    HighNoonSun,
    DirectSunlight,
    OvercastSky,
    ClearBlueSky,
    WarmFluorescent,
    StandardFluorescent,
    CoolWhiteFluorescent,
    FullSpectrumFluorescent,
    GrowLightFluorescent,
    BlackLightFluorescent,
    MercuryVapor,
    SodiumVapor,
    MetalHalide,
    HighPressureSodium,
    UncorrectedTemperature,
};

// ir remote vars
#define RAMSIZE 0xFFFF // Dummy value for platforms where RAMSIZE is not defined

volatile bool sIRDataJustReceived = false;
volatile bool power                     = false;
volatile bool play_loop                 = false;
volatile bool play_loop_rainbow         = false;
volatile bool play_loop_rainbowglitter  = false;
volatile bool play_loop_confetti        = false;
volatile bool play_loop_sinelon         = false;
volatile bool play_loop_juggle          = false;
volatile bool play_loop_bpm             = false;
volatile bool loop_running              = false;
volatile bool add_glitter               = false;  // glitter loop
volatile bool fade_color                = false;  // fade loop
volatile bool fade_up_fade_down         = false;  // true is up, false is down
volatile bool flash_color               = false;  // flash loop
volatile bool flashing                  = false;  // true is on, false is off
volatile int brightness                 = BRIGHTNESS;
CHSV color;
CRGB current_color;
const int step = 5;
const int fade_step = 1;
const int fade_delay = 3;
const int flash_delay = 100;

// ir remote button command values
const long on_off          = 0x40;
const long play_pause      = 0x41;

const long brightness_up   = 0x5C;
const long brightness_down = 0x5D;

const long red_full        = 0x58;
const long green_full      = 0x59;
const long blue_full       = 0x45;
const long white           = 0x44;
const long orange          = 0x54;
const long green           = 0x55;
const long blue            = 0x49;
const long pink_one        = 0x48;
const long red             = 0x50;
const long light_blue      = 0x51;
const long purple          = 0x4D;
const long pink_two        = 0x4C;
const long light_orange    = 0x1C;
const long teal            = 0x1D;
const long cherry_red      = 0x1E;
const long baby_blue_one   = 0x1F;
const long yellow          = 0x18;
const long dark_blue       = 0x19;
const long ruby_red        = 0x1A;
const long baby_blue_two   = 0x1B;

const long red_up          = 0x14;
const long red_down        = 0x10;
const long green_up        = 0x15;
const long green_down      = 0x11;
const long blue_up         = 0x16;
const long blue_down       = 0x12;

const long diy1            = 0xC;
const long diy2            = 0xD;
const long diy3            = 0xE;
const long diy4            = 0x8;
const long diy5            = 0x9;
const long diy6            = 0xA;

const long quick           = 0x17;
const long slow            = 0x13;
const long autob           = 0xF;
const long flash           = 0xB;
const long jump3           = 0x4;
const long jump7           = 0x5;
const long fade3           = 0x6;
const long fade7           = 0x7;

// global vars
const long loop_all             = autob;
const long loop_rainbow         = diy1;
const long loop_rainbowglitter  = diy2;
const long loop_confetti        = diy3;
const long loop_sinelon         = diy4;
const long loop_juggle          = diy5;
const long loop_bpm             = diy6;
const long loop_fade            = slow;
const long loop_flash           = flash;
const long loop_glitter         = quick;

#endif
