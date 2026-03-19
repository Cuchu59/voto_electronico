#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);
int myFunction(int x,int y) {
  return x+y;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Running test");
  delay(5000);  // 5 seconds delay 
}

