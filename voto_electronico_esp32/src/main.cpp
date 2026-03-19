#include <Arduino.h>
/*
   ESP8266 o ESP32 (ver las modificaciones)
   Este script sube al server un valor "aleatorio" en el topic "PCOLUNGA_EL_MEJOR_PROFE_DE_ANAYDIS" 

*/

#include <WiFi.h> // Para el ESP32
WiFiClient WIFI_CLIENT;

#include <PubSubClient.h>
PubSubClient MQTT_CLIENT;

// Nombre y contraseña de tu red WiFi.
const char* ssid = "UA-Alumnos";
const char* password = "41umn05WLC";

// Def of functions
void reconnect();

void setup() {
  Serial.begin(115200);
  
  delay(10);

  Serial.println();
  Serial.print("Conectando con ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado.");
  Serial.println("IP: ");
  Serial.print(WiFi.localIP());
}

void loop() {
  // Continuamente comprueba si tiene conexión con MQTT,
  // en caso de que no tenga, reconecta.
  if (!MQTT_CLIENT.connected()) {
    reconnect();
  }

  // Publicar un mensaje. Publish.
  // Convierte el entero a char. DEBE ser char.
  int aleatorio = random(1,90);
  Serial.println(aleatorio);
  String aleatorioString = String(aleatorio);
  char alea[6];
  aleatorioString.toCharArray(alea, 6);

  //                   Topic / valor
  MQTT_CLIENT.publish("PCOLUNGA_EL_MEJOR_PROFE_DE_ANAYDIS/aleatorio", alea);

  // Espera antes de Publicar otro aleatorio.
  delay(5000);
}

// Reconecta con MQTT broker
void reconnect() {
  // MQTT_CLIENT.setServer("192.168.1.206", 1883); // si uso un servidor local <ver IP correcta>
    MQTT_CLIENT.setServer("broker.hivemq.com", 1883);  // servidor gratuito

  MQTT_CLIENT.setClient(WIFI_CLIENT);

  // Intentando conectar con el broker.
  while (!MQTT_CLIENT.connected()) {
    Serial.println("Intentando conectar con MQTT.");
    MQTT_CLIENT.connect("PCOLUNGA_EL_MEJOR_PROFE_DE_ANAYDIS"); // Escribe cualquier nombre.

    // Espera antes de volver a intentarlo.
    delay(3000);
  }

  Serial.println("Conectado a MQTT.");
}

