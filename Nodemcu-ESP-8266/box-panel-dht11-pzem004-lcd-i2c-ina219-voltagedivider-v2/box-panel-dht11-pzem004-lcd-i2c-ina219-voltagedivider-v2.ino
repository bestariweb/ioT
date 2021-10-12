#include "ThingsBoard.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <PZEM004Tv30.h>
#include <SoftwareSerial.h>
#include <DHT.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_INA219.h>

#include <ESP8266WiFi.h>
#include <Wire.h>  // This library is already built in to the Arduino IDE
#include <LiquidCrystal_I2C.h> //This library you can add via Include Library > Manage Library > 

#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>


#define BLYNK_PRINT serial
//#define PakaiThingsboard false
#define TOKEN               "g3quguWOlMxAxTdORlM9"
#define THINGSBOARD_SERVER  "43.252.237.40"

Adafruit_INA219 ina219;
// Initialize ThingsBoard client
WiFiClient espClient;
const bool PakaiThingsboard=false;
// Initialize ThingsBoard instance
//if (PakaiThingsboard){
  ThingsBoard tb(espClient);
//}
// the Wifi radio's status
int status = WL_IDLE_STATUS;

BlynkTimer timer;

PZEM004Tv30 pzem(14, 12);
//PZEM004Tv30 plts(2, 13);
float harga_KWh = 1.467; //harga per KWh
const long utcOffsetInSeconds = 25200;

char daysOfTheWeek[7][12] = {"Min", "Sen", "Sel", "Rab", "Kam", "Jum", "Sab"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", utcOffsetInSeconds);

int hh, mm, ss;
// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;
String YYYY,MM,DD;
    
LiquidCrystal_I2C lcd(0x27, 20, 4);//Tampilkan hasil di LCD di alamat 0x27. Alamat ini didapat dari sketch i2c scan ya.. cek tutorial terkait LCD i2c
// kREgWTG_WuSHEDMt6QS1UYc35-yOZuYW
const char* Blynkauth = "58lGq5i4anHB02rj5SuoTVqGCSegozYt";
//const char* ssid = "NodeMCU";// ssid nodemcu as accesspoint
//const char* password = "T101950617";// put your wifi password nodemcu
const char* ssid = "Bestariwebhost.com";
const char* password="05102011HuM4Y";
const char* BlynkServer = "blynk.bestariwebhost.com";
      float current_mA = 0;
      float loadvoltage = 0;
      float power_mW = 0;
      float ArusSebenarnya = 0;
String MainPower = "PLN";

String eblauth;
String eblserver;
String etbtoken;
String etbserver;

//BlynkTimer timer;


int i = 0;
int statusCode;
//const char* ssid = "text";
//const char* passphrase = "text";
String st;
String content;
String incomingByte;

#define DHTPIN D4     
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);


