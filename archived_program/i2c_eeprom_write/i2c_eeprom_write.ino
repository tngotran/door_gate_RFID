/*
 * EEPROM Write
 *
 * Stores values read from analog input 0 into the EEPROM.
 * These values will stay in the EEPROM when the board is
 * turned off and may be retrieved later by another sketch.
 */

#include <AT24Cxx.h>
#define i2c_address 0x50

/** the current address in the EEPROM (i.e. which byte we're going to write to next) **/
int address = 0;
AT24Cxx eep(i2c_address, 8);

void setup() {
	
	Serial.begin(9600);	// initialize serial and wait for port to open:
	while (!Serial) {
		; // wait for serial port to connect. Needed for native USB port only
	}

}

void loop() {
	/***
	Need to divide by 4 because analog inputs range from
	0 to 1023 and each byte of the EEPROM can only hold a
	value from 0 to 255.
	***/

	// int val = analogRead(0)/ / 4;
	// Serial.println(val);
  byte var = 255;
  byte var1 = 171;


  byte master_read1 = 67; //43
  byte master_read2 = 233; //E9
  byte master_read3 = 147; //93
  byte master_read4 = 174; //ae

  

  // Serial.println(var, HEX);
  // Serial.println(var1, HEX);


	/***
	Write the value to the appropriate byte of the EEPROM.
	these values will remain there when the board is
	turned off.
	***/

	eep.write(0, master_read1);
	address = address + 1;
  eep.write(1, master_read2);
  eep.write(2, master_read3);
  eep.write(3, master_read4);


  // Serial.println(eep.length());

	/***
	Advance to the next address, when at the end restart at the beginning.

	***/
	// if (address == eep.length())
	// {
	// 	address = 0;
	// }

	/***
	As the EEPROM sizes are powers of two, wrapping (preventing overflow) of an
	EEPROM address is also doable by a bitwise and of the length - 1.

	++address &= eep.length() - 1;
	***/


  exit(0);
}
