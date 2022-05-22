#include "MPU6050.h"

/*****Setting up*****/
void MPU6050::begin(uint8_t ordinal) {
  _scale[0] = DEF_SCALE_A;
  _scale[1] = DEF_SCALE_G;
  _smplrt = DEF_SMPLRT;
  _dlpf = DEF_DLPF;
  // calib
  _shrink = VECTOR_3D<double>(0.00005998, 0.00006065, 0.00006124);
  _offset[0] = VECTOR_3D<double>(523.4, 129, -4322.6);
  _offset[1] = VECTOR_3D<double>(-146, 129.2, -52.6);

  this->sensorBegin(ADDR_MPU6050, ordinal);
}

/*****Start to read*****/
void MPU6050::start() {
  _gyro2dAngle = (double)_scale[1] / 32768 * PI / 180;
  _delay = 1000/_smplrt;

  this->writeByte(this->REG_INT_EN, DEF_INT_EN);
  this->writeByte(this->REG_SMPLRT, (_dlpf ? 1000 : 8000) / _smplrt - 1);
  this->writeByte(this->REG_CONF_A, uint8_t(log(_scale[0] / 2) / log(2)) << 3);
  this->writeByte(this->REG_CONF_G, uint8_t(log(_scale[1] / 250) / log(2)) << 3);
  this->writeByte(this->REG_CONF, _dlpf);
  this->writeByte(this->REG_POWER1, DEF_TEMP_EN | DEF_CLOCK);
}

void MPU6050::calib(VALUE_TYPE type) {
  Serial.println(String(F("Start to calibrate ")) + this->getName());
  if (type == ACCEL | type == ALL) {
    Serial.println(String(F("Accel calibration! Accel will show data seperated by your input! Number of value: ")) + String(DEF_NUM_A));
    for (uint8_t i = 0; i < DEF_NUM_A; i++) {
      while (!Serial.available());
      Serial.readString();
      VECTOR_3D<double> sum;
      for (uint8_t j = 0; j < 50; j++) {
        while (!this->get<bool>(DRDY)) delay(1);
        this->readData(ACCEL);
        sum = sum + _data[0];
        delay(_delay);
      }
      (sum * (1.0 / 50)).print();
    }
  } Serial.println(F("Accel calibration done!"));
  
  if (type == GYRO | type == ALL) {
    Serial.println(String(F("Gyro calibration! Gyro will show continual data! Number of value: ")) + String(DEF_NUM_G));
    Serial.println(F("Write something to start!"));
    while (!Serial.available());
    Serial.readString();
    for (uint16_t i = 0; i < DEF_NUM_G; i++) {
      while (!this->get<bool>(this->DRDY)) delay(1);
      this->readData(GYRO);
      _data[1].print();
      delay(_delay);
    }
  } Serial.println(F("Gyro calibration done!"));
}

bool MPU6050::readData(VALUE_TYPE type) {
  if (type == ALL) {
    uint8_t a[14] = {REG_ACC_X};
    if (!this->readBytes(a, 14)) return 0;
    for (uint8_t i = 0; i < 6; i += 2) {
      _data[0].setElement(i / 2, a[i] << 8 | a[i + 1]);
      _data[1].setElement(i / 2, a[i + 8] << 8 | a[i + 9]);
    }
  } else if (type == ACCEL) {
    uint8_t a[6] = {REG_ACC_X};
    if (!this->readBytes(a, 6)) return 0;
    for (uint8_t i = 0; i < 6; i += 2)
      _data[0].setElement(i / 2, a[i] << 8 | a[i + 1]);
  } else if (type == GYRO) {
    uint8_t a[6] = {REG_GYRO_X};
    if (!this->readBytes(a, 6)) return 0;
    for (uint8_t i = 0; i < 6; i += 2)
      _data[1].setElement(i / 2, a[i] << 8 | a[i + 1]);
  }
  this->procData(type);
  return 1;
}

void MPU6050::reset() {
  this->writeByte(REG_POWER1, 0x80);
}
