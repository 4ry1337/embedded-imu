#include "MadgwickAHRS.h"
#include "network/SocketServer.h"

#include <fcntl.h>
#include <iostream>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define LIS3MDL_ADDR 0x1E

#define OUTX_L_M 0x28
#define OUTX_H_M 0x29
#define OUTY_L_M 0x2A
#define OUTY_H_M 0x2B
#define OUTZ_L_M 0x2C
#define OUTZ_H_M 0x2D

#define CTRL_REG1 0x20
#define CTRL_REG2 0x21

#define M_SCALE 0.0001461f;

#include "LSM6DS33.h"
#include <iostream>
#include <unistd.h>

int main() {
  std::cout << "AltIMU Sensor Demonstration" << std::endl;

  try {
    exploringBB::LSM6DS33 imuSensor(2);

    for (int i = 0; i < 100; ++i) {
      imuSensor.read_sensor_state();

      std::cout << "IMU Data - Accel(X,Y,Z): " << imuSensor.get_acceleration_x()
                << ", " << imuSensor.get_acceleration_y() << ", "
                << imuSensor.get_acceleration_z()
                << " | Gyro(X,Y,Z): " << imuSensor.get_rotation_x() << ", "
                << imuSensor.get_rotation_y() << ", "
                << imuSensor.get_rotation_z()
                << " | Pitch: " << imuSensor.get_pitch()
                << " Roll: " << imuSensor.get_roll() << std::endl;

      usleep(100000); // 100ms delay
    }
  } catch (const std::exception &e) {
    std::cerr << "Error initializing or reading sensors: " << e.what()
              << std::endl;
    return 1;
  }

  return 0;
}

/* int main(int argc, char *argv[]) {
  std::cout << "Starting Beagle Board Server\n";
  exploringBB::SocketServer server(54321);
  if (server.listen() != 0) {
    std::cerr << "Socket Server failed to start." << std::endl;
    return 1;
  }
  std::cout << "Server started. Waiting for connection...\n";
  while (true) {
  }
  std::cout << "End of Beagle Board Server\n";
  return 0;
} */
