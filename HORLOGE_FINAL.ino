
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "TimeManager.h"
#include <avr/sleep.h>


unsigned long lastDebounceTime = 0;  // Tijd van de laatste knopdruk
const unsigned long debounceDelay = 50;  // Debounce vertraging in milliseconden
byte btnNumber = 0;
bool btnPushed = false;
byte dimfactor = 1;

bool effectsEnabled = true;
int timeMillis = 15;               // 130 ms om 1 seconde te simuleren

const byte push1_PIN{ 8 }, push2_PIN{ 9 }, push3_PIN{ 10 }, push4_PIN{ 0 };  // push buttons pinnr's
const byte xtal1_PIN{ 5 }, xtal2_PIN{ 4 }, mosfet_PIN{ 1 }, led_PIN{ 7 };    // andere pinnr's
const byte aantal_LEDs{ 24 };                                                // aantal LEDs
byte stepSpeed[] = {100,70,55,30};                                            // snelheden effecten
byte speedPreset = 0;                                                       // om te itereren over clockpulses
byte *secPtr, *minPtr, *hrPtr;                                             // pointers naar seconden, minuten en uren
bool hrEffect{ false }, minEffect{ false };                                // bools voor effecten af te spelen
byte effectStep{ 0 };                                                      // effecten in max 256 stappen verdelen
byte R_min{ 0 }, G_min{ 0 }, B_min{ 0 }, R_hr{ 0 }, G_hr{ 0 }, B_hr{ 0 };  // RGB kleuren bytes
byte hrIndex{ 0 }, minIndex{ 0 };                    

byte colourcodes[] = {0, 64, 128, 192, 255};                      // index voor leds array

TimeManager timeMgr = TimeManager();  // tijdmanager object

// Maak een NeoPixel-object aan met de juiste pin en het juiste aantal pixels
Adafruit_NeoPixel strip(aantal_LEDs, led_PIN, NEO_RGB + NEO_KHZ800);


void setup() {
  // Zet de data pin van de LED-strip als output
  pinMode(led_PIN, OUTPUT);
  strip.begin();  // Initializeer de Neopixel-bibliotheek
  strip.show();   // Zet alle pixels uit bij het opstarten

  // pointers naar realtime tijdsvariabelen van timemanager
  secPtr = timeMgr.getSeconds();
  minPtr = timeMgr.getMinutes();
  hrPtr = timeMgr.getHours();

  // Buttons als input
  pinMode(push1_PIN, INPUT);
  pinMode(push2_PIN, INPUT);
  pinMode(push3_PIN, INPUT);
  pinMode(push4_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(push1_PIN), interruptPush1, RISING);
  attachInterrupt(digitalPinToInterrupt(push2_PIN), interruptPush2, RISING);
  attachInterrupt(digitalPinToInterrupt(push3_PIN), interruptPush3, RISING);
  attachInterrupt(digitalPinToInterrupt(push4_PIN), interruptPush4, RISING);
}

void MinuteEffect() {
  // uureffect

  for (byte i = 0; i < 3; i++) {
    for (byte j = 0; j < 3; j++) {
      //strip.fill(0); // alle LED waardes resetten naar zwart
      strip.setPixelColor(i, strip.Color(20 * j * j, 0, colourcodes[i + j]));
      strip.setPixelColor(i + 3, strip.Color(0, 50 * j * j, colourcodes[i + j]));
      strip.setPixelColor(i + 6, strip.Color(50 * j * j, 0, colourcodes[i + j]));
      strip.setPixelColor(i + 9, strip.Color(0, 50 * j * j, colourcodes[i + j]));
      strip.show();
      delay(stepSpeed[speedPreset]);
    }
  }
  for (byte i = 0; i < 3; i++) {
    for (byte j = 0; j < 3; j++) {
      strip.fill(0);  // alle LED waardes resetten naar zwart
      strip.setPixelColor(i + 12, strip.Color(62 * j * j, 55 * j * j, 255));
      strip.setPixelColor(i + 15, strip.Color(0, 62 * j * j, 200));
      strip.setPixelColor(i + 18, strip.Color(62 * j * j, 55 * j * j, 255));
      strip.setPixelColor(i + 21, strip.Color(0, 62 * j * j, 200));
      if (j == 0) {
        strip.setPixelColor(i + 0, strip.Color(180, 180, 180));
        strip.setPixelColor(i + 6, strip.Color(180, 180, 180));
        strip.setPixelColor(i + 3, strip.Color(180, 180, 180));
        strip.setPixelColor(i + 9, strip.Color(180, 180, 180));
      }
      strip.show();
      delay(stepSpeed[speedPreset]);
    }
  }
  minEffect = false;
  hrEffect = false;
  UpdateLEDs();
}

