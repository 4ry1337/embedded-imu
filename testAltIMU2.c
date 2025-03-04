#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define LSM6DS33 0x6B
#define LIS3MDL 0x1E

#define OUTX_L_G 0x22
#define OUTX_H_G 0x23
#define OUTY_L_G 0x24
#define OUTY_H_G 0x25
#define OUTZ_L_G 0x26
#define OUTZ_H_G 0x27
#define OUTX_L_A 0x28
#define OUTX_H_A 0x29
#define OUTY_L_A 0x2A
#define OUTY_H_A 0x2B
#define OUTZ_L_A 0x2C
#define OUTZ_H_A 0x2D
#define OUTX_L_M 0x28
#define OUTX_H_M 0x29
#define OUTY_L_M 0x2A
#define OUTY_H_M 0x2B
#define OUTZ_L_M 0x2C
#define OUTZ_H_M 0x2D

int writeRegister(int file, int addr, int reg, int value) {
  if (ioctl(file, I2C_SLAVE, addr) < 0) {
    perror("Failed to select I2C device");
    return -1;
  }
  char buffer[2] = {reg, value};
  if (write(file, buffer, 2) != 2) {
    perror("Failed to write to register");
    return -1;
  }
  return 0;
}

int16_t readRegisterPair(int file, int addr, int lowReg, int highReg) {
  char low, high;
  if (ioctl(file, I2C_SLAVE, addr) < 0) {
    perror("Failed to select I2C device");
    return -1;
  }

  if (write(file, &lowReg, 1) != 1 || read(file, &low, 1) != 1) {
    perror("Failed to read low byte");
    return -1;
  }

  if (write(file, &highReg, 1) != 1 || read(file, &high, 1) != 1) {
    perror("Failed to read high byte");
    return -1;
  }

  return (int16_t)((high << 8) | low);
}

int main() {
  int file;

  if ((file = open("/dev/i2c-2", O_RDWR)) < 0) {
    perror("Failed to open the bus");
    return 1;
  }

  writeRegister(file, LSM6DS33, 0x10, 0x30);
  writeRegister(file, LSM6DS33, 0x11, 0x30);
  writeRegister(file, LSM6DS33, 0x16, 0x00);
  writeRegister(file, LSM6DS33, 0x17, 0x00);
  writeRegister(file, LSM6DS33, 0x18, 0x38);
  writeRegister(file, LIS3MDL, 0x20, 0x0C);
  writeRegister(file, LIS3MDL, 0x21, 0x00);

  while (1) {
    int16_t accelX = readRegisterPair(file, LSM6DS33, OUTX_L_A, OUTX_H_A);
    int16_t accelY = readRegisterPair(file, LSM6DS33, OUTY_L_A, OUTY_H_A);
    int16_t accelZ = readRegisterPair(file, LSM6DS33, OUTZ_L_A, OUTZ_H_A);
    int16_t gyroX = readRegisterPair(file, LSM6DS33, OUTX_L_G, OUTX_H_G);
    int16_t gyroY = readRegisterPair(file, LSM6DS33, OUTY_L_G, OUTY_H_G);
    int16_t gyroZ = readRegisterPair(file, LSM6DS33, OUTZ_L_G, OUTZ_H_G);
    int16_t magX = readRegisterPair(file, LIS3MDL, OUTX_L_M, OUTX_H_M);
    int16_t magY = readRegisterPair(file, LIS3MDL, OUTY_L_M, OUTY_H_M);
    int16_t magZ = readRegisterPair(file, LIS3MDL, OUTZ_L_M, OUTZ_H_M);

    printf("\nAccelerometer (mg): X=%d Y=%d Z=%d\n", accelX, accelY, accelZ);
    printf("Gyroscope (dps): X=%d Y=%d Z=%d\n", gyroX, gyroY, gyroZ);
    printf("Magnetometer (gauss): X=%d Y=%d Z=%d\n", magX, magY, magZ);

    usleep(500000);
  }

  close(file);
  return 0;
}
