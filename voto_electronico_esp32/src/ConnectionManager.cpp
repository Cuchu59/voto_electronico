#include "ConnectionManager.h"
#include <Arduino.h> // Asegúrate de incluir esto para que reconozca uint8_t

// Para la conexion a wifi y el mqtt
#include <WiFi.h> 
WiFiClient WIFI_CLIENT;
#include <PubSubClient.h>
PubSubClient MQTT_CLIENT;

#define MY_SSID "UA-Alumnos"
#define MY_PASSWORD "41umn05WLC"
#define TOPIC "datos_voto_electronico"
#define TOPIC_vAleatorio "datos_voto_electronico/aleatorio"



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

  publishRandInt(); // publica un valor al azar
  
  MQTT_CLIENT.loop(); // El motor del cliente, llamamos para que se actualice y ejecute lo necesario (callbacks)
  
}

void ConnectionManager::publishRandInt() {
  // Publicar un mensaje. Publish.
  // Convierte el entero a char. DEBE ser char.
  int aleatorio = random(1,90);
  Serial.println(aleatorio);
  String aleatorioString = String(aleatorio);
  char alea[6];
  aleatorioString.toCharArray(alea, 6);
  MQTT_CLIENT.publish(TOPIC_vAleatorio, alea);
  
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