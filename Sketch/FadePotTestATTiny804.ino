// Fader Pot Test
// Using 10K motorized slide pot:  Bourns PSM60-081A-103B2
//
// Target Hardware:  ATTiny804 controlling L293DD motor driver
//
// Tested with Arduino IDE 1.8.13
// Gadget Reboot
// https://www.youtube.com/gadgetreboot

#include <Wire.h>
#define i2cAddr 0x54
volatile byte buf = 0;          // I2C received data buffer
volatile bool newData = false;  // flag to detect new data received

const int pwmPin      = 0;     // motor enable
const int motA        = 2;     // Motor pins   If A = +V and B= Gnd, fader moves toward the motor side (min position)
const int motB        = 1;     //   note: I had to reverse the software A/B pins from the schematic because motor seems to
                               //         run in reverse from what I interpret in the motor data sheet
const int servoPotPin = 3;     // servo track pot for position indication
const int pwmSpeed    = 255;   // pwm motor speed 0-255
const int adcMaxVal   = 1023;  // pot max analog reading value on ATTiny ADC


void setup() {

  Serial.begin(9600);

  Wire.begin(i2cAddr);             // join i2c bus
  Wire.onReceive(receiveData);     // function to execute when data received

  digitalWrite(pwmPin, 0);  // disable motor
  digitalWrite(motA, 0);    // disable motor
  digitalWrite(motB, 0);    // disable motor

  pinMode(pwmPin, OUTPUT);
  pinMode(motA, OUTPUT);
  pinMode(motB, OUTPUT);
  pinMode(servoPotPin, INPUT);

}

void loop() {

  int sensorValue = analogRead(servoPotPin);

  // move fader if I2C command has been received
  // calculate the adc value that represents the requested percentage of travel location
  // eg 50% = half of adcMaxVal
  if (newData) {
    int target = ((buf / 100.0) * adcMaxVal); 
    moveFader(pwmSpeed, target);
  }
  delay(100);

}

// decide if needing to go fwd or rev or nothing
// approach that direction until position matches target or has gone past, then stop
void moveFader (int motSpeed, int targetPos) {

  newData = false;  // clear new received data flag

  // as the fader moves toward the top/max position (away from motor)
  // the servo pot wiper moves more toward VCC = adcMaxVal
  // so we can determine what position the fader is at by reading the servo track pot

  int currentPos = (analogRead(servoPotPin));  // current position of fader

  if (currentPos != targetPos) {   // if motor needs to move
    if (currentPos > targetPos) {  // if need to move motor toward min position
      digitalWrite(motA, 1);       // set motor to run toward min position (motor side of pot)
      digitalWrite(motB, 0);
    }
    else  if (currentPos < targetPos) {  // if need to move motor toward max position
      digitalWrite(motA, 0);             // set motor to run toward max position
      digitalWrite(motB, 1);
    }

    analogWrite(pwmPin, motSpeed);  // start motor

    while (abs(targetPos - (analogRead(servoPotPin))) > 50 ) {  // loop until fader moves close to target value
      delay(1);
    }
  }
  digitalWrite(pwmPin, 0);  // disable motor
  digitalWrite(motA, 0);
  digitalWrite(motB, 0);
}

// function that executes whenever data is received from I2C master
void receiveData(int16_t numBytes) {             // the Wire library tells us how many bytes
  // for (uint8_t i = 0; i < numBytes; i++) {    // were received so we can for loop for that
  buf = Wire.read();                             // amount and read the received data
  buf = constrain(buf, 0, 100); //constrain number from 0 to 100 to use in a percentage calculation later
  newData = true;                                // set flag for main loop
  // }
}
