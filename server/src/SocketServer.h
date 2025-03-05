#ifndef SOCKETSERVER_H_
#define SOCKETSERVER_H_

#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>

namespace BB {

/**
 * @class SocketServer
 * @brief A class that encapsulates a server socket for network communication
 */
class SocketServer {
private:
  int portNumber;
  int socketfd, clientSocketfd;
  struct sockaddr_in serverAddress;
  struct sockaddr_in clientAddress;
  bool clientConnected;

public:
  SocketServer(int portNumber);
  virtual int listen();
  virtual int send(std::string message);
  virtual std::string receive(int size);

  virtual ~SocketServer();
};

} // namespace BB

#endif /* SOCKETSERVER_H_ */
