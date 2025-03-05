#include "AltIMU.h"
#include "SocketServer.h"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>

int main(int argc, char *argv[]) {
  std::cout << "Starting Beagle Board Server" << std::endl;
  BB::SocketServer server(54321);
  if (server.listen() != 0) {
    std::cerr << "Socket Server failed to start." << std::endl;
    return 1;
  }
  std::cout << "Server started. Waiting for connection..." << std::endl;
  BB::AltIMU imu(2);
  while (true) {
    imu.read_sensors_state();
    std::string quaternionData = imu.getQuaternionString();
    if (server.send(quaternionData) != 0) {
      std::cerr << "Error sending data to client." << std::endl;
      break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  std::cout << "End of Beagle Board Server" << std::endl;
  return 0;
}
