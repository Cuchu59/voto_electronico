#include "component/ConnectionManager.h"
#include <Arduino.h> // Asegúrate de incluir esto para que reconozca uint8_t

// Para la conexion a wifi y el mqtt
#include <WiFi.h> 
WiFiClient WIFI_CLIENT;
#include <../src/external/PubSubClient.h>
PubSubClient MQTT_CLIENT;

#define MY_SSID "UA-Alumnos"
#define MY_PASSWORD "41umn05WLC"
#define TOPIC "datos_voto_electronico"
#define TOPIC_dni "datos_voto_electronico/dni"



void ConnectionManager::setup() {
Serial.println();
  Serial.print("Conectando con ");
  Serial.println(MY_SSID);

  WiFi.begin(MY_SSID, MY_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi conectado.");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("");

}

void ConnectionManager::update() {
// Continuamente comprueba si tiene conexión con MQTT,
  // en caso de que no tenga, reconecta.
  if (!MQTT_CLIENT.connected()) {
    reconnect();
  }
  
  MQTT_CLIENT.loop(); // El motor del cliente, llamamos para que se actualice y ejecute lo necesario (callbacks)
  
}

void ConnectionManager::publishScannerData(const std::vector<std::string>& data) {
    for (const std::string& dni : data) {
        // Publicamos cada DNI en el tópico configurado
        MQTT_CLIENT.publish(TOPIC_dni, dni.c_str());
        
        Serial.print("Enviando a AWS: ");
        Serial.println(dni.c_str());
    }
}


void ConnectionManager::callback(char* topic, byte* payload, unsigned int length) {
    Serial.println("");
    Serial.print("Mensaje recibido en el topic: ");
    Serial.println(topic);
    
    std::string message;
    for (int i = 0; i < length ; i++) {
        message += (char)payload[i];
    }
    
    Serial.print("Contenido:\t");
    Serial.println(message.c_str());

}


void ConnectionManager::reconnect() {
  // MQTT_CLIENT.setServer("192.168.1.206", 1883); // si uso un servidor local <ver IP correcta>
  IPAddress ip(44,196,23,48);
  MQTT_CLIENT.setServer(ip, 1883);  // servidor gratuito
  MQTT_CLIENT.setClient(WIFI_CLIENT);
  MQTT_CLIENT.setCallback(callback);  // Agregamos el callback para responder a la infomracion subida

  // Intentando conectar con el broker.
  while (!MQTT_CLIENT.connected()) {
    
    Serial.println("Intentando conectar con MQTT.");
    MQTT_CLIENT.connect(TOPIC); // Escribe cualquier nombre.
    MQTT_CLIENT.subscribe(TOPIC); // nos suscribimos al mismo topico para escuchar lo publicado
    
    // Espera antes de volver a intentarlo.
    delay(3000);
  }

  Serial.println("Conectado a MQTT.");
}