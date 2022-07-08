/*
 * ======================================
 *    SENSOR TEGANGAN / 220VAC ZMPT101B
 * ======================================
 */

float bacategangan(){
double sensorValue1 = 0;
double sensorValue2 = 0;
int crosscount = 0;
int climb_flag = 0;
int val[100];
int max_v = 0;
double VmaxD = 0;
double VeffD = 0;
double Veff = 0;


  for ( int i = 0; i < 100; i++ ) {
    sensorValue1 = analogRead(A0);
    //if (analogRead(A0) > 511) {
    if (analogRead(A0) > 480) {
      val[i] = sensorValue1;
    }
    else {
      val[i] = 0;
    }
    delay(1);
  }

  max_v = 0;

  for ( int i = 0; i < 100; i++ )
  {
    if ( val[i] > max_v )
    {
      max_v = val[i];
    }
    val[i] = 0;
  }
  if (max_v != 0) {


    VmaxD = max_v;
    //Serial.print("VmaxD: ");Serial.println(VmaxD);
    VeffD = VmaxD / sqrt(2);
    //Serial.print("VeffD: ");Serial.println(VeffD);
    Veff = (((VeffD - 420.76) / -90.24) * -210.2) + 210.2;
  }
  else {
    Veff = 0;
  }
  return Veff;

}


void testzmpt() {
  Serial.print("Analog Data : ");
  Serial.println(analogRead(A0));
  delay(100); 
}


/*
 * =============================
 *        DHT 11 SENSOR
 * =============================
 */


void GetSuhuKelembaban(){
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
    suhu = event.temperature;
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
    humidity = event.relative_humidity;
  }
}
