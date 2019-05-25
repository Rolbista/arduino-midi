#include <TM1637Display.h>
#include <RotaryEncoder.h>
#include <MIDI_Controller.h> // Include the library

//Set maximum and minimum values for encoder to match up with MIDI CC range
#define ROTARYSTEPS 1
#define ROTARYMIN 0
#define ROTARYMAX 127

//Set display values
//Clock pin
#define CLK 14
//Data pin
#define DIO 15
//Create object
TM1637Display display(CLK, DIO);

// Setup a RoraryEncoder for pins A2 and A3:
RotaryEncoder encoder(2, 3);

// Last known rotary position.
int lastPos = -1;
int modWheelMsg = 26; //initial CC number

// this constant won't change:
const int  buttonPin = 4;    // the pin that the pushbutton is attached to
const int ledPin = 17;       // the pin that the LED is attached to

// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 1;         // current state of the button
int lastButtonState = 1;     // previous state of the button
int ledState = LOW;             // ledState used to set the LED

unsigned long previousMillis = 0;        // will store last time LED was updated
long interval = 1000;           // interval at which to blink (milliseconds)

//initialize analog potentiometer with 0 to ignore offset
Analog potentiometer(A0, 0, 1);

void setup() {
  // initialize the button pin as a input:
  pinMode(buttonPin, INPUT_PULLUP);
  // initialize the LED as an output:
  //pinMode(ledPin, OUTPUT);
  // initialize serial communication:
  //Serial.begin(9600);
  HardwareSerialMIDI_Interface midi(Serial1, MIDI_BAUD);
  //display setup
  display.setBrightness(0x08);
  display.showNumberDec(modWheelMsg, false, 4, 0); //display initial encoder value 
  encoder.setPosition(modWheelMsg / ROTARYSTEPS); // start with the value of modWheelMsg.
  potentiometer.setAddressOffset(modWheelMsg);
}
int encoderConfig() {
    encoder.tick();
    // get the current physical position and calc the logical position
    int newPos = encoder.getPosition() * ROTARYSTEPS;

    if ( newPos < ROTARYMIN ) {
      encoder.setPosition(ROTARYMIN / ROTARYSTEPS);
      newPos = ROTARYMIN;

    } else if ( newPos > ROTARYMAX ) {
      encoder.setPosition(ROTARYMAX / ROTARYSTEPS);
      newPos = ROTARYMAX;
    } // if

    if ( lastPos != newPos ) {
      Serial.print(newPos);
      Serial.println();
      lastPos = newPos;
      modWheelMsg = newPos;
      display.showNumberDec(newPos, false, 4, 0);
      return newPos;
    }
}
void loop() {
  // read the pushbutton input pin:
  //unsigned long currentMillis = millis();
  buttonState = digitalRead(buttonPin);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == LOW) {
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounter++;
      Serial.println("on");
      Serial.print("number of button pushes: ");
      Serial.println(buttonPushCounter);
      Serial.print("buttonState: ");
      Serial.println(buttonState);
      if ( buttonPushCounter == 2 ) {
        buttonPushCounter = 0;
      }
    } else {
      // if the current state is LOW then the button went from on to off:
      Serial.println("off");
    }
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;


  // turns on the LED every four button pushes by checking the modulo of the
  // button push counter. the modulo function gives you the remainder of the
  // division of two numbers:
  if (buttonPushCounter == 1) {
    modWheelMsg = encoderConfig();
    //Analog potentiometer(A0, modWheelMsg, 1);
    potentiometer.setAddressOffset(modWheelMsg);
  } else if (buttonPushCounter == 0) {
    interval = 100;
    //if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    //previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    // if (ledState == LOW) {
    //   ledState = HIGH;
    // } else {
    //   ledState = LOW;
    // }

    // set the LED with the ledState of the variable:
    //digitalWrite(ledPin, ledState);
    MIDI_Controller.refresh();
    Serial.println("new CC message: \n");
    Serial.println(modWheelMsg);
    //}
  }
}
