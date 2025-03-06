#include "SocketClient.h"
#include <exception>
#include <iostream>
#include <sstream>
#include <vector>

int main(int argc, char *argv[]) {
  std::cout << "Starting Beagle Board Client Example" << std::endl;
  BB::SocketClient client("192.168.7.2", 54321);
  if (client.connectToServer() != 0) {
    std::cerr << "Error: Failed to connect to server." << std::endl;
    return 1;
  }
  std::cout << "Connected to server" << std::endl;

  while (true) {
    // Read from the server; expecting a CSV string "q1,q2,q3,q4"
    std::string data = client.receive(1024);
    if (data.empty()) {
      continue; // No data received; try again.
    }

    // Parse the CSV string.
    std::stringstream ss(data);
    std::string token;
    std::vector<float> qValues;
    while (getline(ss, token, ',')) {
      try {
        float q = stof(token);
        qValues.push_back(q);
      } catch (const std::exception &e) {
        std::cerr << "Parsing error for token: " << token << std::endl;
      }
    }

    // Check if we received all 4 components.
    if (qValues.size() == 4) {
      std::cout << "Quaternion: q1=" << qValues[0] << ", q2=" << qValues[1]
                << ", q3=" << qValues[2] << ", q4=" << qValues[3] << std::endl;
    } else {
      std::cout << "Incomplete data received: " << data << std::endl;
    }
  }

  client.disconnectFromServer();

  std::cout << "End of Beagle Board Client Example" << std::endl;
}
