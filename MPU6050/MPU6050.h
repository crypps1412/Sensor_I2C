#ifndef __MPU6050_H__
#define __MPU6050_H__

#include <Arduino.h>
#include <Sensor_I2C.h>
#include "class3D.h"

#define ADDR_MPU6050 0x68

class MPU6050: public SENSOR
{
  private:
    uint8_t _dlpf, _delay; // digital low-pass filter, ms delay between reads
    uint16_t _scale[2], _smplrt; // scale (accel: 0, gyro: 1), sample rate
    VECTOR_3D<int16_t> _data[2];
    double _gyro2dAngle;  // gyro --> rad/s

    VECTOR_3D<double> _shrink;  // 3 axis-scale value (myAx in matlab)
    VECTOR_3D<double> _offset[2]; // 3 center position value (center in matlab)
    VECTOR_3D<double> _proc[2], _dProc;

  public:
    enum VALUE_TYPE {
      SCALE_A, SCALE_G, SMPL_RATE, DLP_FILTER, DRDY,
      ACCEL, GYRO, ALL,
      PROC_A, PROC_G, PROC_AX, PROC_AY, PROC_AZ, PROC_GX, PROC_GY, PROC_GZ,
      DATA_A, DATA_G, DATA_AX, DATA_AY, DATA_AZ, DATA_GX, DATA_GY, DATA_GZ
    };
    enum REG {
      REG_ID = 0x75,      REG_SMPLRT = 0x19,  REG_INT_STT = 0x3A,
      REG_INT_EN = 0x38,  REG_POWER1 = 0x6B,
      REG_CONF = 0x1A,    REG_CONF_A = 0x1C,  REG_CONF_G = 0x1B,
      REG_ACC_X = 0x3B,   REG_ACC_Y = 0x3D,   REG_ACC_Z = 0x3F,
      REG_GYRO_X = 0x43,  REG_GYRO_Y = 0x45,  REG_GYRO_Z = 0x47
    };
    enum DEFAULT_VALUE {
      DEF_SCALE_A = 4,   // 4*g
      DEF_SCALE_G = 2000,  // +-2000 degrees per second
      DEF_DLPF = 0, // no digital low-pass filter
      DEF_CLOCK = 3,  // gyro X based clock
      DEF_TEMP_EN = 0b1000, // disable temperature sensor
      DEF_SMPLRT = 250, // sample rate 250Hz
      DEF_INT_EN = 1,  // enable interrupt
      DEF_NUM_A = 20, // number of accel value to calib
      DEF_NUM_G = 10000  // number of gyro value to calib
    };

    /*****Setting up*****/
    MPU6050(const char *name): SENSOR(name, REG_ID) {};
    MPU6050(): MPU6050("MPU6050") {};
    void begin(uint8_t ordinal = 1);
    template<class T> bool set(const VALUE_TYPE& type, const T& value);
    template<class T> T get(const VALUE_TYPE& type);

    /*****Start to read*****/
    void start(); // Implement all setting and activate the sensor measurement mode
    void calib(VALUE_TYPE type = ALL);
    bool readData(VALUE_TYPE type = ALL);
    void reset();
    
  protected:
    /*****Calculate*****/
    void procData(const VALUE_TYPE& type) {
      if (type == ACCEL | type == ALL)
        _proc[0] = (VECTOR_3D<double>(_data[0]) - _offset[0]) * _shrink;
      if (type == GYRO | type == ALL)
        _proc[1] = (VECTOR_3D<double>(_data[1]) - _offset[1]) * _gyro2dAngle;
    }
};

template<class T> bool MPU6050::set(const VALUE_TYPE& type, const T& val) {
  if (type == SCALE_A || type == SCALE_G) _scale[type - SCALE_A] = val;
  else if (type == SMPL_RATE) _smplrt = val;
  else if (type == DLP_FILTER) _dlpf = val;
  else return 0;
  return 1;
}

template<class T> T MPU6050::get(const VALUE_TYPE& type) {
  if (type == SCALE_A || type == SCALE_G) return _scale[type - SCALE_A];
  else if (type == SMPL_RATE) return _smplrt;
  else if (type == DLP_FILTER) return _dlpf;
  else if (type == DRDY) return this->readByte(REG_INT_STT) & 1;
  else if (type >= DATA_AX && type <= DATA_AZ) return _data[0][type - DATA_AX];
  else if (type == DATA_A) return _data[0];
  else if (type >= DATA_GX && type <= DATA_GZ) return _data[1][type - DATA_GX];
  else if (type == DATA_G) return _data[1];
  else if (type >= PROC_AX && type <= PROC_AZ) return _proc[0][type - PROC_AX];
  else if (type == PROC_A) return _proc[0];
  else if (type >= PROC_GX && type <= PROC_GZ) return _proc[1][type - PROC_GX];
  else if (type == PROC_G) return _proc[1];
  return 0.;
}

#endif
