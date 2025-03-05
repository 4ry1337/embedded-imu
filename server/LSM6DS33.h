#ifndef LSM6DS33_H_
#define LSM6DS33_H_
#include "I2CDevice.h"

#define LSM6DS33_BUFFER_SIZE 0x70

namespace exploringBB {

class LSM6DS33 : protected I2CDevice {
private:
  unsigned int I2CBus, I2CAddress;
  unsigned char *registers;

  short acceleration_x, acceleration_y, acceleration_z;
  short rotation_x, rotation_y, rotation_z;

  float pitch, roll, yaw;

  short combine_registers(unsigned char msb, unsigned char lsb);
  void calculate_orientation();

public:
  LSM6DS33(unsigned int I2CBus, unsigned int I2CAddress = 0x6A);
  virtual int read_sensor_state();

  virtual short get_acceleration_x() { return acceleration_x; }
  virtual short get_acceleration_y() { return acceleration_y; }
  virtual short get_acceleration_z() { return acceleration_z; }

  virtual short get_rotation_x() { return rotation_x; }
  virtual short get_rotation_y() { return rotation_y; }
  virtual short get_rotation_z() { return rotation_z; }

  virtual float get_pitch() { return pitch; }
  virtual float get_roll() { return roll; }
  virtual float get_yaw() { return yaw; }

  virtual void debug(int iterations = 600);
  virtual ~LSM6DS33();
};
} /* namespace exploringBB */

#endif /* LSM6DS33_H_ */
