#include "Accel_Utils.hpp"

TwoWire I2C_0 = TwoWire(0);
TwoWire I2C_1 = TwoWire(1);

AccelerometerData accel1;
AccelerometerData accel2;

void i2cWrite8(TwoWire &bus, uint8_t addr, uint8_t reg, uint8_t val) {
  bus.beginTransmission(addr);
  bus.write(reg);
  bus.write(val);
  bus.endTransmission();
}

void i2cReadBytes(TwoWire &bus, uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t len) {
  bus.beginTransmission(addr);
  bus.write(reg | 0x80); // auto-increment
  bus.endTransmission(false);
  bus.requestFrom(addr, len);
  for (int i = 0; i < len; i++) {
    buf[i] = bus.read();
  }
}

void initAccel(TwoWire &bus) {
  // 100 Hz, X/Y/Z enabled
  i2cWrite8(bus, ACC_ADDR, 0x20, 0b01010111);

  // ±2g, high resolution
  i2cWrite8(bus, ACC_ADDR, 0x23, 0b00001000);
}

void readData(AccelerometerData &accel, TwoWire &bus) {
  uint8_t buf[6];
  int16_t tempX;
  int16_t tempY;
  int16_t tempZ;

  i2cReadBytes(bus, ACC_ADDR, 0x28, buf, 6);

  tempX = (int16_t)(buf[1] << 8 | buf[0]) >> 4;
  tempY = (int16_t)(buf[3] << 8 | buf[2]) >> 4;
  tempZ = (int16_t)(buf[5] << 8 | buf[4]) >> 4;

  accel.x = (float)tempX;
  accel.y = (float)tempY;
  accel.z = (float)tempZ;
  accel.roll  = atan2(accel.y, accel.z) * 180.0 / PI;
  accel.pitch = atan2(-accel.x, sqrt(accel.y*accel.y + accel.z*accel.z)) * 180.0 / PI;
}