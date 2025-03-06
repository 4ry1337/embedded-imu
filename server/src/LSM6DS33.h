#ifndef LSM6DS33_H_
#define LSM6DS33_H_

#include "I2CDevice.h"

namespace BB {

class LSM6DS33 : protected I2CDevice {
private:
  unsigned int I2CBus, I2CAddress;
  unsigned char *registers;

  short ax, ay, az;
  short gx, gy, gz;

  short combine_registers(unsigned char msb, unsigned char lsb);

public:
  LSM6DS33(unsigned int I2CBus, unsigned int I2CAddress = 0x6B);
  virtual int read_sensor_state();

  virtual short get_ax() { return ax; }
  virtual short get_ay() { return ay; }
  virtual short get_az() { return az; }

  virtual short get_gx() { return gx; }
  virtual short get_gy() { return gy; }
  virtual short get_gz() { return gz; }

  virtual void debug();
  virtual void display();
  virtual ~LSM6DS33();
};
} /* namespace BB */

#endif /* LSM6DS33_H_ */
