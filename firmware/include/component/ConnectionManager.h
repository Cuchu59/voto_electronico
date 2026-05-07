#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H
#include <Arduino.h> // Asegúrate de incluir esto para que reconozca uint8_t
#include <iostream>
#include <vector>

class ConnectionManager {
    public:
        void setup();
        void update();
        void publishScannerData(const std::vector<std::string>& data);
    private: 
        void reconnect();     
        static void callback(char* topic, byte* payload, unsigned int length);
};

#endif