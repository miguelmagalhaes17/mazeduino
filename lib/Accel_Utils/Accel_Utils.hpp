#pragma once
#include <Arduino.h>
#include <Wire.h>

#define ACC_ADDR 0x19

struct AccelerometerData
{
  float x;
  float y;
  float z;
  float roll;
  float pitch;
};

extern TwoWire I2C_0;
extern TwoWire I2C_1;
extern AccelerometerData accel1;
extern AccelerometerData accel2;

void i2cWrite8(TwoWire &bus, uint8_t addr, uint8_t reg, uint8_t val);
void i2cReadBytes(TwoWire &bus, uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t len);
void initAccel(TwoWire &bus);
void readData(AccelerometerData &accel, TwoWire &bus);