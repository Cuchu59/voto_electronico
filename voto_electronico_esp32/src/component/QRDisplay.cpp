#include "QRDisplay.h"
#include <qrcode.h>

// Para el display 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// Declaración del display conectado por I2C (pines SDA, SCL)
// El -1 indica que el display no tiene un pin de reset físico
Adafruit_SSD1306 display(128, 64, &Wire, -1);

void QRDisplay::setup() {
    // Inicializar el display con la dirección I2C 0x3C (la más común)
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
        Serial.println(F("Fallo al inicializar la pantalla SSD1306"));
        for(;;); // Bucle infinito si falla
    }
    // Limpiar el buffer de la pantalla (borrar cualquier basura previa)
    display.clearDisplay();
    display.display();
}

void QRDisplay::update() {
    // Draws the QR into the display
    drawQR();
}


void QRDisplay::drawQR() {
  QRCode qrcode;
  // URL de tu EC2. Ajusta la versión (el '3') si la URL es muy larga.
  String url = "http://44.196.23.48/votar"; 
  
  uint8_t qrcodeData[qrcode_getBufferSize(3)];
  qrcode_initText(&qrcode, qrcodeData, 3, 0, url.c_str());

  display.clearDisplay();

  // Escalar el QR: cada punto del QR será un bloque de 2x2 píxeles para que se vea mejor
  int escala = 2;
  int x_offset = (128 - (qrcode.size * escala)) / 2;
  int y_offset = (64 - (qrcode.size * escala)) / 2;

  for (uint8_t y = 0; y < qrcode.size; y++) {
    for (uint8_t x = 0; x < qrcode.size; x++) {
      if (qrcode_getModule(&qrcode, x, y)) {
        display.fillRect(x * escala + x_offset, y * escala + y_offset, escala, escala, SSD1306_WHITE);
      }
    }
  }
  display.display();
}