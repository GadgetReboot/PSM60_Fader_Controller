/* I2C Fader Pot Control Demo
    Uses Arduino Uno and sends I2C commands to an ATTiny804
    attached to a motorized Fader pot

    Four fader pots are given numbers from 0 to 100, triggered by a button input,
    representing the percentage of travel from bottom to top (motor side to opposite side)
    that the fader shall move to.  The pots will be given an arbitrary position to move to
    based on which button is pressed to demonstrate control.

    Gadget Reboot
*/

#include <Wire.h>

#define potAddr1 0x54
#define potAddr2 0x55
#define potAddr3 0x56
#define potAddr4 0x57


void setup() {
  Wire.begin();        // init I2C

  // button inputs
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
}

void loop() {

  if (!digitalRead(2)) {
    sendDataWire(100, potAddr1);   // send position (0-100) to I2C Addr
    sendDataWire(100, potAddr2);   // send position (0-100) to I2C Addr
    sendDataWire(100, potAddr3);   // send position (0-100) to I2C Addr
    sendDataWire(100, potAddr4);   // send position (0-100) to I2C Addr
    delay(100);
  }

  if (!digitalRead(3)) {
    sendDataWire(50, potAddr1);   // send position (0-100) to I2C Addr
    sendDataWire(50, potAddr2);   // send position (0-100) to I2C Addr
    sendDataWire(50, potAddr3);   // send position (0-100) to I2C Addr
    sendDataWire(50, potAddr4);   // send position (0-100) to I2C Addr
    delay(100);
  }

  if (!digitalRead(4)) {
    sendDataWire(0, potAddr1);    // send position (0-100) to I2C Addr
    sendDataWire(25, potAddr2);   // send position (0-100) to I2C Addr
    sendDataWire(50, potAddr3);   // send position (0-100) to I2C Addr
    sendDataWire(75, potAddr4);  // send position (0-100) to I2C Addr
    delay(100);
  }

  if (!digitalRead(5)) {
    sendDataWire(60, potAddr1);   // send position (0-100) to I2C Addr
    sendDataWire(40, potAddr2);   // send position (0-100) to I2C Addr
    sendDataWire(80, potAddr3);   // send position (0-100) to I2C Addr
    sendDataWire(50, potAddr4);  // send position (0-100) to I2C Addr
    delay(100);
  }

}

void sendDataWire(byte dataByte, byte addr) {
  Wire.beginTransmission(addr);     // prepare transmission to i2c slave address
  Wire.write(dataByte);             // send the percentage data
  Wire.endTransmission();           // finish transmission
}
