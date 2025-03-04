#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "MadgwickAHRS.h"

#define LSM6DS33_ADDR 0x6B
#define LIS3MDL_ADDR 0x1E

#define OUTX_L_G 0x22
#define OUTX_H_G 0x23
#define OUTY_L_G 0x24
#define OUTY_H_G 0x25
#define OUTZ_L_G 0x26
#define OUTZ_H_G 0x27

#define OUTX_L_XL 0x28
#define OUTX_H_XL 0x29
#define OUTY_L_XL 0x2A
#define OUTY_H_XL 0x2B
#define OUTZ_L_XL 0x2C
#define OUTZ_H_XL 0x2D

#define OUTX_L_M 0x28
#define OUTX_H_M 0x29
#define OUTY_L_M 0x2A
#define OUTY_H_M 0x2B
#define OUTZ_L_M 0x2C
#define OUTZ_H_M 0x2D

#define la_scale 0.061f * 9.81f * 0.001f;
#define g_scale 8.75f * 0.001f;
#define m_scale 0.0001461f;

int writeRegister(int file, unsigned char reg, unsigned char value) {
  char buffer[2] = {reg, value};
  if (write(file, buffer, 2) != 2) {
    perror("Failed to write to register");
    return 1;
  }
  return 0;
}

unsigned char readRegister(int file, unsigned char reg) {
  char buffer[1] = {reg};
  if (write(file, buffer, 1) != 1) {
    perror("Failed to set register address");
    return 1;
  }
  char value[1];
  if (read(file, value, 1) != 1) {
    perror("Failed to read register value");
    return 1;
  }
  return value[0];
}

short combineValues(unsigned char high, unsigned char low) {
  return ((short)high << 8) | (short)low;
}

int main() {
  int file;
  printf("Starting the IMU sensor application\n");
  if ((file = open("/dev/i2c-2", O_RDWR)) < 0) {
    perror("Failed to open I2C bus");
    return 1;
  }
  if (ioctl(file, I2C_SLAVE, LSM6DS33_ADDR) < 0) {
    perror("Failed to connect to LSM6DS33");
    return 1;
  }

  short g_x = combineValues(readRegister(file, OUTX_H_G),
                            readRegister(file, OUTX_L_G)),
        g_y = combineValues(readRegister(file, OUTY_H_G),
                            readRegister(file, OUTY_L_G)),
        g_z = combineValues(readRegister(file, OUTZ_H_G),
                            readRegister(file, OUTZ_L_G));
  short la_x = combineValues(readRegister(file, OUTX_H_XL),
                             readRegister(file, OUTX_L_XL)),
        la_y = combineValues(readRegister(file, OUTY_H_XL),
                             readRegister(file, OUTY_L_XL)),
        la_z = combineValues(readRegister(file, OUTZ_H_XL),
                             readRegister(file, OUTZ_L_XL));

  if (ioctl(file, I2C_SLAVE, LIS3MDL_ADDR) < 0) {
    perror("Failed to connect to LIS3MDL");
    return 1;
  }

  short m_x = combineValues(readRegister(file, OUTX_H_M),
                            readRegister(file, OUTX_L_M)),
        m_y = combineValues(readRegister(file, OUTY_H_M),
                            readRegister(file, OUTY_L_M)),
        m_z = combineValues(readRegister(file, OUTZ_H_M),
                            readRegister(file, OUTZ_L_M));

  float gx = g_x * g_scale;
  float gy = g_y * g_scale;
  float gz = g_z * g_scale;

  float ax = la_x * la_scale;
  float ay = la_y * la_scale;
  float az = la_z * la_scale;

  float mx = m_x * m_scale;
  float my = m_y * m_scale;
  float mz = m_z * m_scale;

  MadgwickAHRSupdate(gx, gy, gz, ax, ay, az, mx, my, mz);
  printf("Quaternion - q0=%.6f q1=%.6f q2=%.6f q3=%.6f\n", q0, q1, q2, q3);
  close(file);
  return 0;
}
