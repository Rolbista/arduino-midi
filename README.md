# MidiBoi™

MidiBoi™ is a controller designed for use with Korg® Monologue™. It is literally just two reprogrammable mod wheels.  
Because Monologue™ only has one pitch-stick, dynamic modulation is limited when playing by hand since it's  impossible  
to do, for example, pitch bend and add vibrato at the same time conveniently. LFO int knob is on the other side of  
the synthesizer. Why not use a standard MIDI keyboard with normal wheels then? Well, while standard Pitch Bend MIDI  
message is accepted and interpreted by the synth, Mod Wheel message is not. LFO intensity is assigned to MIDI CC 26  
so it won't react unless you reprogram the wheel to send it. I could not find a controller that would let me do this,  
space was also a consideration - since the Monologue™ already has a keyboard and doesn't exist in a module form apart  
from some risky looking mods, why take up space with another if two wheels are all that is necessary to add  
expressiveness to the interface? Potentiometers with programmable CC messages are useful in cases where hardware is  
fixed and cannot easily accept a different value like you would on a DAW by just using MIDI Learn. While it was built  
with a specific model in mind, this can be used with other synthesizers as well if you need additional mod wheel or  
two or want a comfortable wheel to steer a parameter not present on the panel.

## Setup  

The default selected board is Arduino Pro Micro or clone because of its small size and native USB capability which  
allows you to use it with your DAW without any extra fuss. The library dependencies are in the sketch.  
Open the `./switcher_one_pot.ino` sketch, update the pins as appropriate and upload it.

## Usage

MidiBoi™ is powered by USB, just plug one of those old phone chargers that you have lying around that will surely  
come in handy one day - now is their time to shine! Use a standard DIN MIDI cable to connect MidiBoi™ output to  
synth MIDI input. Default CC are 26 (LFO intensity) for wheel 1 and 24 (LFO rate) for wheel 2. When the controller  
powers up it works in default, loop mode, sending default messages. To reprogram wheel 1 push the encoder button once,  
select the desired wheel number (1), press the button again. Now select new CC message (0-119) keeping in mind your  
synths MIDI implementation. Press the button again to save the change. Now wheel one send a new CC message and you  
instrument should react accordingly.  
  
To save potentiometer settings to persistent memory double press the encoder while in `PLAY`/`LOOP` mode.  
A `SAVE` message should display for 1,5 seconds and return to normal.  

## Features

* Segment display showing current wheel/potentiometer/MIDI CC selected
* Encoder for value input
* Support for n potentiometers with reprogrammable CCs limited only by number of ADC pins on selected Arduino board
* Support for storing and saving potentiometer configs in EEPROM  
* USB and DIN5 MIDI output on channel 1
* Extensibility: with little modification of the hardware any MIDI parameter can be controlled with any type of analog device - add a jack for an expression pedal if you like

## Roadmap  

* Software MIDI passthrough so the only DIN jack on target synth is not taken up
* Possibly OLED screen for clearer information display
* Channel reprogrammability
* Higher bit resolution (10 bit, allowed by Control-Surface library)
* Example PCB design (Kicad single-sided board)

## Additional info

Libraries used:  
https://github.com/tttapa/Control-Surface  
https://github.com/mathertel/RotaryEncoder  
https://github.com/avishorp/TM1637  
https://github.com/mathertel/OneButton  

Monologue MIDI Implementation Chart:  
https://www.korg.com/us/support/download/manual/0/733/3519/  
https://bit.ly/2ZH0zTO (page 58)