//Function Decalration
bool testWifi(void);
void launchWeb(void);
void setupAP(void);
void CekMainPower();
void teganganaki();
void CekArusAki();    
uint32_t currentFrequency;

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  while (!Serial) {
      // will pause Zero, Leonardo, etc until serial console opens
      delay(1);
  }
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
  
  //=========== Tambahan web server
  lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight
  Serial.println("Disconnecting previously connected WiFi");
  lcd.setCursor(0,0);lcd.print("Disconect from  ");
  lcd.setCursor(0,1);lcd.print("prev WiFi...    ");
  WiFi.disconnect();
  EEPROM.begin(512); //Initialasing EEPROM
  delay(10);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println();
  Serial.println();
  lcd.setCursor(0,0);lcd.print("STARTING UP     ");
  lcd.setCursor(0,1);lcd.print("READING EEPROM  ");
  Serial.println("Startup");
 
  //---------------------------------------- Read EEPROM for SSID and pass
  Serial.println("Reading EEPROM ssid");
 
  String esid;
  for (int i = 0; i < 32; ++i)
  { 
    if ((EEPROM.read(i) > 0) && (EEPROM.read(i) < 255)){
    esid += char(EEPROM.read(i));
    }
  }
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(esid);
  Serial.println("Reading EEPROM pass");
 
  String epass = "";
  for (int i = 32; i < 64; ++i)
  {
    if ((EEPROM.read(i) > 0) && (EEPROM.read(i) < 255)){
    epass += char(EEPROM.read(i));
    }
  }
  Serial.print("PASS: ");
  Serial.println(epass);

  eblauth = "";
  for (int i = 64; i < 96; ++i)
  {
    if ((EEPROM.read(i) > 0) && (EEPROM.read(i) < 255)){
    eblauth += char(EEPROM.read(i));
    }
  }
  Serial.print("Blynk Auth: ");
  Serial.println(eblauth);

  eblserver = "";
  for (int i = 96; i < 128; ++i)
  {
    if ((EEPROM.read(i) > 0) && (EEPROM.read(i) < 255)){
    eblserver += char(EEPROM.read(i));
    }
  }
  Serial.print("Blynk Server: ");
  Serial.println(eblserver);

  etbtoken = "";
  for (int i = 128; i < 160; ++i)
  {
    if ((EEPROM.read(i) > 0) && (EEPROM.read(i) < 255)){
    etbtoken += char(EEPROM.read(i));
    }
  }
  Serial.print("Thingsboard Token: ");
  Serial.println(etbtoken);

  etbserver = "";
  for (int i = 160; i < 192; ++i)
  {
    if ((EEPROM.read(i) > 0) && (EEPROM.read(i) < 255)){
    etbserver += char(EEPROM.read(i));
    }
  }
  Serial.print("Thingsboard Server: ");
  Serial.println(etbserver);
 
  lcd.setCursor(0,0);lcd.print("CONNECTING TO     ");
  lcd.setCursor(0,1);lcd.print("INTERNET VIA WIFI ");
  WiFi.begin(esid.c_str(), epass.c_str());
  if (testWifi())
  {
    Serial.println("Succesfully Connected!!!");
    lcd.setCursor(0,0);lcd.print("SUCCESSFULLY    ");
    lcd.setCursor(0,1);lcd.print("CONNECTED.....  ");
    return;
  }
  else
  {
    Serial.println("Turning the HotSpot On");
    lcd.setCursor(0,0);lcd.print("FAILED! STARTING");
    lcd.setCursor(0,1);lcd.print("HOTSPOT.........");
    delay(1000);
    launchWeb();
    setupAP();// Setup HotSpot
  }
 
  Serial.println();
  Serial.println("Waiting.");
  lcd.setCursor(0,0);lcd.print("SSID: PanelBOX ");
  lcd.setCursor(0,1);lcd.print("Tanpa Password ");
  i= 0;
  while ((WiFi.status() != WL_CONNECTED))
  {
    Serial.print(".");
    ++i;
    if (i<15){
    lcd.setCursor(0,0);lcd.print("SSID: PanelBOX  ");
    lcd.setCursor(0,1);lcd.print("Tanpa Password  ");
    } else
    {
    lcd.setCursor(0,0);lcd.print("Buka web http://");
    lcd.setCursor(0,1);lcd.print("192.168.4.1     ");
    }
    if (i>30){
      i = 0;
    }
    delay(100);
    server.handleClient();
  }
  Serial.println("Connecting to Blynk..");
  Blynk.config(eblauth.c_str(),eblserver.c_str(),8080);
  Blynk.connect();

  //=============== end of tambahan
  
    timeClient.begin();
    ina219.begin();
    pinMode(D4,OUTPUT); //Power ON OFF Inverter
    digitalWrite(D4,HIGH); // Set to nonactive (NC Connected)
    pinMode(D8,OUTPUT); //Switch Mode Main Power (ON OFF PLN)
    pinMode(9,INPUT);
    pinMode(10,INPUT);
    //Serial.println();
    //Serial.print("DHT11 begin..");
    dht.begin();
    //Serial.println("done");
    //Serial.print("Initialing LCD... ");
    
    //Serial.println("done");
 
    //WiFi.softAP(ssid, password);//bertindak sebagai access point dengan nama wifi dan pass diatas
    // WiFi.begin(ssid, password);
    //Serial.print("Connecting to Blynk Server via Wifi.. MAC: ");
    //Serial.println(WiFi.macAddress());
    
    //Pakai ini untuk koneksi wifi sekaligus blynk
    //Blynk.begin(auth,ssid,password,serverku,8080);

    //Pakai ini jika wifi sudah terkoneksi
    //Blynk.config(auth,serverku,8080);
    //Blynk.connect();
    
    //Serial.println("done");
    IPAddress apip = WiFi.localIP(); //Alamat IP as client
    //Serial.print("Wifi Connected with IP:");
    //Serial.println(apip); 
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("CONNECTED");
    Blynk.virtualWrite(V1,"CONNECTED");
    lcd.setCursor(0,1);
    lcd.print(apip);
    //Blynk.virtualWrite(V2,WiFi.localIP().toString());
    
    }


