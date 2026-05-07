#include "component/ScannerParser.h"

// SCANNER PINS - GPIO16, GPIO17
#define RX 16
#define TX 17

HardwareSerial Scanner(2); // Usar UART2

std::vector<std::string> ScannerParser::getData() {
    // Movemos el contenido a temp. buffer queda vacío automáticamente.
    std::vector<std::string> temp = std::move(buffer);
    
    return temp;
}

void ScannerParser::setup() {
    Scanner.begin(9600, SERIAL_8N1, RX, TX);
    Serial.println("Scanner listo...");
    
}



void ScannerParser::update() {
    if (Scanner.available()) {
        String data = Scanner.readStringUntil('\r'); // El GM865 envía un retorno de carro al final
        
        buffer.push_back(data.c_str());
        
        Serial.print("Lectura: ");
        Serial.println(data);
    }
}


