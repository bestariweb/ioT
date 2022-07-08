void checkconnection(){
  if ((pernahconnect) && (tampilan < 2)){
    WiFi.disconnect();
    WiFi.mode(WIFI_AP_STA);
  if (ehssid.length() > 2){
    WiFi.softAPConfig(MyapIP, MyapIP, IPAddress(255, 255, 255, 0)); 
    WiFi.softAP(ehssid,ehpass);
    lcd.setCursor(0,0);lcd.print("CREATING HOTSPOT");
    lcd.setCursor(0,1);lcd.print(ehssid);
  } else {
    WiFi.softAPConfig(MyapIP, MyapIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(AP_SSID, AP_PASS);
    lcd.setCursor(0,0);lcd.print("CREATING HOTSPOT");
    lcd.setCursor(0,1);lcd.print(AP_SSID);
  }
  // Begin WiFi
  lcd.clear();
  if (essid.length() > 2){
    WiFi.begin(essid,epass);
    lcd.setCursor(0,0);lcd.print("CONNECTING TO WIFI");
    lcd.setCursor(0,1);lcd.print(essid);
    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress());
    Serial.print("Connecting to ");
    Serial.print(essid);
  } else {
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    lcd.setCursor(0,0);lcd.print("CONNECTING TO WIFI");
    lcd.setCursor(0,1);lcd.print(WIFI_SSID);
    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress());
    Serial.print("Connecting to ");
    Serial.print(WIFI_SSID);
  }
  // Connecting to WiFi...
  int i=0;
  while ((WiFi.status() != WL_CONNECTED) && (i<200))
  {
    delay(100);
    Serial.print(".");
    ++i;
  }
  lcd.clear();
  lcd.setCursor(0,0);lcd.print("Connecting to");
    Serial.print("Connecting to BLYNK");
    lcd.setCursor(0,1);lcd.print("BLYNK............");
    Blynk.config(eblauth.c_str(),eblserver.c_str(),8080);
    Blynk.connect();
    Serial.println(eblserver);
    delay(200);
  lcd.clear();
    lcd.setCursor(0,0);lcd.print("  NO INTERNET  ");
    lcd.setCursor(0,1);lcd.print("  CONNECTION   ");
    delay(1500);
    lcd.setCursor(0,0);lcd.print("HUBUNGKAN HP KE");
    lcd.setCursor(0,1);lcd.print(" WIFI ALAT INI ");
    delay(2000);
    if (ehssid.length() > 2){
      lcd.clear();
      lcd.setCursor(0,0);lcd.print("SSID: ");lcd.print(ehssid);
      lcd.setCursor(0,1);lcd.print("PASS: ");lcd.print(ehpass);
      delay(3000);
    } else {
      lcd.clear();
      lcd.setCursor(0,0);lcd.print("SSID: ");lcd.print(AP_SSID);
      lcd.setCursor(0,1);lcd.print("PASS: ");lcd.print(AP_PASS);
      delay(3000);
    }
    lcd.clear();
    lcd.setCursor(0,0);lcd.print("  BUKA CHROME  ");
    lcd.setCursor(0,1);lcd.print("HTTP://");lcd.print(ipstrap);
    delay(2000);
    
  } else if (tampilan ==1) {
    lcd.clear();
    lcd.setCursor(0,0);lcd.print("  NO INTERNET  ");
    lcd.setCursor(0,1);lcd.print("  CONNECTION   ");
    delay(1500);
    lcd.setCursor(0,0);lcd.print("HUBUNGKAN HP KE");
    lcd.setCursor(0,1);lcd.print(" WIFI ALAT INI ");
    delay(2000);
    if (ehssid.length() > 2){
      lcd.clear();
      lcd.setCursor(0,0);lcd.print("SSID: ");lcd.print(ehssid);
      lcd.setCursor(0,1);lcd.print("PASS: ");lcd.print(ehpass);
      delay(3000);
    } else {
      lcd.clear();
      lcd.setCursor(0,0);lcd.print("SSID: ");lcd.print(AP_SSID);
      lcd.setCursor(0,1);lcd.print("PASS: ");lcd.print(AP_PASS);
      delay(3000);
    }
    lcd.clear();
    lcd.setCursor(0,0);lcd.print("  BUKA CHROME  ");
    lcd.setCursor(0,1);lcd.print("HTTP://");lcd.print(ipstrap);
    delay(2000);
 }
    
  
}