//Turn ON/OFF LCD Backlight on V5
BLYNK_WRITE(V5)
  {   
  if (param.asInt()) {
    lcd.noBacklight();
  }

  else {
    lcd.backlight();
  }
  }

BLYNK_WRITE(V29)
  {   
  if (param.asInt()) {
//    Blynk.virtualWrite(V30,energy);
    pzem.resetEnergy();
  }

  }

float Vaki = 0;

void loop() {
  //Blynk.run();  
  CekMainPower();
  if (Blynk.connected()) {  
  Blynk.run();  
  }
  
  else {
    //Serial.println("Reconnect to Blynk Server...");
    lcd.clear();lcd.setCursor(0,0);
    lcd.print("RECONNECT TO");
    lcd.setCursor(0,1);
    lcd.print("BLYNK SERVER..");
    //Blynk.begin(auth,ssid,password,serverku,8080);  
    //Pakai ini jika wifi sudah terkoneksi
    
    Blynk.config(eblauth.c_str(),eblserver.c_str(),8080);
    Blynk.connect();
    //Serial.println("done");
    lcd.clear();lcd.setCursor(0,0);
    lcd.print("BLYNK SERVER");
    lcd.setCursor(0,1);
    lcd.print("CONNECTED..");
    delay(1000);
  }
if (PakaiThingsboard)
{
if (!tb.connected()) {
    // Connect to the ThingsBoard
    //Serial.print("Connecting to: ");
    //Serial.print(THINGSBOARD_SERVER);
    //Serial.print(" with token ");
    //Serial.println(TOKEN);
    lcd.clear();lcd.setCursor(0,0);
    lcd.print("RECONNECT TO");
    lcd.setCursor(0,1);
    lcd.print("THINGSBOARD SERVER");
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      //Serial.println("Failed to connect");
      lcd.clear();lcd.setCursor(0,0);
    lcd.print("THINGSBOARD SERVER");
    lcd.setCursor(0,1);
    lcd.print("FAILED TO CONNECT");
      return;
    }
  }
}
  
 timer.run();

 // Update Jam
  timeClient.update();
  hh = timeClient.getHours();
  mm = timeClient.getMinutes();
  ss = timeClient.getSeconds();

  formattedDate = timeClient.getFormattedDate();
  //Serial.println(formattedDate);

  // Extract date
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);
  //Serial.print("DATE: ");
  //Serial.println(dayStamp);
  YYYY = formattedDate.substring(0, 4);
  MM = formattedDate.substring(7, 5);
  DD = formattedDate.substring(10, 8);
 
    //Serial.print("Custom PZEM Address:");
    //Serial.println(pzem.readAddress(), HEX);
    // Read the data from the sensor
    float voltage = pzem.voltage();
    float current = pzem.current();
    float power = pzem.power();
    float energy = pzem.energy();
    float frequency = pzem.frequency();
    float pf = pzem.pf();

    //Baca data dari sendor pzem ke 2 (PLTS)
    /*
    float voltage2 = plts.voltage();
    float current2 = plts.current();
    float power2 = plts.power();
    float energy2 = plts.energy();
    float frequency2 = plts.frequency();
    float pf2 = plts.pf();
    */

    if (isnan(voltage) ) {
    voltage = 0;
    }
    if (isnan(current) ) {
      current = 0;
    }
    if (isnan(power) ) {
      power = 0;
    }
    if (isnan(energy) ) {
      energy = 0;
    }
    if (isnan(frequency) ) {
      frequency = 0;
    }
    if (isnan(pf) ) {
      pf = 0;
    }
