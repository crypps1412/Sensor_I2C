
#include "Sensor_I2C.h"

/********** I2C **********/

/*
 * Get all device address on I2C bus
 * Input:
 *  uint8_t array: to save address
 *  uint8_t &: number of the first devices you want to get address from (default: 128)
 *  bool: check if you just want to get the number of devices (default: 0)
 * Output: uint8_t: number of devices whose address was read
 */
uint8_t I2C::scanAllDevices(uint8_t addr[], uint8_t maxNumberOfDevices, bool justNumber) {
  uint8_t i = 0;
  for (uint8_t a = 0; a < 128 && i < maxNumberOfDevices; a++) {
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
 * Input: uint8_t &: the address provided
 * Output: bool: 1 if connection is good, 0 otherwise
 */
bool I2C::checkDeviceConnection(const uint8_t &addr) {
  if (addr > 127) return 0;
  Wire.beginTransmission(addr);
  return !Wire.endTransmission();
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
 *  uint8_t: the provided address (default: 0)
 *  uint8_t: ordinal (default: 1)
 * Output: bool: have to success to continue the program (always 1)
 */
bool SENSOR::sensorBegin(uint8_t addr, uint8_t ordinal) {
  Wire.begin();
  if (addr > 127) {
    addr = I2C::scanDeviceByOrdinal(ordinal);
    if (addr == 255) {
      Serial.println(F("Can't find any device! Auto-loop!!!"));
      delay(999);
      return sensorBegin();
    }
  } else if (!I2C::checkDeviceConnection(addr)) {
      Serial.println(F("Can't find device with provided address! Auto-scanning!!!"));
      return sensorBegin();
  }
  this->_id = getByte(_idAddr);
  this->_addr = addr;
  Serial.print(F("Detected "));
  this->showInfo();
  return 1;
}

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

char* SENSOR::getName() {
  return _name;
}

uint8_t SENSOR::getId() {
  return _id;
}

bool SENSOR::getBytes(int8_t bytes[], const uint8_t &numberOfBytes, int8_t failAlt) {
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

uint8_t SENSOR::getByte(const uint8_t &registerAddr, uint8_t failAlt) {
  uint8_t data[] = {registerAddr};
  this->getBytes(data, 1, failAlt);
  return *data;
}

uint16_t SENSOR::getWord(const uint8_t &registerAddr, uint16_t failAlt) {
  uint8_t data[2] = {registerAddr, 0};
  if (!this->getBytes(data, 2)) return failAlt;
  return data[0] << 8 | data[1];
}

bool SENSOR::sendByte(const uint8_t &registerAddr, const uint8_t &cwd) {
  Wire.beginTransmission(_addr);
  Wire.write(registerAddr);
  Wire.write(cwd);
  return !Wire.endTransmission();
}
