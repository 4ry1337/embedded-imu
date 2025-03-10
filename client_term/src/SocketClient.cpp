#include "SocketClient.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

namespace BB {

SocketClient::SocketClient(std::string serverName, int portNumber) {
  this->socketfd = -1;
  this->server = NULL;
  this->serverName = serverName;
  this->portNumber = portNumber;
  this->isConnected = false;
}

int SocketClient::connectToServer() {
  socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketfd < 0) {
    perror("Socket Client: error opening socket.\n");
    return 1;
  }
  server = gethostbyname(serverName.data());
  if (server == NULL) {
    perror("Socket Client: error - no such host.\n");
    return 1;
  }
  bzero((char *)&serverAddress, sizeof(serverAddress));
  serverAddress.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serverAddress.sin_addr.s_addr,
        server->h_length);
  serverAddress.sin_port = htons(portNumber);

  if (connect(socketfd, (struct sockaddr *)&serverAddress,
              sizeof(serverAddress)) < 0) {
    perror("Socket Client: error connecting to the server.\n");
    return 1;
  }
  this->isConnected = true;
  return 0;
}

int SocketClient::send(std::string message) {
  const char *writeBuffer = message.data();
  int length = message.length();
  int n = write(this->socketfd, writeBuffer, length);
  if (n < 0) {
    perror("Socket Client: error writing to socket");
    return 1;
  }
  return 0;
}

std::string SocketClient::receive(int size = 1024) {
  char readBuffer[size];
  int n = read(this->socketfd, readBuffer, sizeof(readBuffer));
  if (n < 0) {
    perror("Socket Client: error reading from socket");
  }
  return std::string(readBuffer);
}

/*
string SocketClient::receiveAll(){
        //int recv_all(int sockfd, void *buf, size_t len, int flags)
    size_t toread = len;
    char  *bufptr = (char*) buf;
    while (toread > 0)
    {
        ssize_t rsz = recv(sockfd, bufptr, toread, 0);
        if (rsz <= 0)
            return rsz;  //Error or other end closed cnnection

        toread -= rsz;  // Read less next time
        bufptr += rsz;  // Next buffer position to read into
    }
    return len;
}
*/

int SocketClient::disconnectFromServer() {
  this->isConnected = false;
  close(this->socketfd);
  return 0;
}

SocketClient::~SocketClient() {
  if (this->isConnected == true) {
    disconnectFromServer();
  }
}

} // namespace BB
