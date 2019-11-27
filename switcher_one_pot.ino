#include <TM1637Display.h>
#include <RotaryEncoder.h> // Separate encoder library for now due to better performance than using "Encoder.h"
// Encoder must be included before Control Surface
#include <Encoder.h>
#include <Control_Surface.h> // Include the library

int modWheelMsg[] = {
  26,
  24
}; // Initial CC numbers TODO: move this to EEPROM.read/write

// States for mode type
enum State { MODE_LOOP, MODE_SCAN, MODE_CONFIG };
int current_state = MODE_LOOP;

int rotarySteps = 1;

bool showLastPot = false; // Switch variables to trigger single display instruction
bool showLastCC  = false;

int whichPot = 1; // Selected potentiometer

// Diagnostic display message for loop mode
const uint8_t SEG_DONE[] = {
	SEG_A | SEG_D | SEG_E | SEG_F,          // C
	SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,  // H
	SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,  // U
	SEG_B | SEG_C | SEG_D                   // J
};
// Create a display instance
TM1637Display display(14, 15);

// Create two MIDI interfaces
USBMIDI_Interface usbmidi;
HardwareSerialMIDI_Interface serialmidi = {Serial1, MIDI_BAUD};

// And group them together
MultiMIDI_Interface<2> midi = {{&usbmidi, &serialmidi}};

// Array of banks allows dynamic indexing
Bank<120> banks[] = {
  {1},
  {1}
};

// Two encoder objects on the same pins for storing recet position
RotaryEncoder encoderSelectMode(2, 3);  // Encoder for potentiometer selection
RotaryEncoder encoderConfigMode(2, 3);  // Encoder for CC Message selection

Button enc_button = {4};

// Array of potentiometers
Bankable::CCPotentiometer potentiometers[] = {
  {{banks[0], BankType::CHANGE_ADDRESS}, A0, {0x00, CHANNEL_1}}, // TODO: fix the PCB design only pins A1 and A9 functional on the pcb, some headers wrongly connected to PWM pins instead of Analog
  {{banks[1], BankType::CHANGE_ADDRESS}, A9, {0x00, CHANNEL_1}}, // investigate the board with preferably another Arduino
};

void setup() {
  // Display setup
  display.setBrightness(0x0b);

  // Encoder setup
  encoderSelectMode.setPosition(whichPot / rotarySteps );
  encoderConfigMode.setPosition(modWheelMsg[0] / rotarySteps );

  // Initial MIDI potentiometer setting
  banks[0].select(modWheelMsg[0]);
  banks[1].select(modWheelMsg[1]);

  //button setup
  enc_button.begin();
}
// Function to read encoder in a range and display the current value
int encoderConfig(RotaryEncoder* encoder, TM1637Display* display, int rotaryMin, int rotaryMax, int startPos, int rotarySteps=1) {

  encoder->tick();
  int newPos = encoder->getPosition() * rotarySteps;

  if ( newPos < rotaryMin ) {                         // Limit encoder return values
    newPos = rotaryMin;
    encoder->setPosition(rotaryMin / rotarySteps );
  } else if ( newPos > rotaryMax ) {
    newPos = rotaryMax;
    encoder->setPosition(rotaryMax / rotarySteps );
  }
  
  if ( startPos != newPos ) {
    display->showNumberDec(newPos, false, 4, 0);
    return newPos;
  } else if ( rotaryMin == rotaryMax ) {
    display->showNumberDec(newPos, false, 4, 0);
    return newPos;                                    // Special case for only one potentiometer
  } else {
    return startPos;
  }
}

void loop() {

  if ( enc_button.update() == Button::Falling ) {
    switch (current_state) {
      case MODE_LOOP:
        current_state = MODE_SCAN;
        break;
      case MODE_SCAN:
        current_state = MODE_CONFIG;
        break;
      case MODE_CONFIG:
        current_state = MODE_LOOP;
        break;
    }
  }
  
  switch (current_state) {
    case MODE_LOOP:

      display.setSegments(SEG_DONE);
      Control_Surface.loop();
      showLastPot = true;                             // Update the switch variable to display last selected potentiometer in MODE_SCAN

      break;    
    case MODE_SCAN:

      if ( showLastPot ) {                            // Display last edited potentiometer number ONCE
        display.showNumberDec(whichPot, false, 4, 0); 
        showLastPot = false;                          // Update the switch variable to not display until next mode change
      }

      whichPot = encoderConfig(&encoderSelectMode, &display, 1, 2, whichPot);
      encoderConfigMode.setPosition( modWheelMsg[whichPot - 1] * rotarySteps ); // Set config encoder to selected potentiometer's recent value so it starts from there in MODE_CONFIG

      showLastCC = true;                              // Update the switch variable to display last selected CC message in MODE_CONFIG

      break;
    case MODE_CONFIG:

      if ( showLastCC == true ) {                     // Display current CC message on currently edited potentiometer ONCE
        display.showNumberDec(modWheelMsg[whichPot - 1], false, 4, 0);
        showLastCC = false;
      }

      modWheelMsg[whichPot - 1] = encoderConfig(&encoderConfigMode, &display, 0, 119, modWheelMsg[whichPot - 1]);

      banks[whichPot - 1].select(modWheelMsg[whichPot - 1]); // -1 to account for 0-based indexing
      
      break;    
  }
}
