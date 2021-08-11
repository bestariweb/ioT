void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT); //Set GPIO2 (onboard LED Nodemcu sebagai output)
}

void loop() {
  // put your main code here, to run repeatedly:

  
  digitalWrite(2, LOW);   //Nyalakan LED
  delay(50);             // tunggu 50 ms
  digitalWrite(2, HIGH);   //Matikan LED
  delay(50);             // tunggu 50 ms
  digitalWrite(2, LOW);   //Nyalakan LED
  delay(50);             // tunggu 50 ms
  digitalWrite(2, HIGH);   //Matikan LED
  delay(50);             // tunggu 50 ms
  digitalWrite(2, LOW);   //Nyalakan LED
  delay(50);             // tunggu 50 ms
  digitalWrite(2, HIGH);   //Matikan LED
  delay(50);             // tunggu 50 ms
  digitalWrite(2, HIGH);  //Matikan LED
  delay(2000);             // Tunggu 1000ms
}
