void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // Nyalakan LED 
  delay(1000);                        // Tunggu selama 500 mili detik
  digitalWrite(LED_BUILTIN, LOW);    // Matikan LED
  delay(1000);                       // Tunggu selama 1 detik
}