/*
    if (isnan(voltage2) ) {
    voltage2 = 0;
    }
    if (isnan(current2) ) {
      current2 = 0;
    }
    if (isnan(power2) ) {
      power2 = 0;
    }
    if (isnan(energy2) ) {
      energy2 = 0;
    }
    if (isnan(frequency2) ) {
      frequency2 = 0;
    }
    if (isnan(pf2) ) {
      pf2 = 0;
    }
  */  
    float totalHarga = energy * harga_KWh;

        // Print the values to the Serial console
        //Serial.print("Voltage: ");      Serial.print(voltage);      Serial.println("V");
        //Serial.print("Voltage PLTS: ");      Serial.print(voltage2);      Serial.println("V");
        Blynk.virtualWrite(V12,voltage);
        //Blynk.virtualWrite(V19,voltage2);
        if (PakaiThingsboard) {
        tb.sendTelemetryFloat("voltage", voltage);
        //tb.sendTelemetryFloat("voltagePLTS", voltage2);
        }
        lcd.setCursor(0,0); lcd.print(energy);lcd.print("kWH  ");  lcd.setCursor(9,0);lcd.print((int)voltage);lcd.print("V    ");
        //Serial.print("Current: ");      Serial.print(current);      Serial.println("A");
        //Serial.print("Current PLTS: ");      Serial.print(current2);      Serial.println("A");
        Blynk.virtualWrite(V13,current);
        //Blynk.virtualWrite(V20,current2);
        if (PakaiThingsboard) {
        tb.sendTelemetryFloat("current", current);
        //tb.sendTelemetryFloat("currentPLTS", current2);
        }
        lcd.setCursor(0,1);  lcd.print(current);lcd.print("A    ");
        //Serial.print("Power: ");        Serial.print(power);        Serial.println("W  ");
        Blynk.virtualWrite(V14,power);
        //Blynk.virtualWrite(V21,power2);
        if (PakaiThingsboard) {
        tb.sendTelemetryFloat("power", power);
        //tb.sendTelemetryFloat("powerPLTS", power2);
        }
        lcd.setCursor(9,1);        lcd.print(power);lcd.print("W     ");
        //Serial.print("Energy: ");       Serial.print(energy,3);     Serial.println("kWh");
        if (PakaiThingsboard) {
        tb.sendTelemetryFloat("energy", energy);
        //tb.sendTelemetryFloat("energyPLTS", energy2);
        }
        Blynk.virtualWrite(V15,energy);
        //Blynk.virtualWrite(V22,energy2);
        //Serial.print("Frequency: ");    Serial.print(frequency, 1); Serial.println("Hz   ");
        Blynk.virtualWrite(V16,frequency);
        //Blynk.virtualWrite(V23,frequency2);
        if (PakaiThingsboard) {
        tb.sendTelemetryInt("frequency", frequency);
        //tb.sendTelemetryInt("frequencyPLTS", frequency2);
        //Serial.print("PF: ");           Serial.println(pf);
        tb.sendTelemetryFloat("pf", pf);
        //tb.sendTelemetryFloat("pfPLTS", pf2);
        }
        Blynk.virtualWrite(V17,pf);
        //Blynk.virtualWrite(V24,pf2);
        Blynk.virtualWrite(V18,totalHarga);
        
        // Daftar Virtual Pin BLYNK
        /* V12  Voltage PLN
         * V13  Arus PLN
         * V14  power PLN
         * V15  energy PLN
         * V16  frek PLN
         * V17  pf PLN
         * V18  Hrga kwH (Cost)
         * V19  Voltage PLTS
         * V20  Arus PLTS
         * V21  Power PLTS
         * V22  Energy PLTS
         * V23  frek PLTS
         * V24  pf PLTS
         */
        if ((mm % 5)==0){
          lcd.init();   // initializing the LCD
          lcd.backlight(); // Enable or Turn On the backlight
        }
        if ((hh==0) && (mm==0) && (energy > 0.1)){
          Blynk.virtualWrite(V30,energy);
          //Blynk.virtualWrite(V31,energy2);
          if (PakaiThingsboard) {
          tb.sendTelemetryFloat("energyharian", energy);
          //tb.sendTelemetryFloat("energyharianPLTS", energy2);
          }
          pzem.resetEnergy();
          //plts.resetEnergy();
        }
      teganganaki();
      CekArusAki();
      //Serial.print("V Aki: ");Serial.println(Vaki);
      delay(1500);
      lcd.clear();
      lcd.setCursor(0,0); 
      lcd.print("I Aki: ");lcd.print(ArusSebenarnya);lcd.print("A ");
      Blynk.virtualWrite(V20,ArusSebenarnya);
      lcd.setCursor(0,1); 
      lcd.print("V Aki: ");lcd.print(Vaki);lcd.print("V  ");
      Blynk.virtualWrite(V19,Vaki);
      delay(1500);
      float h = dht.readHumidity();
      // Read temperature as Celsius (the default)
      float t = dht.readTemperature();
      if (isnan(h) || isnan(t) ) {
        Serial.println("Failed to read from DHT sensor!");
        //lcd.setCursor(0,0);
        //lcd.print("DHT: X");
        //return;
      }
      Serial.print("Humidity: ");
      Serial.print(h);
      Serial.print(" %\t");
      Serial.print("Temperature: ");
      Serial.println(t);
      Blynk.virtualWrite(V10,t);
      if (PakaiThingsboard) {
      tb.sendTelemetryInt("temperature", t);
      tb.sendTelemetryInt("humidity", h);
      }
      Blynk.virtualWrite(V11,h);
      
      lcd.clear();
      //lcd.setCursor(0,0);
      //lcd.print("DHT:OK");
      //lcd.setCursor(8,0);
      //lcd.print("PZEM:");
      lcd.setCursor(0,0);
      lcd.print(daysOfTheWeek[timeClient.getDay()]);
      lcd.print(",");
      lcd.print(DD);
      lcd.print("/");
      lcd.print(MM);
      lcd.print(" ");
      lcd.print(hh);
      lcd.print(":");
      lcd.print(mm);
      lcd.print("       ");
      //lcd.setCursor(7,0);lcd.print("Jam ");lcd.print(hh);lcd.print(":");lcd.print(mm);
      lcd.setCursor(0,1);lcd.print("H:");
      lcd.print((int)h);lcd.print("%");
      lcd.setCursor(7,1);
      lcd.print("Suhu ");
      lcd.print((int)t);
      lcd.print("\xDF" "C");
      delay(2000);
    }


 
