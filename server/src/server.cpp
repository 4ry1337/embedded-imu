#include "AltIMU.h"
#include "SocketServer.h"
#include <iostream>
#include <unistd.h>

int main(int argc, char *argv[]) {
  /* std::cout << "Starting Beagle Board Server\n";
  BB::SocketServer server(54321);
  if (server.listen() != 0) {
    std::cerr << "Socket Server failed to start." << std::endl;
    return 1;
  }
  std::cout << "Server started. Waiting for connection...\n"; */
  BB::AltIMU imu(2);
  while (true) {
    imu.read_sensors_state();
    imu.display();
    usleep(100000);
  }
  /*   std::cout << "End of Beagle Board Server\n"; */
  return 0;
}
