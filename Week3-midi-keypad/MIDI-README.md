# MIDI Keypad

A custom MIDI keypad with 7 capacitive touch keys, 3 buttons, a rotary encoder, 2 sliders and 3 RGB LEDs, all run by an ESP32-S3. It plugs into a computer over USB and shows up as a MIDI device.

Made for Half Life (Week 3), following the MIDI keypad guide and then changing it to make it my own.

![PCB layout](Images/PCB-front.png)
![Enclosure render](Images/Enclosure-render.png)

## What it has

- **7 touch keys:** large bare copper pads that work like piano keys. I designed the key footprint myself.
- **3 Kailh Choc buttons:** octave down, octave up and sustain.
- **Rotary encoder (EC11):** volume.
- **2 slide potentiometers (Bourns PTA1543):** filter cutoff and mod wheel.
- **3 SK6812 MINI-E LEDs:** show the octave, the key being touched and sustain.
- **Microcontroller:** Waveshare ESP32-S3-Zero.
- **Board:** 2 layers, about 99.8 x 90.15 mm.
- **Case:** 3D printed enclosure designed in Fusion 360.

## How it makes sound

The keypad doesn't make sound itself. It sends MIDI messages over USB, like "note C pressed", and a computer or synth app plays the sound. To try it, open a free online synth such as midi.city in Chrome, or a program like Surge XT or LMMS, and pick the keypad as the MIDI input.

## Controls

| Control | What it does |
| --- | --- |
| Touch keys 1 to 7 | Play C, D, E, F, G, A, B |
| SW1 / SW2 | Octave down / octave up |
| SW3 | Sustain pedal while held |
| Rotary encoder | Volume (CC7) |
| Slider RV1 | Filter cutoff (CC74) |
| Slider RV2 | Mod wheel (CC1) |

## Pin connections

| Part | ESP32-S3-Zero pin |
| --- | --- |
| Touch key 1 to 7 | GPIO 12, 10, 7, 11, 9, 8, 13 |
| Buttons SW1, SW2, SW3 | GPIO 4, 5, 6 |
| Slider RV2, RV1 | GPIO 1, 2 |
| LED data (DIN) | GPIO 3 |
| Encoder A, B | TX, RX |

## Files

- `Hardware/`: KiCad project (schematic and PCB)
- `Gerbers/`: files for the PCB manufacturer
- `Firmware/MidiKeypad/`: Arduino sketch
- `Case/`: Fusion 360 file and 3MF for printing
- `Images/`: screenshots and renders
- `bom.csv`: bill of materials with prices

## Build it

**Firmware:** in the Arduino IDE, install the ESP32 board package (3.0 or newer) and the Adafruit NeoPixel library. Choose **ESP32S3 Dev Module**, set **USB Mode** to **USB-OTG (TinyUSB)** and **USB CDC On Boot** to **Enabled**, then open `MidiKeypad.ino` and upload. If the port doesn't show up, hold BOOT while plugging in.

**Touch tuning:** if the keys trigger by themselves or don't react, change `TOUCH_PERCENT` in the code. Set `DEBUG` to `true` to see the values in the Serial Monitor.

## Bill of Materials

See [`bom.csv`](bom.csv). Add the table here once the prices are final.

## How I made it

1. Followed the guide and drew the schematic in KiCad. I changed the layout a few times so the PCB would be easier to wire.
2. Designed my own touch key footprint and tuned the dimensions.
3. Fixed the key connections in the schematic so every key reaches the ESP32.
4. Placed and routed the board, using the Freerouting plugin for the rest.
5. Ran DRC and fixed errors, then exported the production files with the JLCPCB Fabrication Toolkit and got a quote.
6. Imported the board into Fusion 360 with STEP models of all the parts and designed the enclosure, with cutouts for the buttons, encoder knob and sliders, and an open area so the touch keys can be touched directly.
7. Wrote the firmware and exported the case as a 3MF file for printing.

## Status

- [x] Schematic
- [x] PCB placed and routed
- [ ] DRC fully clean
- [ ] Silkscreen text on the keys
- [x] Case designed in Fusion 360
- [x] Firmware written
- [ ] Firmware tested on real hardware
- [ ] Case printed
- [ ] Parts ordered and board built

## What I learned

- How to make a custom footprint for capacitive touch keys, and why touch pads should stay away from other copper.
- How nets work: a pad can only be wired to pads on the same net.
- How to use an autorouter and read a DRC report.
- How to design an enclosure around a real PCB using imported STEP models.

## Credits

Based on the MIDI Keypad guide: https://github.com/KaiPereira/MIDI-Keypad

The firmware was written with help from Claude (Anthropic).
