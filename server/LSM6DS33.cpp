#include "LSM6DS33.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

namespace exploringBB {

// LSM6DS33 Register Addresses
#define WHO_AM_I 0x0F
#define CTRL1_XL 0x10 // Accelerometer control register
#define CTRL2_G 0x11  // Gyroscope control register
#define CTRL7_G 0x16
#define CTRL8_XL 0x17
#define CTRL9_XL 0x18

#define OUTX_L_XL 0x28 // Accelerometer X-axis low byte
#define OUTX_H_XL 0x29 // Accelerometer X-axis high byte
#define OUTY_L_XL 0x2A // Accelerometer Y-axis low byte
#define OUTY_H_XL 0x2B // Accelerometer Y-axis high byte
#define OUTZ_L_XL 0x2C // Accelerometer Z-axis low byte
#define OUTZ_H_XL 0x2D // Accelerometer Z-axis high byte

#define OUTX_L_G 0x22 // Gyroscope X-axis low byte
#define OUTX_H_G 0x23 // Gyroscope X-axis high byte
#define OUTY_L_G 0x24 // Gyroscope Y-axis low byte
#define OUTY_H_G 0x25 // Gyroscope Y-axis high byte
#define OUTZ_L_G 0x26 // Gyroscope Z-axis low byte
#define OUTZ_H_G 0x27 // Gyroscope Z-axis high byte

#define LA_SCALE 0.061f * 9.81f * 0.001f;
#define G_SCALE 8.75f * 0.001f;

LSM6DS33::LSM6DS33(unsigned int I2CBus, unsigned int I2CAddress)
    : I2CDevice(I2CBus, I2CAddress) {
  this->I2CAddress = I2CAddress;
  this->I2CBus = I2CBus;
  this->acceleration_x = 0;
  this->acceleration_y = 0;
  this->acceleration_z = 0;
  this->rotation_x = 0;
  this->rotation_y = 0;
  this->rotation_z = 0;
  this->pitch = 0.0f;
  this->roll = 0.0f;
  this->yaw = 0.0f;
  this->registers = NULL;
  this->write_register(CTRL1_XL, 0x30);
  this->write_register(CTRL2_G, 0x30);
}

short LSM6DS33::combine_registers(unsigned char msb, unsigned char lsb) {
  return ((short)msb << 8) | (short)lsb;
}

void LSM6DS33::calculate_orientation() {
  float acc_x = this->acceleration_x * LA_SCALE;
  float acc_y = this->acceleration_y * LA_SCALE;
  float acc_z = this->acceleration_z * LA_SCALE;

  this->pitch = 180 * atan(acc_x / sqrt(acc_y * acc_y + acc_z * acc_z)) / M_PI;
  this->roll = 180 * atan(acc_y / sqrt(acc_x * acc_x + acc_z * acc_z)) / M_PI;
}

int LSM6DS33::read_sensor_state() {
  this->registers = this->read_registers(LSM6DS33_BUFFER_SIZE, 0x00);

  if (*(this->registers) != 0x69) {
    perror("LSM6DS33: Failure Condition - Sensor ID not Verified");
    return -1;
  }

  // Read accelerometer data
  this->acceleration_x = this->combine_registers(*(registers + OUTX_H_XL),
                                                 *(registers + OUTX_L_XL));
  this->acceleration_y = this->combine_registers(*(registers + OUTY_H_XL),
                                                 *(registers + OUTY_L_XL));
  this->acceleration_z = this->combine_registers(*(registers + OUTZ_H_XL),
                                                 *(registers + OUTZ_L_XL));

  // Read gyroscope data
  this->rotation_x =
      this->combine_registers(*(registers + OUTX_H_G), *(registers + OUTX_L_G));
  this->rotation_y =
      this->combine_registers(*(registers + OUTY_H_G), *(registers + OUTY_L_G));
  this->rotation_z =
      this->combine_registers(*(registers + OUTZ_H_G), *(registers + OUTZ_L_G));

  this->calculate_orientation();
  return 0;
}

void LSM6DS33::debug(int iterations) {
  int count = 0;
  while (count < iterations) {
    cout << "Accel(X,Y,Z): " << this->get_acceleration_x() << ", "
         << this->get_acceleration_y() << ", " << this->get_acceleration_z()
         << " | Gyro(X,Y,Z): " << this->get_rotation_x() << ", "
         << this->get_rotation_y() << ", " << this->get_rotation_z()
         << " | Pitch:" << this->get_pitch() << " Roll:" << this->get_roll()
         << "     \r" << flush;
    usleep(100000);
    this->read_sensor_state();
    count++;
  }
}

LSM6DS33::~LSM6DS33() {}

} /* namespace exploringBB */
