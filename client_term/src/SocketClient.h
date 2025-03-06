#ifndef SOCKETCLIENT_H_
#define SOCKETCLIENT_H_

#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>

namespace BB {

/**
 * @class SocketClient
 * @brief A class that encapsulates a socket client to be used for network
 * communication
 */
class SocketClient {

private:
  int socketfd;
  struct sockaddr_in serverAddress;
  struct hostent *server;
  std::string serverName;
  int portNumber;
  bool isConnected;

public:
  SocketClient(std::string serverName, int portNumber);
  virtual int connectToServer();
  virtual int disconnectFromServer();
  virtual int send(std::string message);
  virtual std::string receive(int size);
  // virtual std::string receiveAll();
  bool isClientConnected() { return this->isConnected; }
  virtual ~SocketClient();
};

} // namespace BB

#endif /* SOCKETCLIENT_H_ */
