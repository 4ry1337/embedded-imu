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
    std::string data = client.receive(1024);
    if (data.empty()) {
      continue;
    }

    std::stringstream ss(data);
    std::string token;
    std::vector<float> qs;

    while (getline(ss, token, ',')) {
      try {
        float q = stof(token);
        qs.push_back(q);
      } catch (const std::exception &e) {
        std::cerr << "Parsing error for token: " << token << std::endl;
      }
    }

    if (qs.size() == 4) {
      std::cout << "q1=" << qs[0] << ", q2=" << qs[1] << ", q3=" << qs[2]
                << ", q4=" << qs[3] << std::endl;
    } else {
      std::cout << "Incomplete data received: " << data << std::endl;
    }
  }

  client.disconnectFromServer();

  std::cout << "End of Beagle Board Client" << std::endl;
}
