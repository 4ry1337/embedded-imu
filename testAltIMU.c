#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define LSM6DS33 0x6B
#define LIS3MDL 0x1E

#define CTRL1_XL 0x10
#define CTRL2_G 0x11
#define CTRL7_G 0x16
#define CTRL8_XL 0x17
#define CTRL9_XL 0x18

#define CTRL_REG1 0x20
#define CTRL_REG2 0x21

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

  printf("Starting the AltuIMU sensor application\n");

  if ((file = open("/dev/i2c-2", O_RDWR)) < 0) {
    perror("Failed to open I2C bus");
    return 1;
  }

  if (ioctl(file, I2C_SLAVE, LSM6DS33) < 0) {
    perror("Failed to connect to LSM6DS33");
    return 1;
  }

  writeRegister(file, CTRL1_XL, 0x30);
  writeRegister(file, CTRL2_G, 0x30);
  writeRegister(file, CTRL7_G, 0x00);
  writeRegister(file, CTRL8_XL, 0x00);
  writeRegister(file, CTRL9_XL, 0x38);

  short gyro_x =
      combineValues(readRegister(file, OUTX_H_G), readRegister(file, OUTX_L_G));
  short gyro_y =
      combineValues(readRegister(file, OUTY_H_G), readRegister(file, OUTY_L_G));
  short gyro_z =
      combineValues(readRegister(file, OUTZ_H_G), readRegister(file, OUTZ_L_G));

  short accel_x = combineValues(readRegister(file, OUTX_H_XL),
                                readRegister(file, OUTX_L_XL));
  short accel_y = combineValues(readRegister(file, OUTY_H_XL),
                                readRegister(file, OUTY_L_XL));
  short accel_z = combineValues(readRegister(file, OUTZ_H_XL),
                                readRegister(file, OUTZ_L_XL));

  if (ioctl(file, I2C_SLAVE, LIS3MDL) < 0) {
    perror("Failed to connect to LIS3MDL");
    return 1;
  }

  writeRegister(file, CTRL_REG1, 0x0C);
  writeRegister(file, CTRL_REG2, 0x00);

  short mag_x =
      combineValues(readRegister(file, OUTX_H_M), readRegister(file, OUTX_L_M));
  short mag_y =
      combineValues(readRegister(file, OUTY_H_M), readRegister(file, OUTY_L_M));
  short mag_z =
      combineValues(readRegister(file, OUTZ_H_M), readRegister(file, OUTZ_L_M));

  printf("The Device ID is: 6B (LSM6DS33)\n");
  printf("X=%d Y=%d Z=%d (Gyroscope)\n", gyro_x, gyro_y, gyro_z);
  printf("X=%d Y=%d Z=%d (Accelerometer)\n", accel_x, accel_y, accel_z);
  printf("X=%d Y=%d Z=%d (Magnetometer)\n", mag_x, mag_y, mag_z);

  close(file);
  return 0;
}
