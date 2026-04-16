#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <Arduino.h> // Asegúrate de incluir esto para que reconozca uint8_t

class ConnectionManager {
    public:
        void setup();
        void update();
    private: 
        void publishRandInt(); 
        void reconnect();     
        static void callback(char* topic, byte* payload, unsigned int length);
};

#endif