#include "LIS3MDL.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

namespace BB {

#define LIS3MDL_ADDR 0x1E
#define LIS3MDL_BUFFER_SIZE 0x34

#define OUTX_L_M 0x28
#define OUTX_H_M 0x29
#define OUTY_L_M 0x2A
#define OUTY_H_M 0x2B
#define OUTZ_L_M 0x2C
#define OUTZ_H_M 0x2D

#define CTRL_REG1 0x20
#define CTRL_REG2 0x21

LIS3MDL::LIS3MDL(unsigned int I2CBus, unsigned int I2CAddress)
    : I2CDevice(I2CBus, I2CAddress) {
  this->I2CAddress = I2CAddress;
  this->I2CBus = I2CBus;
  this->mx = 0;
  this->my = 0;
  this->mz = 0;
  this->registers = NULL;
  this->write_register(CTRL_REG1, 0x0C);
  this->write_register(CTRL_REG2, 0x00);
}

short LIS3MDL::combine_registers(unsigned char msb, unsigned char lsb) {
  return ((short)msb << 8) | (short)lsb;
}

int LIS3MDL::read_sensor_state() {
  this->registers = this->read_registers(LIS3MDL_BUFFER_SIZE, 0x00);

  if (*(this->registers + (int)0x0f) != 0x3D) {
    std::cerr << "LIS3MDL: Failure Condition - Sensor ID not Verified"
              << std::endl;
    return 1;
  }

  this->mx =
      this->combine_registers(*(registers + OUTX_H_M), *(registers + OUTX_L_M));
  this->my =
      this->combine_registers(*(registers + OUTY_H_M), *(registers + OUTY_L_M));
  this->mz =
      this->combine_registers(*(registers + OUTZ_H_M), *(registers + OUTZ_L_M));

  return 0;
}

void LIS3MDL::debug() { this->debug_registers(); }

void LIS3MDL::display() {
  std::cout << "Magnet(X,Y,Z): " << this->get_mx() << ", " << this->get_my()
            << ", " << this->get_mz() << "     \r" << std::flush;
}

LIS3MDL::~LIS3MDL() {}

} /* namespace BB */
