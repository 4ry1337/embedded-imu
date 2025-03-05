#ifndef I2C_H_
#define I2C_H_

#define BBB_I2C_0 "/dev/i2c-0"
#define BBB_I2C_1 "/dev/i2c-1"
#define BBB_I2C_2 "/dev/i2c-2"

namespace BB {

/**
 * @class I2CDevice
 * @brief Generic I2C Device class that can be used to connect to any type of
 * I2C device and read or write to its registers
 */
class I2CDevice {
private:
  unsigned int bus;
  unsigned int device;
  int file;

public:
  I2CDevice(unsigned int bus, unsigned int device);
  virtual int open();
  virtual int write(unsigned char value);
  virtual unsigned char read_register(unsigned int registerAddress);
  virtual unsigned char *read_registers(unsigned int number,
                                        unsigned int fromAddress = 0);
  virtual int write_register(unsigned int registerAddress, unsigned char value);
  virtual void debug_registers(unsigned int number = 0xff);
  virtual void close();
  virtual ~I2CDevice();
};

} /* namespace BB */

#endif /* I2C_H_ */
