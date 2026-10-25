<img width="1649" height="1031" alt="Screenshot 2026-10-25 142341" src="https://github.com/user-attachments/assets/041f89c2-daaf-46f7-aa68-14a6212a9b8e" />
<img width="657" height="681" alt="Screenshot 2026-10-24 231630" src="https://github.com/user-attachments/assets/e2dc5520-3045-46a4-9a75-434d2b694fa7" />
<img width="643" height="690" alt="Screenshot 2026-10-24 231647" src="https://github.com/user-attachments/assets/521ffcbc-d06b-4c73-a369-4c5f1ea3df67" />
# MIDI Keypad

A custom MIDI keypad PCB with 7 capacitive touch keys, 3 mechanical buttons, a rotary encoder, 2 slide potentiometers and 3 RGB LEDs, all controlled by an ESP32-S3.

Made for Half Life (Week 3), following the MIDI keypad guide and then changing it to make it my own.

![PCB layout](Images/PCB-front.png)

## What it has

- **7 touch keys:** large copper pads on the PCB that work like piano keys. I designed the key footprint myself.
- **3 Kailh Choc buttons:** for extra controls.
- **Rotary encoder (EC11):** for volume or scrolling.
- **2 slide potentiometers (Bourns PTA1543):** for smooth controls like pitch or volume.
- **3 SK6812 MINI-E LEDs:** chained on one data pin.
- **Microcontroller:** Waveshare ESP32-S3-Zero.
- **Board:** 2 layers, about 99.8 x 90.15 mm.

## Pin connections

| Part | ESP32-S3-Zero pin |
| --- | --- |
| Touch key 1 to 7 | GPIO 12, 10, 7, 11, 9, 8, 13 |
| Buttons SW1, SW2, SW3 | GPIO 4, 5, 6 |
| Slider RV2, RV1 | GPIO 1, 2 |
| LED data (DIN) | GPIO 3 |
| Encoder A, B | TX, RX |

Check these against the schematic before building, in case I changed a pin later.

## Files

- `Production/`: files for the PCB manufacturer
- `Images/`: screenshots of the design



## How I made it

1. Followed the guide and drew the schematic in KiCad. I changed the layout a few times so the PCB would be easier to wire.
2. Designed my own touch key footprint and tuned the dimensions.
3. Fixed the key connections in the schematic so every key reaches the ESP32.
4. Placed the parts and routed the board, using the Freerouting plugin for the rest.
5. Ran DRC and fixed the clearance and board edge errors.
6. Exported the production files with the JLCPCB Fabrication Toolkit and got a quote.

## Status

- [x] Schematic
- [x] PCB placed and routed
- [x] DRC fully clean
- [x] Case designed
- [x] Firmware
- [ ] Parts ordered and board built

## What I learned

- How to make a custom footprint for capacitive touch keys, and why touch pads should stay away from other copper.
- How nets work: a pad can only be wired to pads on the same net.
- How to use an autorouter and read a DRC report.

## Credits

Based on the MIDI Keypad guide: https://github.com/KaiPereira/MIDI-Keypad
