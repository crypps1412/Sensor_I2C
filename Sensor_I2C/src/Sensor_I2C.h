#ifndef __SENSOR_I2C_H__
#define __SENSOR_I2C_H__

#include <Arduino.h>
#include <Wire.h>

namespace I2C {
uint8_t scanAllDevices(uint8_t addr[], uint8_t maxDevices = 128, bool justNumber = 0);	// Get number and addresses of all I2C devices
uint8_t scanDeviceByOrdinal(uint8_t ordinal = 1);	// Get I2C device address by ordinal
uint8_t numberOfDevices();	// Get number of I2C devices
uint8_t checkDeviceConnection(const uint8_t &addr);	// Check I2C device connection
void flush();	// Flush all unused data left in buffer
}

class SENSOR {
  public:
    SENSOR(const char *deviceName, const uint8_t &idAddr): _idAddr(idAddr), _name(deviceName) {};	// Set sensor name and Id address
    bool sensorBegin(uint8_t addr = 255, uint8_t ordinal = 1);	// Check sensor address by user provided address or ordinal

    void showInfo();	// Print to serial sensor name, address and Id
    char* getName();	// Get sensor name pointer
    uint8_t getId();	// Get sensor Id

    bool readBytes(uint8_t bytes[], const uint8_t &numberOfBytes, uint8_t failAlt = 0);	// Read a number of bytes
    uint8_t readByte(const uint8_t &registerAddr, uint8_t failAlt = 0);	// Read a byte
    uint16_t readWord(const uint8_t &registerAddr, uint16_t failAlt = 0);	// Read 2 bytes
    bool writeByte(const uint8_t &registerAddr, const uint8_t &cwd);	// Write a byte to a register
  private:
    uint8_t _addr = 0, _idAddr, _id = 0;
    char* _name;
};

#endif
