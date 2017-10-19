/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

 This example code is in the public domain.
 */
int sensorPin       = A0;    // select the input pin for the potentiometer
int ledPin          = 13;      // select the pin for the LED
int sensorValue     = 0;  // variable to store the value coming from the sensor
int oldSensorValue  = 0;
int outputValue     = 0;        // value output to the PWM (analog out)
int MSB             = 0;
int LSB             = 0;
int delayTime       = 10;              // time between reads 
int baudRate        = 9600;

void setup(){
    // initialize serial communication at 9600 bits per second:
    Serial.begin(baudRate);
}
void loop(){
    int sensorValue = analogRead(sensorPin);
    if(oldSensorValue != sensorValue){
        Serial.println(sensorValue);
        int oldSensorValue = sensorValue;
    }
    delay(delayTime);        // delay in between reads for stability
}