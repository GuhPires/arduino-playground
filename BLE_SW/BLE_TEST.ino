/*
 * 
 *          ---- BLUETOOTH COMUNICATION TEST ----
 * 
 * Simple Sketch that tests and logs all incoming data from BLE.
 *  
 * Author: GuhPires
 * 
 */


#include <SoftwareSerial.h>
// Creates a new object from the Software Serial class
SoftwareSerial BTSerial(11, 10); //RX|TX

void setup() {
  // Starts both Serials
  Serial.begin(9600);
  BTSerial.begin(9600);
  // Waits until the Arduino's Serial become available
  while(!Serial);
  // Informs by the Serial when everything is ready-to-go
  Serial.println("READY!");
}

void loop() {
  // Waits until BLE Serial (My Serial object) becomes available
  while(BTSerial.available()){
    // Writes all incoming data from BLE to Arduino Serial
    Serial.write(BTSerial.read());
  }
}
