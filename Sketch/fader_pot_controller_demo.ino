/* I2C Fader Pot Control Demo
    Uses Arduino Uno and sends I2C commands to an ATTiny804
    attached to a motorized Fader pot

    Data sent is a number from 0 to 100, input from the user via serial monitor,
    and represents the percentage of travel from bottom to top (motor side to opposite side)
    that the fader shall move to.

    Gadget Reboot
*/

#include <Wire.h>

void setup() {
  Wire.begin();        // init I2C
  Serial.begin(9600);
}

void loop() {

  Serial.println("Enter position (0-100) to move Fader as a percentage travel from bottom to top:  ");

  String inString = "";
  while (Serial.available() == 0) {} // wait for user input

  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      // convert the incoming byte to a char and add it to the string:
      inString += (char)inChar;
    }
    delay(10);
  }

  int potPosition = inString.toInt();

  Serial.print("Setting fader to ");
  byte c = byte(potPosition);
  Serial.print(c);
  Serial.println("%");
  sendDataWire(c);
  delay(100);
}

void sendDataWire(byte dataChar) {
  Wire.beginTransmission(0x54);     // prepare transmission to slave with address 0x54
  Wire.write(dataChar);             // send the percentage data
  Wire.endTransmission();           // finish transmission
}
