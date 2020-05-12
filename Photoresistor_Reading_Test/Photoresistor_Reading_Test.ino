void setup() {
  Serial.begin(9600);
}

void loop() {
  int val = analogRead(A0);
  Serial.print("VALUE: ");
  Serial.println(val);
  delay(500);
}