//-------- Fuctions used for WiFi credentials saving and connecting to it which you do not need to change 
bool testWifi(void)
{
  int c = 0;
  Serial.println("Waiting for Wifi to connect");
  while ( c < 20 ) {
    if (WiFi.status() == WL_CONNECTED)
    {
      return true;
    }
    delay(500);
    Serial.print("*");
    c++;
  }
  Serial.println("");
  Serial.println("Connect timed out, opening AP");
  return false;
}
 
void launchWeb()
{
  Serial.println("");
  if (WiFi.status() == WL_CONNECTED)
    Serial.println("WiFi connected");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  createWebServer();
  // Start the server
  server.begin();
  Serial.println("Server started");
}
 
void setupAP(void)
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");
  st = "<form method='get' action='setting'>";
  st += "<p><label>SSID: </label>";
  st += "<select name=\"ssid\" id=\"ssid\">";
  for (int i = 0; i < n; ++i)
  {
    // Print SSID and RSSI for each network found
    
    st +="\t<option value=";
    st +="\"";
    st += WiFi.SSID(i);
    st += "\">";
    st += WiFi.SSID(i);
    st += " (Strength: ";
    st += WiFi.RSSI(i);
    st += ")";
    //st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
    st +="</option>\n";
  /*
    st += "<li>";
    st += WiFi.SSID(i);
    st += " (";
    st += WiFi.RSSI(i);
 
    st += ")";
    st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
    st += "</li>";
    */
  }
  st += "</select></p>";
  delay(100);
  WiFi.softAP("PanelBOX", "");
  Serial.println("softap");
  launchWeb();
  Serial.println("over");
}
 
