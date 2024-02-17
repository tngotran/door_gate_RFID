/*
 * I2C EEPROM Read
 *
 * Reads the value of each byte of the I2C EEPROM and prints it
 * to the computer.
 * This example code is in the public domain.
 */

#include <AT24Cxx.h>
#define i2c_address 0x50

// start reading from the first byte (address 0) of the EEPROM
int address = 0;
byte value;

// Initilaize using AT24CXX(i2c_address, size of eeprom in KB).  
AT24Cxx eep(i2c_address, 8);

void setup() {
  // initialize serial and wait for port to open:
  
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
}

void loop() {
  // read a byte from the current address of the EEPROM
  value = eep.read(address);

  Serial.print(address);
  Serial.print(":\t");
  Serial.print(value, HEX);
  Serial.println();

  address = address + 1;
  // Serial.println(eep.length());
  if (address == eep.length()) {
    address = 0;
  }

  /***
    As the EEPROM sizes are powers of two, wrapping (preventing overflow) of an
    EEPROM address is also doable by a bitwise and of the length - 1.
    ++address &= EEPROM.length() - 1;
  ***/

  delay(500);
  if (address > 10){
    value = eep.read(8000);

  Serial.print(8000);
  Serial.print(":\t");
  Serial.print(value, HEX);
  Serial.println();


    exit(0);
  }
}