void HourEffect() {
  // minuuteffect
  for (byte i = 0; i < 6; i++) {
    strip.fill(0);  // alle LED waardes resetten naar zwart
    strip.setPixelColor(i, strip.Color(i * 20, 0, i * 51));
    strip.setPixelColor(12 - i, strip.Color(0, i * 51, i * 20));
    strip.setPixelColor(6, strip.Color(i * 51, i * 51, i * 51));
    strip.setPixelColor(18, strip.Color(i * 51, i * 51, i * 33));
    strip.show();
    delay(stepSpeed[speedPreset]);
  }
  strip.fill(0);
  strip.setPixelColor(6, strip.Color(255, 255, 255));
  strip.setPixelColor(18, strip.Color(255, 255, 185));
  strip.show();
  delay(stepSpeed[speedPreset]);
  for (byte i = 0; i < 6; i++) {
    strip.fill(0);  // alle LED waardes resetten naar zwart
    strip.setPixelColor((i + 1) + 18, strip.Color(100 - i * 20, 0, 255 - i * 51));
    strip.setPixelColor((i + 1) + 6, strip.Color(255 - i * 51, 255 - i * 51, 175 - i * 35));
    strip.setPixelColor(18 - (i + 1), strip.Color(0, 255 - i * 51, 100 - i * 20));
    strip.setPixelColor(6 - (i + 1), strip.Color(255 - i * 51, 255 - i * 51, 175 - i * 35));
    strip.setPixelColor(0, strip.Color(255 - i * 51, 255 - i * 51, 255 - i * 51));
    strip.setPixelColor(12, strip.Color(255 - i * 51, 255 - i * 51, 165 - i * 33));
    strip.show();
    delay(stepSpeed[speedPreset]);
  }
  delay(stepSpeed[speedPreset]);
  minEffect = false;
  hrEffect = false;
  UpdateLEDs();
}

void UpdateLEDs() {

  // ** STAP 1 : kleuren aanpassen **

  // minuten = overgang rood naar groen
  // uren = overgang groen naar rood
  R_min = colourcodes[(*minPtr % 5)];
  G_min = colourcodes[4 - (*minPtr % 5)];

  R_hr = map(*minPtr, 0, 59, 255, 0);
  G_hr = map(*minPtr, 0, 59, 0, 255);

  // ** STAP 2 : LED minuten en uren juiste index bepalen **
  // * UREN

  switch (*hrPtr) {
    case 1:
    case 13:
      hrIndex = 1;
      break;
    case 2:
    case 14:
      hrIndex = 2;
      break;
    case 3:
    case 15:
      hrIndex = 3;
      break;
    case 4:
    case 16:
      hrIndex = 4;
      break;
    case 5:
    case 17:
      hrIndex = 5;
      break;
    case 6:
    case 18:
      hrIndex = 6;
      break;
    case 7:
    case 19:
      hrIndex = 7;
      break;
    case 8:
    case 20:
      hrIndex = 8;
      break;
    case 9:
    case 21:
      hrIndex = 9;
      break;
    case 10:
    case 22:
      hrIndex = 10;
      break;
    case 11:
    case 23:
      hrIndex = 11;
      break;
    default:
      hrIndex = 0;
      break;
  }

  // * MINUTEN

  // delen door 5, afronding naar beneden en +12 voor juiste index van de LED
  minIndex = (*minPtr / 5) + 12;

  // ** STAP 3 : LED data samenstellen en verzenden **

  strip.fill(0);  // alle LED waardes resetten naar zwart
  // juiste dimfactor setting toepassen
  R_min = (byte)(R_min / dimfactor);
  G_min = (byte)(G_min / dimfactor);
  B_min = (byte)(B_min / dimfactor);
  R_hr = (byte)(R_hr / dimfactor);
  G_hr = (byte)(G_hr / dimfactor);
  B_hr = (byte)(B_hr / dimfactor);

  strip.setPixelColor(minIndex, strip.Color(R_min, G_min, B_min));
  strip.setPixelColor(hrIndex, strip.Color(R_hr, G_hr, B_hr));
  strip.show();
}

void loop() {
  if(btnPushed) {
    btnPushed = false;

    switch(btnNumber) {
      case 1:
      // logica push1: KNOP WERKT NIET
 
      break;
      case 2:
      // logica push2: dimfactor bijwerken
      // geeft 100%, 50%, 33%, 25%, 20% brightness
      if (dimfactor <= 4) dimfactor++;
      else dimfactor = 1;
      break;
      case 3:
      // logica push3: timefactor bijwerken
      // snelheid horloge aanpasbaar
      // 1000ms , 500ms, 250ms, 100ms, 25ms
      if (speedPreset < 3) speedPreset++;
      else speedPreset = 0;
      break;
      case 4:
      // logica push4
      effectsEnabled = !effectsEnabled;
      break;
      default:
      // logica default, niks doen
      break;
    }
  }

  delay(timeMillis);  // 1 sec of sneller!
  timeMgr.addSecond();

  // checken of uur veranderd is dus seconden = 0 en minuten = 0
  if (*minPtr == 0 && *secPtr == 0 && effectsEnabled) {
    hrEffect = true;
  }
  // geen uurverandering? checken op minuutverandering, sec = 0
  else if (*secPtr == 0 && effectsEnabled) {
    minEffect = true;
  }
  if (hrEffect) HourEffect();
  else if (minEffect) MinuteEffect();
  UpdateLEDs();
}

void interruptPush1() {
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // push 1 code here
    btnPushed = true;
    btnNumber = 1;
    lastDebounceTime = millis();
  }
}

void interruptPush2() {
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // push 2 code here
    btnPushed = true;
    btnNumber = 2;
    lastDebounceTime = millis();
  }
}

void interruptPush3() {
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // push 3 code hier
    btnPushed = true;
    btnNumber = 3;
    lastDebounceTime = millis();
  }
}

void interruptPush4() {
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // push 4 code hier
    btnPushed = true;
    btnNumber = 4;
    lastDebounceTime = millis();
  }
}