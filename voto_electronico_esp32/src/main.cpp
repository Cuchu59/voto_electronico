#include <Arduino.h>
#include "component/ConnectionManager.h"
#include "component/QRDisplay.h"

// Components 
ConnectionManager coMan;    // MQTT connection  ->> Sub and Pub
QRDisplay qrDis;            // Display of the QR
  
void setup() {
  Serial.begin(115200);
  delay(10);
  coMan.setup();          // Setup MQTT Connection
  qrDis.setup();          // Setup Display
}

void loop() {
  coMan.update();         // Update the state of the MQTT manager
  qrDis.update();         // Update the display to draw
  delay(5000);
}
