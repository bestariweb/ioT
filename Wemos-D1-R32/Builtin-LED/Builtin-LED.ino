void setup() {
  // GPIO2 sebagai output.
  pinMode(2, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(2, HIGH);   // Nyalakan LED 
  delay(100);              // Tunggu 100 milidetik
  digitalWrite(2, LOW);    // Matikan LED 
  delay(50);              // Tunggu 50 mili detik
  x   // Nyalakan LED 
  delay(100);              // Tunggu 100 milidetik
  digitalWrite(2, LOW);    // Matikan LED 
  delay(50);              // Tunggu 50 mili detik
  digitalWrite(2, HIGH);   // Nyalakan LED 
  delay(100);              // Tunggu 100 milidetik
  digitalWrite(2, LOW);    // Matikan LED 
  delay(1000);              // Tunggu 1 detik
}
