#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

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