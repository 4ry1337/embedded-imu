#!/bin/bash
mkdir -p build
# g++ client.cpp network/SocketClient.cpp -o client
# g++ server.cpp network/SocketServer.cpp -o server
g++ server.cpp I2CDevice.cpp LSM6DS33.cpp -o build/server
