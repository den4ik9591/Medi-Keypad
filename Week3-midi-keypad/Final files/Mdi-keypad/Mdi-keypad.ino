// MIDI Keypad - Waveshare ESP32-S3-Zero
// 7 touch keys (piano notes), 3 buttons, rotary encoder, 2 sliders, 3 LEDs.
// Shows up on the computer as a USB MIDI device.
//
// Arduino IDE settings:
//   Board:            ESP32S3 Dev Module   (needs ESP32 board package 3.0 or newer)
//   USB Mode:         USB-OTG (TinyUSB)
//   USB CDC On Boot:  Enabled   (lets Serial debug work next to MIDI)
// Library needed: Adafruit NeoPixel

#include "USB.h"
#include "USBMIDI.h"
#include <Adafruit_NeoPixel.h>

USBMIDI MIDI;

// ---------- Pins (from my schematic - check against yours) ----------
const uint8_t TOUCH_PINS[7] = {12, 10, 7, 11, 9, 8, 13};  // keys 1 to 7
const uint8_t BTN_PINS[3]   = {4, 5, 6};                  // SW1, SW2, SW3
const uint8_t SLIDER1_PIN   = 2;                          // RV1
const uint8_t SLIDER2_PIN   = 1;                          // RV2
const uint8_t LED_PIN       = 3;                          // SK6812 data
const uint8_t ENC_A         = 43;                         // TX pad
const uint8_t ENC_B         = 44;                         // RX pad

// ---------- Settings ----------
const uint8_t BASE_NOTES[7] = {60, 62, 64, 65, 67, 69, 71};  // C D E F G A B
const float TOUCH_PERCENT = 5.0;   // how big a change counts as a touch. Raise if keys trigger by themselves, lower if they don't react.
const bool DEBUG = false;          // true = print touch values to Serial Monitor so you can tune TOUCH_PERCENT

Adafruit_NeoPixel strip(3, LED_PIN, NEO_GRB + NEO_KHZ800);

float touchBase[7];
bool pressed[7];
uint8_t sounding[7];     // note currently playing for each key (0 = none)
int octaveShift = 0;     // -2 to +2
bool sustainOn = false;
int volume = 100;
int lastSlider1 = -1, lastSlider2 = -1;
bool lastBtn[3] = {true, true, true};
unsigned long lastBtnTime[3] = {0, 0, 0};
uint8_t encLast = 0;
int8_t encAccum = 0;

void releaseAllNotes() {
  for (int i = 0; i < 7; i++) {
    if (sounding[i]) {
      MIDI.noteOff(sounding[i], 0, 1);
      sounding[i] = 0;
    }
  }
}

void calibrateTouch() {
  // Don't touch the keys while this runs (about 1 second)
  for (int i = 0; i < 7; i++) {
    double sum = 0;
    for (int n = 0; n < 32; n++) {
      sum += touchRead(TOUCH_PINS[i]);
      delay(5);
    }
    touchBase[i] = sum / 32.0;
    pressed[i] = false;
    sounding[i] = 0;
  }
}

void updateTouch() {
  for (int i = 0; i < 7; i++) {
    uint32_t v = touchRead(TOUCH_PINS[i]);
    float diff = fabs((float)v - touchBase[i]);
    float limit = touchBase[i] * TOUCH_PERCENT / 100.0;

    if (DEBUG && i == 0 && millis() % 500 < 5) {
      Serial.printf("key1 value=%u base=%.0f limit=%.0f\n", v, touchBase[i], limit);
    }

    if (!pressed[i]) {
      if (diff > limit) {
        pressed[i] = true;
        uint8_t note = BASE_NOTES[i] + 12 * octaveShift;
        sounding[i] = note;
        MIDI.noteOn(note, 100, 1);
      } else {
        // slowly follow drift (temperature, humidity) while not touched
        touchBase[i] = touchBase[i] * 0.995 + v * 0.005;
      }
    } else {
      if (diff < limit * 0.6) {
        pressed[i] = false;
        if (sounding[i]) MIDI.noteOff(sounding[i], 0, 1);
        sounding[i] = 0;
      }
    }
  }
}

void changeOctave(int delta) {
  releaseAllNotes();
  octaveShift = constrain(octaveShift + delta, -2, 2);
}

void updateButtons() {
  for (int i = 0; i < 3; i++) {
    bool state = digitalRead(BTN_PINS[i]);  // LOW = pressed
    if (state != lastBtn[i] && millis() - lastBtnTime[i] > 30) {
      lastBtnTime[i] = millis();
      lastBtn[i] = state;
      bool down = (state == LOW);
      if (i == 0 && down) changeOctave(-1);          // SW1: octave down
      if (i == 1 && down) changeOctave(+1);          // SW2: octave up
      if (i == 2) {                                  // SW3: sustain while held
        sustainOn = down;
        MIDI.controlChange(64, down ? 127 : 0, 1);
      }
    }
  }
}

void updateEncoder() {
  static const int8_t table[16] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};
  uint8_t s = (digitalRead(ENC_A) << 1) | digitalRead(ENC_B);
  if (s != (encLast & 3)) {
    encAccum += table[((encLast & 3) << 2) | s];
    encLast = s;
    if (encAccum >= 4 || encAccum <= -4) {
      volume = constrain(volume + (encAccum > 0 ? 3 : -3), 0, 127);
      encAccum = 0;
      MIDI.controlChange(7, volume, 1);   // CC7 = volume
    }
  }
}

int readSlider(uint8_t pin) {
  long sum = 0;
  for (int i = 0; i < 8; i++) sum += analogRead(pin);
  return (sum / 8) * 127 / 4095;
}

void updateSliders() {
  int s1 = readSlider(SLIDER1_PIN);
  int s2 = readSlider(SLIDER2_PIN);
  if (abs(s1 - lastSlider1) >= 1) {
    lastSlider1 = s1;
    MIDI.controlChange(74, s1, 1);   // CC74 = filter cutoff
  }
  if (abs(s2 - lastSlider2) >= 1) {
    lastSlider2 = s2;
    MIDI.controlChange(1, s2, 1);    // CC1 = mod wheel
  }
  // If a slider works backwards, swap its ends or use 127 - value here.
}

void updateLeds() {
  // LED 1: octave colour
  static const uint32_t octColors[5] = {
    0x0000FF, 0x00FFFF, 0x00FF00, 0xFFFF00, 0xFF0000
  };
  strip.setPixelColor(0, octColors[octaveShift + 2]);

  // LED 2: colour of the last key held (off when no key is touched)
  int held = -1;
  for (int i = 0; i < 7; i++) if (pressed[i]) held = i;
  if (held >= 0) strip.setPixelColor(1, strip.gamma32(strip.ColorHSV(held * 9000, 255, 255)));
  else strip.setPixelColor(1, 0);

  // LED 3: sustain
  strip.setPixelColor(2, sustainOn ? 0xFFFFFF : 0);
  strip.show();
}

void setup() {
  if (DEBUG) Serial.begin(115200);

  for (int i = 0; i < 3; i++) pinMode(BTN_PINS[i], INPUT_PULLUP);
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  encLast = (digitalRead(ENC_A) << 1) | digitalRead(ENC_B);
  analogReadResolution(12);

  strip.begin();
  strip.setBrightness(40);
  strip.show();

  MIDI.begin();
  USB.begin();
  delay(1000);

  calibrateTouch();
  MIDI.controlChange(7, volume, 1);
}

void loop() {
  updateTouch();
  updateButtons();
  updateEncoder();
  updateSliders();
  updateLeds();
}
