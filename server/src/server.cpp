#include "AltIMU.h"
#include "SocketServer.h"
#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>

std::atomic<bool> gTerminate(false);

void signalHandler(int signum) {
  gTerminate = true;
  std::cout << "\nSignal (" << signum << ") received. Terminating server..."
            << std::endl;
}

int main(int argc, char *argv[]) {
  std::signal(SIGINT, signalHandler);
  std::signal(SIGTERM, signalHandler);

  std::cout << "Starting Beagle Board Server" << std::endl;
  BB::AltIMU imu(2);
  while (true) {
    BB::SocketServer server(54321);
    if (server.listen() != 0) {
      std::cerr << "Socket Server failed to start." << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));
      continue;
    }
    std::cout << "Listening for connection..." << std::endl;
    while (true) {
      imu.read_sensors_state();
      std::string quaternionData = imu.getQuaternionString();
      if (server.send(quaternionData) != 0) {
        std::cerr << "Error sending data to client." << std::endl;
      }
      std::cout << "Data: " << quaternionData << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }
  std::cout << "End of Beagle Board Server" << std::endl;
  return 0;
}
