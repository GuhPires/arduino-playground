#include <SoftwareSerial.h>
SoftwareSerial BTSerial(11,10); //RX|TX

void setup(){
  Serial.begin(9600);
  BTSerial.begin(9600); // baud rate padrao
  while(!Serial);
  Serial.println("=== Comandos AT ===");
  Serial.println("SELECIONAR: Ambos, NL e CR");
  Serial.println("");
}

void loop(){
  // le do BLE e envia para a serial USB
  if(BTSerial.available()) {
    Serial.write(BTSerial.read());
  }

  // le da serial USB e envia para o BLE
  if(Serial.available()) {
    BTSerial.write(Serial.read());
  }
}
