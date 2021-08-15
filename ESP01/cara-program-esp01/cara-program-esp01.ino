void setup() {
  // put your setup code here, to run once:
   Serial.begin(15200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Halo.. ini pesan dikirim dari ESP01 8266");
  delay(500);

}
