# Medi-Keypad
My week 3 half life project - medi keypad
MIDI Keypad

A custom MIDI keypad PCB with 7 capacitive touch keys, 3 mechanical buttons, a rotary encoder, 2 slide potentiometers and 3 RGB LEDs, all controlled by an ESP32-S3.

Made for Half Life (Week 3), following the MIDI keypad guide and then changing it to make it my own.

PCB layout

What it has
7 touch keys: large copper pads on the PCB that work like piano keys. I designed the key footprint myself.
3 Kailh Choc buttons: for extra controls.
Rotary encoder (EC11): for volume or scrolling.
2 slide potentiometers (Bourns PTA1543): for smooth controls like pitch or volume.
3 SK6812 MINI-E LEDs: chained on one data pin.
Microcontroller: Waveshare ESP32-S3-Zero.
Board: 2 layers, about 99.8 x 90.15 mm.
Pin connections
Part	ESP32-S3-Zero pin
Touch key 1 to 7	GPIO 12, 10, 7, 11, 9, 8, 13
Buttons SW1, SW2, SW3	GPIO 4, 5, 6
Slider RV2, RV1	GPIO 1, 2
LED data (DIN)	GPIO 3
Encoder A, B	TX, RX

Check these against the schematic before building, in case I changed a pin later.

Files
Hardware/: KiCad project (schematic and PCB)
Gerbers/: files for the PCB manufacturer
Images/: screenshots of the design
bom.csv: bill of materials with prices
Bill of Materials

See bom.csv. Add the table here once the prices are final.

How I made it
Followed the guide and drew the schematic in KiCad. I changed the layout a few times so the PCB would be easier to wire.
Designed my own touch key footprint and tuned the dimensions.
Fixed the key connections in the schematic so every key reaches the ESP32.
Placed the parts and routed the board, using the Freerouting plugin for the rest.
Ran DRC and fixed the clearance and board edge errors.
Exported the production files with the JLCPCB Fabrication Toolkit and got a quote.
Status
 Schematic
 PCB placed and routed
 DRC fully clean
 Silkscreen text on the keys
 Case designed
 Firmware
 Parts ordered and board built
What I learned
How to make a custom footprint for capacitive touch keys, and why touch pads should stay away from other copper.
How nets work: a pad can only be wired to pads on the same net.
How to use an autorouter and read a DRC report.
Credits

Based on the MIDI Keypad guide: https://github.com/KaiPereira/MIDI-Keypad
