#ifndef ALTIMU_H_
#define ALTIMU_H_

#include "LIS3MDL.h"
#include "LSM6DS33.h"
#include <string>

namespace BB {

#define LA_SCALE 0.061 * 9.81 * 1000.0;
#define G_SCALE 8.75 / 1000.0;
#define M_SCALE 1.0 / 6842.0;

#define sampleFreq 512.0f // sample frequency in Hz
#define betaDef 0.1f      // 2 * proportional gain

class AltIMU {
private:
  LSM6DS33 imu_sensor;
  LIS3MDL mag_senseor;
  float beta;
  float q0, q1, q2, q3;

  void MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay,
                          float az, float mx, float my, float mz);
  void MadgwickAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay,
                             float az);
  float invSqrt(float x);
  void calculate_quaternions();

public:
  AltIMU(unsigned int I2CBus, unsigned int imu_address = 0x6B,
         unsigned int mag_address = 0x1E);
  virtual int read_sensors_state();
  virtual void debug();
  virtual void display();
  virtual ~AltIMU();
  virtual std::string getQuaternionString();
};
} // namespace BB

#endif // !ALTIMU_H_
