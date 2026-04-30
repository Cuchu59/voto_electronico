#include <Arduino.h>
#include "component/ConnectionManager.h"
#include "component/QRDisplay.h"
#include "component/ScannerParser.h"

// Components 
ConnectionManager coMan;    // MQTT connection  ->> Sub and Pub
QRDisplay qrDis;            // Display of the QR
ScannerParser scPar;        // Class - reads - bar codes ... unga unga  


// Function definition (for thread creation)
void connectionTask(void * pvParameters);

/**
 * Start of the ESP32
 */
void setup() {
  Serial.begin(9600); delay(10);
  
  scPar.setup();          // Setup parser
  qrDis.setup();          // Setup Display
  coMan.setup();          // Setup MQTT Connection
  
  
  // Thread for the Connection Manager --> Independant of the Scanner and QR Display
  xTaskCreatePinnedToCore(
        connectionTask,   // Function to execute
        "ConnectionTask", // Internal name
        10000,            // Stack size (bytes)
        NULL,             // Parameters
        1,                // Priority
        NULL,             // Task handle
        0                 // Core 0 (loop runs at 1)
  );
  
}

/**
 * Main LOOP
 */
void loop() {
  scPar.update();         // Update the scanner parser
  qrDis.update();         // Update the display to draw
  
  delay(50);            // 50ms = 0.050s
  
}


/**
 * Connection function in thread at core 0
 */
void connectionTask(void * pvParameters) {
    for(;;) { // Bucle infinito de la tarea
        coMan.update(); 
        coMan.publishScannerData(scPar.getData());
        
        // The watchdog can reset the ESP32 so we add a delay to let the processor breathe
        vTaskDelay(pdMS_TO_TICKS(1000));  // Delay of 1s 
    }
}
