#include <Wire.h>  // This library is already built in to the Arduino IDE
#include <LiquidCrystal_I2C.h> //This library you can add via Include Library > Manage Library > 

#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <SoftwareSerial.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ZMPT101B.h>

// Set WiFi credentials
#define WIFI_SSID "Bestariwebhost.id"
#define WIFI_PASS ""
 
// Set AP credentials
#define AP_SSID "bestariwebioT"
#define AP_PASS "8888888888"


#define DHTPIN 13
#define DHTTYPE    DHT11

LiquidCrystal_I2C lcd(0x27, 20, 4);
ZMPT101B voltageSensor(A0);
DHT_Unified dht(DHTPIN, DHTTYPE);
ESP8266WebServer server(80);

//eeprom variable
      String essid;
      String epass;
      String eblauth;
      String eblserver;
      String ehssid,ehpass;
bool webopened,pernahconnect;
const long utcOffsetInSeconds = 25200;
char NamaHari[7][4] = {"MNG", "SEN", "SEL", "RAB", "KAM", "JUM", "SAB"};
String NamaBulan[13] = {" ","JAN","FEB","MARET","APRIL","MEI","JUNI","JULI","AGUST","SEPT","OKT","NOV","DES"};
float VLN,VNG;
int awal,akhir;
uint32_t delayMS;
String content;
String incomingByte;
String NamaBaris;
String ipstrap,ipstrhs;
String st;
float suhu,humidity;
int tampilan,statusCode;

IPAddress    MyapIP(10, 8, 0, 1); 

void setup() {
  Serial.begin(115200);
  dht.begin();
  EEPROM.begin(512); //Initialasing EEPROM
  delay(10);
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
  
  lcd.init();
  lcd.backlight();
  lcd.clear();
  readeeprom();
  awal = millis();
  digitalWrite(12,1);voltageSensor.calibrate();
  voltageSensor.setSensitivity(1);
  //Konek ke WIFI
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
  while ((WiFi.status() != WL_CONNECTED) && (i<300))
  {
    delay(100);
    Serial.println(i);
    ++i;
  }
  if (i==300){
    Serial.println();
    Serial.println("CONNECTION FAILED");
    lcd.setCursor(0,2);lcd.print("CONNECTION FAILED     ");
    lcd.setCursor(0,3);lcd.print("NO INTERNET  ");
    pernahconnect = false;
  } else {
    Serial.println();
    Serial.println("CONNECTION SUCCESS");
    lcd.setCursor(0,2);lcd.print("CONNECTION SUCCESS ");
    lcd.setCursor(0,3);lcd.print("WIFI CONNECTED ");
    pernahconnect = true;
  }
  delay(1000);
  
  lcd.clear();
  lcd.setCursor(0,0);lcd.print("Connected!");
  Serial.print("IP address for network ");
  Serial.print(WIFI_SSID);
  Serial.print(" : ");
  Serial.println(WiFi.localIP());
  Serial.print("IP address for network ");
  if (ehssid.length() > 2){
    Serial.print(ehssid);
    lcd.setCursor(0,0);lcd.print("ACCESS POINT: ");
    lcd.setCursor(0,1);lcd.print(ehssid);
  }else{
    Serial.print(AP_SSID);
    lcd.setCursor(0,0);lcd.print("ACCESS POINT: ");
    lcd.setCursor(0,1);lcd.print(AP_SSID);
  }
  Serial.print(" : ");
  IPAddress ipap = WiFi.softAPIP();
  ipstrap = String(ipap[0]) + '.' + String(ipap[1]) + '.' + String(ipap[2]) + '.' + String(ipap[3]);
  IPAddress iphs = WiFi.localIP();
  ipstrhs = String(iphs[0]) + '.' + String(iphs[1]) + '.' + String(iphs[2]) + '.' + String(iphs[3]);
  Serial.println(WiFi.softAPIP());
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  if ((pernahconnect) && (eblauth.length()>10)) {

    lcd.clear();
    lcd.setCursor(0,0);lcd.print("Connecting to");
    Serial.print("Connecting to Blynk");
    lcd.setCursor(0,1);lcd.print("BLYNK............");
    Blynk.config(eblauth.c_str(),eblserver.c_str(),8080);
    Blynk.connect();
    Serial.println(eblserver);
    delay(100);
  }
  createWebServer();
  server.begin();
  Serial.println("Server dijalankan");
}

void loop() {
  server.handleClient();
  delay(50);
  if (Blynk.connected()) {  
    Blynk.run();  
    pernahconnect = true;
  } else {
    checkconnection();
  }
  if (webopened) {
    webopened = false;
  } else {
    akhir = millis();
    VLN = bacategangan();
  
    Serial.print("V L-N: ");Serial.println(VLN);
    Serial.print("V N-G: ");Serial.println(VNG);
    if ((akhir - awal) > 100000){
      awal = akhir;
      lcd.init();
      lcd.backlight();
      lcd.clear();
    }
    GetSuhuKelembaban();
    Blynk.virtualWrite(V0,VLN);
    Blynk.virtualWrite(V1,suhu);
    Blynk.virtualWrite(V2,humidity);
    tampilkan();
    delay(2000);
  }
}
