#ifndef __SENSOR_I2C_H__
#define __SENSOR_I2C_H__

#include <Arduino.h>
#include <Wire.h>

namespace I2C {
uint8_t scanAllDevices(uint8_t address[], uint8_t maxNumberOfDevices = 128, bool justNumber = 0);
uint8_t scanDeviceByOrdinal(uint8_t ordinal = 1);
uint8_t numberOfDevices();
bool checkDeviceConnection(const uint8_t &addr);
void flush();
}

class SENSOR {
  public:
    SENSOR(const char *name, const char &idAddr): _idAddr(idAddr), _name(name) {};
    bool sensorBegin(uint8_t addr = 255, uint8_t ordinal = 1);

    void showInfo();
    char* getName();
    uint8_t getId();

    bool getBytes(int8_t data[], const uint8_t &numberOfBytes, int8_t failAlt = 0);
    uint8_t getByte(const uint8_t &registerAddr, uint8_t failAlt = 0);
    uint16_t getWord(const uint8_t &registerAddr, uint16_t failAlt = 0);
    bool sendByte(const uint8_t &registerAddr, const uint8_t &cwd);
  private:
    uint8_t _addr = 0, _idAddr, _id = 0;
    char* _name;
};

#endif
