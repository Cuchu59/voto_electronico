#include "component/Scanner.h"

// Pines definidos según tu nueva configuración
#define RX_PIN 16
#define TX_PIN 17

HardwareSerial HScanner(2); 

void Scanner::setup() {
    // Probamos 9600 primero (estándar de fábrica) si 115200 falla
    HScanner.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN); 
    Serial.println("--- Scan System Initialized (Pines 16/17) ---");
}

void Scanner::update() {
    // Mientras haya bytes en el buffer de hardware de la UART
    while (HScanner.available() > 0) {
        int raw = HScanner.read();
        
        if (raw > 0) { 
            char c = (char)raw;

            // Si es un terminador, cerramos la palabra/DNI
            if (c == '\r' || c == '\n') {
                if (!currentLine.empty()) {
                    // Guardamos la lectura completa en el vector de mensajes
                    buffer.push_back(currentLine); 
                    Serial.print(" -> Lectura guardada: ");
                    Serial.println(currentLine.c_str());
                    
                    // Limpiamos el acumulador para la siguiente lectura
                    currentLine.clear(); 
                }
            } 
            else {
                // Si es un caracter imprimible, lo acumulamos
                currentLine += c;
            }
        }
    }
}

std::vector<std::string> Scanner::getData() {
    return std::move(buffer); //
}