void createWebServer()
{
 {
    server.on("/", []() {
 
      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
// AWAL ISI WEB ======================
      content = "<!DOCTYPE HTML>\r\n<html>\r\n";
      content += "<head>\r\n<title>Bestariweb ioT Connection Seting</title>\r\n";
      
      content += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";

      content += "<style>\r\n";
      content += "body{font-family:arial;}label{font-size:14px;width:140px;display:inline-block;}input{}h1{font-size:18px;border-bottom: solid 1px #aaa;padding: 5px 0;margin: 10px 0;text-transform: uppercase;min-width: 360px;}";
      content += "\r\n</style>\r\n</head>\r\n<body>\r\n";
      
      //content += ipStr;
      content += "<h1>Connection Setting</h1>";
      content += st;
      content += "<p><label>Password: </label><input type='password' name='pass' length=64></p>";
      content += "<p><label>Blynk Auth: </label><input type='text' name='blauth' length=64 value='"+eblauth+"'></p>";
      content += "<p><label>Blynk Server: </label><input type='text' name='blserver' length=64 value='"+eblserver+"'></p>";
      content += "<p><label>Thingsboard token: </label><input type='text' name='tbtoken' length=64 value='"+etbtoken+"'></p>";
      content += "<p><label>Thingsboard Server: </label><input type='text' name='tbserver' length=64 value='"+etbserver+"'></p>";
      content += "<input type='submit' value='SAVE &amp; REBOOT'></form>";
      content += "<form action=\"/scan\" method=\"POST\"><input type=\"submit\" value=\"SCAN Wifi\"></form>";
      content += "\r\n</body></html>";
// AKHIR ISI WEB ======================
      server.send(200, "text/html", content);
    });
    server.on("/scan", []() {
      //setupAP();
      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
 
      content = "<!DOCTYPE HTML>\r\n<html>go back";
      server.send(200, "text/html", content);
    });
 
    server.on("/setting", []() {
      String qsid = server.arg("ssid");
      String qpass = server.arg("pass");
      String qblauth = server.arg("blauth");
      String qblserver = server.arg("blserver");
      String qtbtoken = server.arg("tbtoken");
      String qtbserver = server.arg("tbserver");
      if (qsid.length() > 0) {
        Serial.println("clearing eeprom");
        for (int i = 0; i < 192; ++i) {
          EEPROM.write(i, 0);
        }
        Serial.println(qsid);
        Serial.println("");
        Serial.println(qpass);
        Serial.println("");
 
        Serial.println("writing eeprom ssid:");
        for (int i = 0; i < qsid.length(); ++i)
        {
          EEPROM.write(i, qsid[i]);
          Serial.print("Wrote: ");
          Serial.println(qsid[i]);
        }
        Serial.println("writing eeprom pass:");
        for (int i = 0; i < qpass.length(); ++i)
        {
          EEPROM.write(32 + i, qpass[i]);
          Serial.print("Wrote: ");
          Serial.println(qpass[i]);
        }

        Serial.println("writing Blynk Auth to eeprom :");
        for (int i = 0; i < qblauth.length(); ++i)
        {
          EEPROM.write(64 + i, qblauth[i]);
          Serial.print("Wrote: ");
          Serial.println(qblauth[i]);
        }

        Serial.println("writing Blynk Server to eeprom :");
        for (int i = 0; i < qblserver.length(); ++i)
        {
          EEPROM.write(96 + i, qblserver[i]);
          Serial.print("Wrote: ");
          Serial.println(qblserver[i]);
        }

        Serial.println("writing Thingsboard token to eeprom :");
        for (int i = 0; i < qtbtoken.length(); ++i)
        {
          EEPROM.write(128 + i, qtbtoken[i]);
          Serial.print("Wrote: ");
          Serial.println(qtbtoken[i]);
        }

        Serial.println("writing Thingsboard Server to eeprom :");
        for (int i = 0; i < qtbserver.length(); ++i)
        {
          EEPROM.write(160 + i, qtbserver[i]);
          Serial.print("Wrote: ");
          Serial.println(qtbserver[i]);
        }
        
        EEPROM.commit();
 
        content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
        statusCode = 200;
        ESP.reset();
      } else {
        content = "{\"Error\":\"404 not found\"}";
        statusCode = 404;
        Serial.println("Sending 404");
      }
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(statusCode, "application/json", content);
 
    });
 }
}

    void teganganaki(){
      float VakiA0 = analogRead(A0);
      Serial.print("A0: ");Serial.println(VakiA0);
      Vaki = (VakiA0 - 0) * 35 / 1023;
      if (Vaki < 11){
        digitalWrite(D4,LOW); // PowerOff Inverter
      } else {
        digitalWrite(D4,HIGH); // Power ON Inverter 
      }
    }

    void CekMainPower(){
      int pin9 = digitalRead(9);
      int pin10 = digitalRead(10);
      Serial.print("PIN 9: ");Serial.println(pin9);
      Serial.print("PIN 10: ");Serial.println(pin10);
      
    }

    void CekArusAki()
    {
      float totalArus = 0;;
      float samplingArus = 0;
      Serial.println("------------------------------");
      for (int i = 0; i <10 ; i++){
         samplingArus = ina219.getCurrent_mA();
         Serial.print("Sampling Arus: ");Serial.println(samplingArus);
         totalArus = totalArus + samplingArus;
         delay(100);
      }
      Serial.println("------------------------------");
      current_mA = totalArus / 10;
    power_mW = ina219.getPower_mW();
    loadvoltage = ina219.getBusVoltage_V() + (ina219.getShuntVoltage_mV() / 1000);
    Serial.print("Total Arus:  "); Serial.print(totalArus); Serial.println(" A");
    Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
    Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
    Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
    Serial.println("");
    ArusSebenarnya = current_mA/8;
    Serial.print("Arus Sebenarnya: ");Serial.print(ArusSebenarnya);Serial.println("Ampere");

    
    }
