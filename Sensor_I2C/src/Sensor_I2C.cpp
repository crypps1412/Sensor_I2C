
#include "Sensor_I2C.h"

/********** I2C **********/

/*
 * Get all device addresses on I2C bus
 * Input:
 *  uint8_t array: to save address
 *  uint8_t: number of the first devices you want to get address from (default: 128)
 *  bool: check if you just want to get the number of devices (default: 0)
 * Output: uint8_t number of devices whose addresses was read
 */
uint8_t I2C::scanAllDevices(uint8_t addr[], uint8_t maxDevices, bool justNumber) {
  uint8_t i = 0;
  for (uint8_t a = 0; a < 128 && i < maxDevices; a++) {
    Wire.beginTransmission(a);
    if (!Wire.endTransmission())
      if (justNumber) i++;
      else addr[i++] = a;
    delayMicroseconds(1000);
  }
  return i;
}

/*
 * Get the particular device address following accending order
 * Input: uint8_t the device's ordinal (default: 1)
 * Output: uint8_t the address of the device
 * Failed output: 255
 */
uint8_t I2C::scanDeviceByOrdinal(uint8_t ordinal) {
  uint8_t addr[ordinal];
  if (scanAllDevices(addr, ordinal) == ordinal && ordinal != 0) return addr[ordinal - 1];
  return 255;
}

/*
 * Get the number of devices on I2C bus
 * Input: none
 * Output: uint8_t number of devices
 * Failed output: 255
 */
uint8_t I2C::numberOfDevices() {
  uint8_t* addr;
  return scanAllDevices(addr, 128, true);
}

/*
 * Check if the address provided to code is correct
 * Input: uint8_t & the address providing
 * Output: uint8_t:
 * 	 0 if connection is good
 *   1 if full buffer
 *   2 if address sent, NACK received
 *   3 if data sent, NACK received
 *   4 for other error
 */
uint8_t I2C::checkDeviceConnection(const uint8_t &addr) {
  if (addr > 127) return 0;
  Wire.beginTransmission(addr);
  return Wire.endTransmission();
}

/*
 * Get rid of all extra data on I2C bus
 * Input: none
 * Output: none
 */
void I2C::flush() {
  while (Wire.available()) Wire.read();
}


/********** SENSOR **********/

/*
 * Check device address and connection, if provided address fails to response, automatically
 * find address following ordinal
 * Input:
 *  uint8_t: the provided address (default: 255)
 *  uint8_t: ordinal (default: 1)
 * Output: bool have to success to continue the program (always 1)
 */
bool SENSOR::sensorBegin(uint8_t addr, uint8_t ordinal) {
  if (addr > 127) {
    addr = I2C::scanDeviceByOrdinal(ordinal);
    if (addr == 255) {
      Serial.println(F("Can't find any device! Auto-loop!!!"));
      delay(999);
      return sensorBegin();
    }
  } else if (I2C::checkDeviceConnection(addr)) {
      Serial.println(F("Can't find device with provided address! Auto-scanning!!!"));
      return sensorBegin();
  }
  this->_id = readByte(_idAddr);
  this->_addr = addr;
  Serial.print(F("Detected "));
  this->showInfo();
  return 1;
}

/*
 * Print sensor info (name, address, id) to serial
 * Input: none
 * Output: none
 */
void SENSOR::showInfo() {
  Serial.println(this->_name);
  Serial.print(F("Device Address: 0b"));
  Serial.print(this->_addr, BIN);
  Serial.print(F(" 0x"));
  Serial.println(this->_addr, HEX);
  Serial.print(F("Device Id: 0b"));
  Serial.print(this->_id, BIN);
  Serial.print(F(" 0x"));
  Serial.println(this->_id, HEX);
}

/*
 * Get sensor name
 * Input: none
 * Output: char* name pointer
 */
char* SENSOR::getName() {
  return _name;
}

/*
 * Get sensor id
 * Input: none
 * Output: uint8_t id number
 */
uint8_t SENSOR::getId() {
  return _id;
}

/*
 * Read multiple bytes from I2C sensor
 * Input:
 *  uint8_t[]: saving array, with the first element containing register address
 *  uint8_t &: number of bytes to collect
 *  uint8_t: result when failing to read (default: 0)
 * Output: bool true if reading successfully
 */
bool SENSOR::readBytes(uint8_t bytes[], const uint8_t &numberOfBytes, uint8_t failAlt) {
  Wire.beginTransmission(_addr);
  Wire.write(bytes[0]);
  if (!Wire.endTransmission()) {
    Wire.requestFrom(_addr, numberOfBytes);
    for (uint8_t i = 0; i < numberOfBytes; i++)
      if (Wire.available() != 0) bytes[i] = Wire.read();
      else bytes[i] = failAlt;
    I2C::flush;
    return true;
  }
  return false;
}

/*
 * Read a byte from I2C sensor
 * Input:
 *  uint8_t &: reading register address
 *  uint8_t: result when failing to read (default: 0)
 * Output: uint8_t data read from register
 */
uint8_t SENSOR::readByte(const uint8_t &registerAddr, uint8_t failAlt) {
  uint8_t data[] = {registerAddr};
  this->readBytes(data, 1, failAlt);
  return *data;
}

/*
 * Read 2 bytes from I2C sensorl
 * Input:
 *  uint8_t &: reading registers address
 *  uint16_t: result when failing to read (default: 0)
 * Output: uint16_t data read from 2 continual registers
 */
uint16_t SENSOR::readWord(const uint8_t &registerAddr, uint16_t failAlt) {
  uint8_t data[2] = {registerAddr, 0};
  if (!this->readBytes(data, 2)) return failAlt;
  return data[0] << 8 | data[1];
}

/*
 * Write a byte to a register on I2C sensor
 * Input:
 *  uint8_t &: writing registers address
 *  uint8_t &: control word
 * Output: bool true if writing successfully
 */
bool SENSOR::writeByte(const uint8_t &registerAddr, const uint8_t &cwd) {
  Wire.beginTransmission(_addr);
  Wire.write(registerAddr);
  Wire.write(cwd);
  return !Wire.endTransmission();
}
