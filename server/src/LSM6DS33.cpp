#include "LSM6DS33.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

namespace BB {

#define WHO_AM_I 0x0F
#define CTRL1_XL 0x10
#define CTRL2_G 0x11
#define CTRL7_G 0x16
#define CTRL8_XL 0x17
#define CTRL9_XL 0x18

#define OUTX_L_XL 0x28
#define OUTX_H_XL 0x29
#define OUTY_L_XL 0x2A
#define OUTY_H_XL 0x2B
#define OUTZ_L_XL 0x2C
#define OUTZ_H_XL 0x2D

#define OUTX_L_G 0x22
#define OUTX_H_G 0x23
#define OUTY_L_G 0x24
#define OUTY_H_G 0x25
#define OUTZ_L_G 0x26
#define OUTZ_H_G 0x27

LSM6DS33::LSM6DS33(unsigned int I2CBus, unsigned int I2CAddress)
    : I2CDevice(I2CBus, I2CAddress) {
  this->I2CAddress = I2CAddress;
  this->I2CBus = I2CBus;
  this->ax = 0;
  this->ay = 0;
  this->az = 0;
  this->gx = 0;
  this->gy = 0;
  this->gz = 0;
  this->registers = NULL;
  this->write_register(CTRL1_XL, 0x30);
  this->write_register(CTRL2_G, 0x30);
}

short LSM6DS33::combine_registers(unsigned char msb, unsigned char lsb) {
  return ((short)msb << 8) | (short)lsb;
}

int LSM6DS33::read_sensor_state() {
  this->registers = this->read_registers(LSM6DS33_BUFFER_SIZE, 0x00);

  if (*(this->registers + (int)0x0f) != 0x69) {
    std::cerr << "LSM6DS33: Failure Condition - Sensor ID not Verified"
              << std::endl;
    return -1;
  }

  // Read accelerometer data
  this->ax = this->combine_registers(*(registers + OUTX_H_XL),
                                     *(registers + OUTX_L_XL));
  this->ay = this->combine_registers(*(registers + OUTY_H_XL),
                                     *(registers + OUTY_L_XL));
  this->az = this->combine_registers(*(registers + OUTZ_H_XL),
                                     *(registers + OUTZ_L_XL));

  // Read gyroscope data
  this->gx =
      this->combine_registers(*(registers + OUTX_H_G), *(registers + OUTX_L_G));
  this->gy =
      this->combine_registers(*(registers + OUTY_H_G), *(registers + OUTY_L_G));
  this->gz =
      this->combine_registers(*(registers + OUTZ_H_G), *(registers + OUTZ_L_G));

  return 0;
}

void LSM6DS33::debug() { this->debug_registers(); }

void LSM6DS33::display() {
  std::cout << "Accel(X,Y,Z): " << this->get_ax() << ", " << this->get_ay()
            << ", " << this->get_az() << " | Gyro(X,Y,Z): " << this->get_gx()
            << ", " << this->get_gy() << ", " << this->get_gz() << "     \r"
            << std::flush;
}

LSM6DS33::~LSM6DS33() {}

} // namespace BB
