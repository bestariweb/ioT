/*
 * ===================================
 *    LVD 3Channel Rellay, System 48V
 * ===================================
 */

#include <PCF8574.h>
#include <Wire.h>  // This library is already built in to the Arduino IDE
#include <LiquidCrystal_I2C.h> //This library you can add via Include Library > Manage Library > 

#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266HTTPClient.h>


PCF8574 PCF(0x20);


// Set WiFi credentials
#define WIFI_SSID "Bestariwebhost.id"
#define WIFI_PASS ""
 
// Set AP credentials
#define AP_SSID "LVD00_3CHannel"
#define AP_PASS "8888888888"

const bool pakaiSerial = true;
int statusCode,tampilan;
String VBATT;
String content;
String incomingByte;
String NamaBaris;
String R1stat,R2stat,R3stat,R1STATE,R2STATE,R3STATE;
String st;
bool EmergencyRelayStop1 = false;
bool EmergencyRelayStop2 = false;
bool EmergencyRelayStop3 = false;
bool PAKAITIMER = false;
bool STATUSTIMERON = false;


//eeprom variable
      String essid;
      String epass;
      String eblauth;
      String eblserver;
      String elv1,ehv1,elv2,ehv2,elv3,ehv3;
      String ehssid,ehpass;
bool webopened;
float vbattfloat;

//WiFiServer server(80);
ESP8266WebServer server(80);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void createWebServer();
void relaystate();
String getVBatt(){
  int ao =0;
  int i=0;
  while (i<50){
    ao += analogRead(A0);
    delay(10);
    ++i;
  }
  vbattfloat = (ao/i) * GetVmax(4700,330000) / 1024;
  Serial.println(vbattfloat);
  return String(vbattfloat);
}

float GetVmax(int R1,int R2){
  return (R2 + R1) / R1;
}

void scanssid(){
  int n = WiFi.scanNetworks();
  if (n == 0){
    Serial.println("no networks found");
  }
  else
  {
    Serial.print(n);
    if (pakaiSerial) Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
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
  //st = "<form method='get' action='savesetting'>";
  st = "<p><label>Wifi SSID: </label>";
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
  }
  st += "</select></p>";
  delay(100);
}


void readeeprom(){
  essid="";
  for (int i = 0; i < 32; ++i)
  { 
    if ((EEPROM.read(i) > 0) && (EEPROM.read(i) < 255)){
    essid += char(EEPROM.read(i));
    }
  }
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(essid);
  Serial.println("Reading EEPROM pass");
  
  epass = "";
  for (int i = 32; i < 48; ++i)
  {
    if ((EEPROM.read(i) > 0) && (EEPROM.read(i) < 255)){
    epass += char(EEPROM.read(i));
    }
  }
  Serial.print("PASS: ");
  Serial.println(epass);

  eblauth = "";
  for (int i = 48; i < 80; ++i)
  {
    if ((EEPROM.read(i) > 0) && (EEPROM.read(i) < 255)){
    eblauth += char(EEPROM.read(i));
    }
  }
  Serial.print("BLYNK Auth: ");
  Serial.println(eblauth);

  eblserver = "";
  for (int i = 80; i < 112; ++i)
  {
    if ((EEPROM.read(i) > 0) && (EEPROM.read(i) < 255)){
    eblserver += char(EEPROM.read(i));
    }
  }
  Serial.print("BLYNK Server: ");
  Serial.println(eblserver);

  elv1 = "";
  for (int i = 112; i < 120; ++i)
  {
    if ((EEPROM.read(i) > 0) && (EEPROM.read(i) < 255)){
    elv1 += char(EEPROM.read(i));
    }
  }
  Serial.print("Relay1 disconnect: ");
  Serial.println(elv1);

  ehv1 = "";
  for (int i = 120; i < 128; ++i)
  {
    if ((EEPROM.read(i) > 0) && (EEPROM.read(i) < 255)){
    ehv1 += char(EEPROM.read(i));
    }
  }
  Serial.print("Relay1 Reconnect: ");
  Serial.println(ehv1);

  elv2 = "";
  for (int i = 128; i < 136; ++i)
  {
    if ((EEPROM.read(i) > 0) && (EEPROM.read(i) < 255)){
    elv2 += char(EEPROM.read(i));
    }
  }
  Serial.print("Relay2 disconnect: ");
  Serial.println(elv2);

  ehv2 = "";
  for (int i = 136; i < 144; ++i)
  {
    if ((EEPROM.read(i) > 0) && (EEPROM.read(i) < 255)){
    ehv2 += char(EEPROM.read(i));
    }
  }
  Serial.print("Relay2 Reconnect: ");
  Serial.println(ehv2);

  elv3 = "";
  for (int i = 144; i < 152; ++i)
  {
    if ((EEPROM.read(i) > 0) && (EEPROM.read(i) < 255)){
    elv3 += char(EEPROM.read(i));
    }
  }
  Serial.print("Relay3 Disconnect: ");
  Serial.println(elv3);

  ehv3 = "";
  for (int i = 152; i < 160; ++i)
  {
    if ((EEPROM.read(i) > 0) && (EEPROM.read(i) < 255)){
    ehv3 += char(EEPROM.read(i));
    }
  }
  Serial.print("Relay3 Reconnect: ");
  Serial.println(ehv3);

  ehssid = "";
  for (int i = 160; i < 192; ++i)
  {
    if ((EEPROM.read(i) > 0) && (EEPROM.read(i) < 255)){
    ehssid += char(EEPROM.read(i));
    }
  }
  Serial.print("Hotspot SSID: ");
  Serial.println(ehssid);

  ehpass = "";
  for (int i = 192; i < 224; ++i)
  {
    if ((EEPROM.read(i) > 0) && (EEPROM.read(i) < 255)){
    ehpass += char(EEPROM.read(i));
    }
  }
  Serial.print("Hotspot SSID: ");
  Serial.println(ehpass);

  
}

void Rellayoff(){
  R1stat="OFF";
  R2stat="OFF";
  R3stat="OFF";
  R1STATE = "LVLOW";
  R2STATE = "LVLOW";
  R3STATE = "LVLOW";
}


void displaylcd1602(){
  if (tampilan < 5){
              if (tampilan ==0){
                lcd.init();
                lcd.backlight();
                lcd.clear();
              }
              lcd.setCursor(0,0);lcd.print(vbattfloat);lcd.print("V    ");
              if (R1stat=="ON"){ 
                lcd.setCursor(10,0);lcd.print("R1 ON ");
              } else if (R1stat=="OFF"){
                lcd.setCursor(10,0);lcd.print("R1 OFF");
              }
        
              // Set Switch di Relay 2
              if (R2stat=="ON"){
                lcd.setCursor(0,1);lcd.print("R2 ON ");
              } else if (R2stat=="OFF"){
                lcd.setCursor(0,1);lcd.print("R2 OFF");
              }
        

              if (R3stat=="ON"){
                lcd.setCursor(10,1);lcd.print("R3 ON ");
              } else if (R3stat=="OFF"){
                lcd.setCursor(10,1);lcd.print("R3 OFF");
              }
              

      } else if ((tampilan >= 5) && (tampilan < 9)) {
        if (tampilan == 5){
          lcd.clear();
        }
        lcd.setCursor(0,0);lcd.print("RELAY 1");
        lcd.setCursor(0,1);lcd.print(elv1);lcd.print("V - ");lcd.print(ehv1);lcd.print("V   ");
      } else if ((tampilan >= 9) && (tampilan < 13)) {
        if (tampilan == 9){
          lcd.clear();
        }
        lcd.setCursor(0,0);lcd.print("RELAY 2");
        lcd.setCursor(0,1);lcd.print(elv2);lcd.print("V - ");lcd.print(ehv2);lcd.print("V   ");
      } else if ((tampilan >= 13) && (tampilan < 17)) {
        if (tampilan == 17){
          lcd.clear();
        }
        lcd.setCursor(0,0);lcd.print("RELAY 3");
        lcd.setCursor(0,1);lcd.print(elv3);lcd.print("V - ");lcd.print(ehv3);lcd.print("V   ");
      } else {
        tampilan = 0;
      }
      tampilan++;
      delay(20);
      
}

void setup() {
  // put your setup code here, to run once:
  tampilan = 0;

PCF.begin();
PCF.write8(0b00000000);


EEPROM.begin(512); //Initialasing EEPROM
delay(10);

// Setup serial port
  Serial.begin(115200);
  Serial.println();
  Rellayoff();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  readeeprom();
  // Begin Access Point
  WiFi.mode(WIFI_AP_STA);
  if (ehssid.length() > 2){
    WiFi.softAP(ehssid,ehpass);
    lcd.setCursor(0,0);lcd.print("CREATING HOTSPOT");
    lcd.setCursor(0,1);lcd.print(ehssid);
  } else {
    WiFi.softAP(AP_SSID, AP_PASS);
    lcd.setCursor(0,0);lcd.print("CREATING HOTSPOT");
    lcd.setCursor(0,1);lcd.print(AP_SSID);
  }
  // Begin WiFi
  lcd.clear();
  if (essid.length() > 2){
    WiFi.begin(essid,epass);
    lcd.setCursor(0,0);lcd.print("CONNECTING TO");
    lcd.setCursor(0,1);lcd.print(essid);
    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress());
    Serial.print("Connecting to ");
    Serial.print(essid);
  } else {
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    lcd.setCursor(0,0);lcd.print("CONNECTING TO");
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
 
  // Connected to WiFi
  Serial.println();
  Serial.println("Connected!");
  Serial.print("IP address for network ");
  Serial.print(WIFI_SSID);
  Serial.print(" : ");
  Serial.println(WiFi.localIP());
  Serial.print("IP address for network ");
  if (ehssid.length() > 2){
    Serial.print(ehssid);
  }else{
    Serial.print(AP_SSID);
  }
  Serial.print(" : ");
  Serial.println(WiFi.softAPIP());
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  //if (eblauth.length()>10){
    Serial.print("Connecting to Blynk ");
    Blynk.config(eblauth.c_str(),eblserver.c_str(),8080);
    Blynk.connect();
    Serial.println(eblserver);
    delay(100);
  //}
  createWebServer();
  server.begin();
  Serial.println("Server dijalankan");
  

}


void loop() {
  server.handleClient();
  delay(50);
  if (webopened) {
  webopened = false;
  } else {
    getVBatt();
    Blynk.virtualWrite(V1,vbattfloat);
    Serial.println("send data to blynk");
  }
  relaystate();
  displaylcd1602();
}


void createWebServer()
{
 {
    scanssid();
    server.on("/", []() {
 
      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
// AWAL ISI WEB ======================
      content = "<!DOCTYPE HTML>\r\n<html>\r\n";
      content += "<head>\r\n<title>Bestariweb ioT Connection Seting</title>\r\n";
      content += "<meta http-equiv=\"refresh\" content=\"0;url=/dashboard\">";
      content += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";

      content += "<style>\r\n";
      content += "\r\n</style>\r\n</head>\r\n<body></BODY></HTML>\r\n";
      
// AKHIR ISI WEB ======================
      server.send(200, "text/html", content);
    });

    server.on("/dashboard", []() {
      webopened = true;
      VBATT = getVBatt();
      Blynk.virtualWrite(V1,VBATT.toFloat());
      content = "<!DOCTYPE HTML>\r\n<html>\r\n";
      content += "<head>\r\n<title>Bestariweb ioT Dashboard</title>\r\n";
      content +="<meta http-equiv=\"refresh\" content=\"5;url=/dashboard\">";
      content += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
      content += "<style>\r\n";
      content += "html{background:#333;color:#fff;}body{font-family:arial;margin:0;min-height:800px;}form{margin:10px 10px 150px 10px;}label{font-size:14px;width:140px;display:inline-block;}input{}h1{font-size:18px;border-bottom: solid 1px #aaa;padding: 5px 0;margin: 10px 0;text-transform: uppercase;}";
      content += ".vleft{font-size:16px;padding: 8px;margin: 10px 0;text-transform: uppercase;width:45%;float:left;text-align:right;}.vright{font-size:16px;border: none;padding: 8px;margin: 10px 0;text-transform: uppercase;width:45%;float:left;background: #dbdb07;box-shadow: inset 2px 2px 2px rgba(0,0,0,0.5);border-radius: 6px;color: #333;max-width:150px}";
      content += ".tombol{width:90%;max-width:300px;clear:both;background:#aa0000;color:#ffffff;margin:10px auto;border-radius:10px;}.tombol a{color:#fff;text-align:center;display:block;padding:20px;text-decoration:none}";
      content += ".topmenu{width:95%;margin:10px auto;border-bottom:solid 1px #00a;height:29px;}.menuitem{border-radius:5px 5px 0 0;float:left;background:#006;color:#fff;padding:5px 8px;margin:0 3px 0 0;}";
      content += ".menuitem a{text-decoration:none}.topmenu > .active {background:#00a!important;box-shadow: 10px -1px 3px rgba(0,0,0,0.6);z-index: 1000;position: relative;}";
      content += "\r\n</style>\r\n";
      content += "</head><body><div class=\"topmenu\"><div class=\"menuitem active\">DASHBOARD</div><div class=\"menuitem\"><a href=\"/setting\">SETTING</a></div></div>";
      content += "<div class=\"vleft\">Tegangan Aki</div><div class=\"vright\">";
      content += VBATT;
      content += "</div><div style=\"clear:both\"></div>";
      if (R1stat=="ON"){
        content += "<div class=\"tombol\"><a href=\"/r1off\">Relay 1: Switch OFF</a></div>";
      } else {
        content += "<div class=\"tombol\"><a href=\"/r1on\">Relay 1: Switch ON</a></div>";
      }

      if (R2stat=="ON"){
        content += "<div class=\"tombol\"><a href=\"/r2off\">Relay 2: Switch OFF</a></div>";
      } else {
        content += "<div class=\"tombol\"><a href=\"/r2on\">Relay 2: Switch ON</a></div>";
      }

      if (R3stat=="ON"){
        content += "<div class=\"tombol\"><a href=\"/r3off\">Relay 3: Switch OFF</a></div>";
      } else {
        content += "<div class=\"tombol\"><a href=\"/r3on\">Relay 3: Switch ON</a></div>";
      }
      content += "<div style=\"position:fixed;bottom:0;display:block;font-size:10px;background:#ff6000;color:#ffffff;width:100%;padding:8px 0;text-align:center;\">";
      content += "MAC:";
      content += WiFi.macAddress();
      content += "<br>Copyright&copy;2021 Bestariweb</div>";
      content += "</body></html>";
      server.send(200, "text/html", content);
    });

    server.on("/setting", []() {
      VBATT = getVBatt();
      content = "<!DOCTYPE HTML>\r\n<html>\r\n";
      content += "<head>\r\n<title>Bestariweb ioT Dashboard</title>\r\n";
      content += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
      content += "<style>\r\n";
      content += "html{background:#333;color:#fff;}body{font-family:arial;margin:0;min-height:800px;}form{margin:10px 10px 150px 10px;}label{font-size:14px;width:140px;float:left;padding:8px;}input[type=password],input[type=number],input[type=text]{font-size:16px;border: none;padding: 8px;width:45%;float:left;background: #dbdb07;box-shadow: inset 2px 2px 2px rgba(0,0,0,0.5);border-radius: 6px;color: #333;max-width:150px}h1{font-size:18px;border-bottom: solid 1px #aaa;padding: 5px 0;margin: 10px 0;text-transform: uppercase;}";
      content += "input[type=number]::-webkit-inner-spin-button {opacity: 1;}p{float:left;width:100%}";
      content += ".vleft{font-size:16px;padding: 8px;margin: 10px 0;text-transform: uppercase;width:45%;float:left;text-align:right;}.vright{font-size:16px;border: none;padding: 8px;margin: 10px 0;text-transform: uppercase;width:45%;float:left;background: #dbdb07;box-shadow: inset 2px 2px 2px rgba(0,0,0,0.5);border-radius: 6px;color: #333;max-width:150px}";
      content += ".tombol{width:90%;max-width:300px;clear:both;background:#aa0000;color:#ffffff;margin:10px auto;border-radius:10px;}.tombol a{color:#fff;text-align:center;display:block;padding:20px;text-decoration:none}";
      content += ".topmenu{width:95%;margin:10px auto;border-bottom:solid 1px #00a;height:29px;}.menuitem{border-radius:5px 5px 0 0;float:left;background:#006;color:#fff;padding:5px 8px;margin:0 3px 0 0;}";
      content += ".menuitem a{text-decoration:none}.topmenu > .active {background:#00a!important;box-shadow: 10px -1px 3px rgba(0,0,0,0.6);z-index: 1000;position: relative;}";
      content += "\r\n</style>\r\n";
      content += "</head><body><div class=\"topmenu\"><div class=\"menuitem \"><a href=\"/dashboard\">DASHBOARD</a></div><div class=\"menuitem active\">SETTING</div></div>";
      content += "<form action=\"/savesetting\">";
      content += "<h1>Access Point Setting</h1>"+st;
      content += "<p><label>Wifi Password: </label><input type='password' name='pass' length=64></p>";
      content += "<p><label>Blynk Server: </label><input type='text' value='"+eblserver+"' name='blserver' length=64></p>";
      content += "<p><label>Blynk Auth: </label><input type='text' value='"+eblauth+"' name='blauth' length=64></p>";
      content += "<h1>Voltage Setting</h1>";
      content += "<p><label>Relay1 Disconnect: </label><input type='number' step='0.1' value='"+elv1+"' name='lv1' length=64></p>";
      content += "<p><label>Relay1 Reconnect: </label><input type='number' step='0.1' name='hv1' value='"+ehv1+"' length=64></p>";
      content += "<p><label>Relay2 Disconnect: </label><input type='number' step='0.1' name='lv2' value='"+elv2+"' length=64></p>";
      content += "<p><label>Relay2 Reconnect: </label><input type='number' step='0.1' name='hv2' value='"+ehv2+"' length=64></p>";
      content += "<p><label>Relay3 Disconnect: </label><input type='number' step='0.1' name='lv3' value='"+elv3+"' length=64></p>";
      content += "<p><label>Relay3 Reconnect: </label><input type='number' step='0.1' name='hv3' value='"+ehv3+"' length=64></p>";
      content += "<h1>Hotspot Setting</h1>";
      content += "<p><label>SSID: </label><input type='text' name='hssid' value='"+ehssid+"' length=64></p>";
      content += "<p><label>Password: </label><input type='password' name='hpass' value='"+ehpass+"' length=64></p>";
      content += "<input type='submit' value='SAVE &amp; REBOOT'></form>";
      
      content += "<div style=\"position:fixed;bottom:0;display:block;font-size:10px;background:#ff6000;color:#ffffff;width:100%;padding:8px 0;text-align:center;\">";
      content += "MAC:";
      content += WiFi.macAddress();
      content += "<br>Copyright&copy;2021 Bestariweb</div>";
      content += "</body></html>";
      server.send(200, "text/html", content);
    });

 server.on("/savesetting", []() {
      String qssid = server.arg("ssid");
      String qpass = server.arg("pass");
      String qblauth = server.arg("blauth");
      String qblserver = server.arg("blserver");
      String qlv1 = server.arg("lv1");
      String qhv1 = server.arg("hv1");
      String qlv2 = server.arg("lv2");
      String qhv2 = server.arg("hv2");
      String qlv3 = server.arg("lv3");
      String qhv3 = server.arg("hv3");
      String qhssid = server.arg("hssid");
      String qhpass = server.arg("hpass");
      if (qssid.length() > 0) {
        Serial.println("clearing eeprom");
        for (int i = 0; i < 256; ++i) {
          EEPROM.write(i, 0);
        }

        //ssid maks 32digit
        Serial.println("writing eeprom Wifi ssid:");
        for (int i = 0; i < qssid.length(); ++i)
        {
          EEPROM.write(i, qssid[i]);
          Serial.print("Wrote: ");
          Serial.println(qssid[i]);
        }
        
        //pass maks 16 digit
        Serial.println("writing eeprom Wifi Password:");
        for (int i = 0; i < qpass.length(); ++i)
        {
          EEPROM.write(32+i, qpass[i]); //ssid maks 32digit
          Serial.print("Wrote: ");
          Serial.println(qpass[i]);
        }

        //blauth maks 32 digit
        Serial.println("writing eeprom Blynk Auth:");
        for (int i = 0; i < qblauth.length(); ++i)
        {
          EEPROM.write(48+i, qblauth[i]); //32+16 = 48
          Serial.print("Wrote: ");
          Serial.println(qblauth[i]);
        }

        //qblserver maks 32 digit
        Serial.println("writing eeprom Blynk Server:");
        for (int i = 0; i < qblserver.length(); ++i)
        {
          EEPROM.write(80+i, qblserver[i]); //32+16+32 = 80
          Serial.print("Wrote: ");
          Serial.println(qblserver[i]);
        }

        //qlv1 maks 8 digit
        Serial.println("writing eeprom lv1:");
        for (int i = 0; i < qlv1.length(); ++i)
        {
          EEPROM.write(112+i, qlv1[i]); //32+16+32+32 = 112
          Serial.print("Wrote: ");
          Serial.println(qlv1[i]);
        }

        //qhv1 maks 8 digit
        Serial.println("writing eeprom hv1:");
        for (int i = 0; i < qhv1.length(); ++i)
        {
          EEPROM.write(120+i, qhv1[i]); //32+16+32+32+8 = 120
          Serial.print("Wrote: ");
          Serial.println(qhv1[i]);
        }

        //qlv2 maks 8 digit
        Serial.println("writing eeprom lv2:");
        for (int i = 0; i < qlv2.length(); ++i)
        {
          EEPROM.write(128+i, qlv2[i]); //120+8 = 128
          Serial.print("Wrote: ");
          Serial.println(qlv2[i]);
        }

        //qhv2 maks 8 digit
        Serial.println("writing eeprom hv2:");
        for (int i = 0; i < qhv2.length(); ++i)
        {
          EEPROM.write(136+i, qhv2[i]); //128+8 = 136
          Serial.print("Wrote: ");
          Serial.println(qhv2[i]);
        }

        //qlv3 maks 8 digit
        Serial.println("writing eeprom lv2:");
        for (int i = 0; i < qlv3.length(); ++i)
        {
          EEPROM.write(144+i, qlv3[i]); //136+8 = 144
          Serial.print("Wrote: ");
          Serial.println(qlv3[i]);
        }

        //qhv3 maks 8 digit
        Serial.println("writing eeprom hv2:");
        for (int i = 0; i < qhv3.length(); ++i)
        {
          EEPROM.write(152+i, qhv3[i]); //144+8 = 152
          Serial.print("Wrote: ");
          Serial.println(qhv3[i]);
        }

        //ssid maks 32digit
        Serial.println("writing eeprom Hostpot ssid:");
        for (int i = 0; i < qhssid.length(); ++i)
        {
          EEPROM.write(160+i, qhssid[i]); //152+8 = 160
          Serial.print("Wrote: ");
          Serial.println(qhssid[i]);
        }
        
        //pass maks 16 digit
        Serial.println("writing eeprom Hotspot Password:");
        for (int i = 0; i < qhpass.length(); ++i)
        {
          EEPROM.write(192+i, qhpass[i]); //160+32 = 192
          Serial.print("Wrote: ");
          Serial.println(qhpass[i]);
        }
        
        EEPROM.commit();
        content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
        statusCode = 200;
      }
      
      server.send(200, "text/html", content);
      ESP.reset();
    });
    
    server.on("/vbatt", []() {
        VBATT = getVBatt();
      server.send(200, "text/html", VBATT);
    });

    server.on("/r1on", []() {
      //PCF.write(0,HIGH);
      R1stat="ON";
      EmergencyRelayStop1 = false;
      content = "<!DOCTYPE HTML>\r\n<html>\r\n";
      content += "<head>\r\n<title>Bestariweb ioT Connection Seting</title>\r\n";
      content +="<meta http-equiv=\"refresh\" content=\"0;url=/dashboard\">";
      content += "</head></html>";
      server.send(200, "text/html", content);
    });

    server.on("/r1off", []() {
      //PCF.write(0,LOW);
      R1stat="OFF";
      EmergencyRelayStop1 = true;
      content = "<!DOCTYPE HTML>\r\n<html>\r\n";
      content += "<head>\r\n<title>Bestariweb ioT Connection Seting</title>\r\n";
      content +="<meta http-equiv=\"refresh\" content=\"0;url=/dashboard\">";
      content += "</head></html>";
      server.send(200, "text/html", content);
    });

    server.on("/r2on", []() {
      //PCF.write(1,HIGH);
      R2stat="ON";
      EmergencyRelayStop2 = false;
      content = "<!DOCTYPE HTML>\r\n<html>\r\n";
      content += "<head>\r\n<title>Bestariweb ioT Connection Seting</title>\r\n";
      content +="<meta http-equiv=\"refresh\" content=\"0;url=/dashboard\">";
      content += "</head></html>";
      server.send(200, "text/html", content);
    });

    server.on("/r2off", []() {
      //PCF.write(1,LOW);
      R2stat="OFF";
      EmergencyRelayStop2 = true;
      content = "<!DOCTYPE HTML>\r\n<html>\r\n";
      content += "<head>\r\n<title>Bestariweb ioT Connection Seting</title>\r\n";
      content +="<meta http-equiv=\"refresh\" content=\"0;url=/dashboard\">";
      content += "</head></html>";
      server.send(200, "text/html", content);
    });

    server.on("/r3on", []() {
      //PCF.write(2,HIGH);
      R3stat="ON";
      EmergencyRelayStop3 = false;
      content = "<!DOCTYPE HTML>\r\n<html>\r\n";
      content += "<head>\r\n<title>Bestariweb ioT Connection Seting</title>\r\n";
      content +="<meta http-equiv=\"refresh\" content=\"0;url=/dashboard\">";
      content += "</head></html>";
      server.send(200, "text/html", content);
    });

    server.on("/r3off", []() {
      //PCF.write(2,LOW);
      R3stat="OFF";
      EmergencyRelayStop3 = true;
      content = "<!DOCTYPE HTML>\r\n<html>\r\n";
      content += "<head>\r\n<title>Bestariweb ioT Connection Seting</title>\r\n";
      content +="<meta http-equiv=\"refresh\" content=\"0;url=/dashboard\">";
      content += "</head></html>";
      server.send(200, "text/html", content);
    });
 }
}

void relaystate(){

  /****************************
   *   CHECK RELAY 1
   * **************************
   */
  if (vbattfloat <= elv1.toFloat()){
         R1STATE = "LVLOW";
         R1stat="OFF";
         Serial.println("RELAY 1 OFF by RELAYSTATE()");

      } else if ((R1STATE == "LVRUN") && (vbattfloat > elv1.toFloat()) && (vbattfloat < ehv1.toFloat())) {
         R1STATE = "LVRUN";
         R1stat="ON";
         if (EmergencyRelayStop1){
            R1stat="OFF";
            Serial.println("EMERGENCY RELAY STOP is TRUE. RELAY WILL NEVER ON");
          }
         if ((PAKAITIMER)){
           if (STATUSTIMERON){
            //Blynk.virtualWrite(VPINRELAY,RELAYSTATUS);
            R1stat="ON";
            Serial.println("RELAY ON by RELAYSTATE(). Status PAKAITIMER dan TIMERSTATUSON true");
           } else {
            //Blynk.virtualWrite(VPINRELAY,0);
            R1stat="OFF";
            Serial.println("RELAY OFF by RELAYSTATE(). Status PAKAITIMER true dan TIMERSTATUSON false");
           }
         } else {
          
          R1stat="ON";
          Serial.println("RELAY ON by RELAYSTATE(). Status PAKAITIMER false tanpa cek TIMERSTATUSON");
         }
         //Serial.print("V aki Normal Antara ");Serial.print(elv1);Serial.print(" dan ");Serial.print(ehv1);Serial.print(" State LV: ");Serial.println(R1STATE);
      } else if ((R1STATE == "LVLOW") && (vbattfloat > elv1.toFloat()) && (vbattfloat < ehv1.toFloat())) {
         R1STATE = "LVLOW";
         R1stat="OFF";
         Serial.println("RELAY OFF by RELAYSTATE(). Status elv1");
         //Serial.print("V aki Naik dari ");Serial.print(elv1);Serial.print(" menuju ");Serial.print(ehv1);Serial.print(" State LV: ");Serial.println(R1STATE);
      } else if ((R1STATE == "LVLOW") && (vbattfloat > ehv1.toFloat())) {
         R1STATE = "LVRUN";
         if ((PAKAITIMER)){
           if (STATUSTIMERON){
            R1stat="ON";
            Serial.println("RELAY 1 ON by RELAYSTATE(). Status PAKAITIMER dan TIMERSTATUSON true");
           } else {
            R1stat="OFF";
            Serial.println("RELAY 1 OFF by RELAYSTATE(). Status PAKAITIMER true dan TIMERSTATUSON false");
           }
         } else {
          
          R1stat="ON";
          Serial.println("RELAY ON by RELAYSTATE(). Status PAKAITIMER false tanpa cek TIMERSTATUSON");
         }
         //Serial.print("V aki Naik dari ");Serial.print(elv1);Serial.print(" sudah mencapai ");Serial.print(ehv1);Serial.print(" State LV: ");Serial.println(R1STATE);
      } else if ((R1STATE == "LVLOW") && (vbattfloat < elv1.toFloat())) {
         R1STATE = "LVLOW";
         R1stat="OFF";
      }
      
      else if ((R1STATE == "LVRUN") && (vbattfloat > ehv1.toFloat())) {
         R1STATE = "LVRUN";
         R1stat="ON";
         if (EmergencyRelayStop1){
            R1stat="OFF";
            Serial.println("EMERGENCY RELAY STOP is TRUE. RELAY WILL NEVER ON");
          }
         if ((PAKAITIMER)){
           if (STATUSTIMERON){
            R1stat="ON";
            Serial.println("RELAY ON by RELAYSTATE(). Status PAKAITIMER true dan TIMERSTATUSON true");
           } else {
            R1stat="OFF";
            Serial.println("RELAY OFF by RELAYSTATE(). Status PAKAITIMER true dan TIMERSTATUSON false");
           }
         } else {
          R1stat="ON";
          Serial.println("RELAY ON by RELAYSTATE(). Status PAKAITIMER false tanpa cek TIMERSTATUSON");
         }
      }


/****************************
   *   CHECK RELAY 2
   * **************************
   */
  if (vbattfloat <= elv2.toFloat()){
         R2STATE = "LVLOW";
         R2stat="OFF";
         Serial.println("RELAY 2 OFF by RELAYSTATE()");

      } else if ((R2STATE == "LVRUN") && (vbattfloat > elv2.toFloat()) && (vbattfloat < ehv2.toFloat())) {
         R2STATE = "LVRUN";
         R2stat="ON";
         if (EmergencyRelayStop2){
            R2stat="OFF";
            Serial.println("EMERGENCY RELAY STOP is TRUE. RELAY 2 WILL NEVER ON");
          }
         if ((PAKAITIMER)){
           if (STATUSTIMERON){
            //Blynk.virtualWrite(VPINRELAY,RELAYSTATUS);
            R2stat="ON";
            Serial.println("RELAY 2 ON by RELAYSTATE(). Status PAKAITIMER dan TIMERSTATUSON true");
           } else {
            //Blynk.virtualWrite(VPINRELAY,0);
            R2stat="OFF";
            Serial.println("RELAY 2 OFF by RELAYSTATE(). Status PAKAITIMER true dan TIMERSTATUSON false");
           }
         } else {
          
          R2stat="ON";
          Serial.println("RELAY ON by RELAYSTATE(). Status PAKAITIMER false tanpa cek TIMERSTATUSON");
         }
      } else if ((R2STATE == "LVLOW") && (vbattfloat > elv2.toFloat()) && (vbattfloat < ehv2.toFloat())) {
         R2STATE = "LVLOW";
         R2stat="OFF";
         Serial.println("RELAY OFF by RELAYSTATE(). Status elv2");
         //Serial.print("V aki Naik dari ");Serial.print(elv1);Serial.print(" menuju ");Serial.print(ehv1);Serial.print(" State LV: ");Serial.println(R1STATE);
      } else if ((R2STATE == "LVLOW") && (vbattfloat > ehv2.toFloat())) {
         R2STATE = "LVRUN";
         if ((PAKAITIMER)){
           if (STATUSTIMERON){
            R2stat="ON";
            Serial.println("RELAY 2 ON by RELAYSTATE(). Status PAKAITIMER dan TIMERSTATUSON true");
           } else {
            R2stat="OFF";
            Serial.println("RELAY 2 OFF by RELAYSTATE(). Status PAKAITIMER true dan TIMERSTATUSON false");
           }
         } else {
          
          R2stat="ON";
          Serial.println("RELAY ON by RELAYSTATE(). Status PAKAITIMER false tanpa cek TIMERSTATUSON");
         }
         //Serial.print("V aki Naik dari ");Serial.print(elv1);Serial.print(" sudah mencapai ");Serial.print(ehv1);Serial.print(" State LV: ");Serial.println(R1STATE);
      } else if ((R2STATE == "LVLOW") && (vbattfloat < elv2.toFloat())) {
         R2STATE = "LVLOW";
         R2stat="OFF";
      }
      
      else if ((R2STATE == "LVRUN") && (vbattfloat > ehv2.toFloat())) {
         R2STATE = "LVRUN";
         R2stat="ON";
         if (EmergencyRelayStop2){
            R2stat="OFF";
            Serial.println("EMERGENCY RELAY STOP is TRUE. RELAY 2 WILL NEVER ON");
          }
         if ((PAKAITIMER)){
           if (STATUSTIMERON){
            R2stat="ON";
            Serial.println("RELAY ON by RELAYSTATE(). Status PAKAITIMER true dan TIMERSTATUSON true");
           } else {
            R2stat="OFF";
            Serial.println("RELAY OFF by RELAYSTATE(). Status PAKAITIMER true dan TIMERSTATUSON false");
           }
         } else {
          R2stat="ON";
          Serial.println("RELAY ON by RELAYSTATE(). Status PAKAITIMER false tanpa cek TIMERSTATUSON");
         }
      }



/****************************
   *   CHECK RELAY 3
   * **************************
   */
  if (vbattfloat <= elv3.toFloat()){
         R3STATE = "LVLOW";
         R3stat="OFF";
         Serial.println("RELAY 2 OFF by RELAYSTATE()");

      } else if ((R3STATE == "LVRUN") && (vbattfloat > elv3.toFloat()) && (vbattfloat < ehv3.toFloat())) {
         R3STATE = "LVRUN";
         R3stat="ON";
         if (EmergencyRelayStop3){
            R3stat="OFF";
            Serial.println("EMERGENCY RELAY STOP is TRUE. RELAY 3 WILL NEVER ON");
          }
         if ((PAKAITIMER)){
           if (STATUSTIMERON){
            //Blynk.virtualWrite(VPINRELAY,RELAYSTATUS);
            R3stat="ON";
            Serial.println("RELAY 3 ON by RELAYSTATE(). Status PAKAITIMER dan TIMERSTATUSON true");
           } else {
            //Blynk.virtualWrite(VPINRELAY,0);
            R3stat="OFF";
            Serial.println("RELAY 3 OFF by RELAYSTATE(). Status PAKAITIMER true dan TIMERSTATUSON false");
           }
         } else {
          
          R3stat="ON";
          Serial.println("RELAY 3 ON by RELAYSTATE(). Status PAKAITIMER false tanpa cek TIMERSTATUSON");
         }
      } else if ((R3STATE == "LVLOW") && (vbattfloat > elv3.toFloat()) && (vbattfloat < ehv3.toFloat())) {
         R3STATE = "LVLOW";
         R3stat="OFF";
         Serial.println("RELAY OFF by RELAYSTATE(). Status elv3");
         //Serial.print("V aki Naik dari ");Serial.print(elv1);Serial.print(" menuju ");Serial.print(ehv1);Serial.print(" State LV: ");Serial.println(R1STATE);
      } else if ((R3STATE == "LVLOW") && (vbattfloat > ehv3.toFloat())) {
         R3STATE = "LVRUN";
         if ((PAKAITIMER)){
           if (STATUSTIMERON){
            R3stat="ON";
            Serial.println("RELAY 3 ON by RELAYSTATE(). Status PAKAITIMER dan TIMERSTATUSON true");
           } else {
            R3stat="OFF";
            Serial.println("RELAY 3 OFF by RELAYSTATE(). Status PAKAITIMER true dan TIMERSTATUSON false");
           }
         } else {
          
          R3stat="ON";
          Serial.println("RELAY 3 ON by RELAYSTATE(). Status PAKAITIMER false tanpa cek TIMERSTATUSON");
         }
         //Serial.print("V aki Naik dari ");Serial.print(elv1);Serial.print(" sudah mencapai ");Serial.print(ehv1);Serial.print(" State LV: ");Serial.println(R1STATE);
      } else if ((R3STATE == "LVLOW") && (vbattfloat < elv3.toFloat())) {
         R3STATE = "LVLOW";
         R3stat="OFF";
      }
      
      else if ((R3STATE == "LVRUN") && (vbattfloat > ehv3.toFloat())) {
         R3STATE = "LVRUN";
         R3stat="ON";
         if (EmergencyRelayStop3){
            R3stat="OFF";
            Serial.println("EMERGENCY RELAY STOP is TRUE. RELAY 3 WILL NEVER ON");
          }
         if ((PAKAITIMER)){
           if (STATUSTIMERON){
            R3stat="ON";
            Serial.println("RELAY ON by RELAYSTATE(). Status PAKAITIMER true dan TIMERSTATUSON true");
           } else {
            R3stat="OFF";
            Serial.println("RELAY OFF by RELAYSTATE(). Status PAKAITIMER true dan TIMERSTATUSON false");
           }
         } else {
          R3stat="ON";
          Serial.println("RELAY ON by RELAYSTATE(). Status PAKAITIMER false tanpa cek TIMERSTATUSON");
         }
      }


      

      Serial.print("R1STATE: ");Serial.println(R1STATE);
      Serial.print("R2STATE: ");Serial.println(R2STATE);
      Serial.print("R3STATE: ");Serial.println(R3STATE);
      Serial.print("R1stat: ");Serial.println(R1stat);
      Serial.print("R2stat: ");Serial.println(R2stat);
      Serial.print("R3stat: ");Serial.println(R3stat);

      // Set Switch di Relay 1
      if (R1stat=="ON"){
        PCF.write(0,HIGH);
        Blynk.virtualWrite(V2,"Relay 1 ON");
      } else if (R1stat=="OFF"){
        PCF.write(0,LOW);
        Blynk.virtualWrite(V2,"Relay 1 OFF");
      }

      // Set Switch di Relay 2
      if (R2stat=="ON"){
        PCF.write(1,HIGH);
        Blynk.virtualWrite(V3,"Relay 2 ON");
      } else if (R2stat=="OFF"){
        PCF.write(1,LOW);
        Blynk.virtualWrite(V3,"Relay 2 OFF");
      }

      // Set Switch di Relay 2
      if (R3stat=="ON"){
        PCF.write(2,HIGH);
        Blynk.virtualWrite(V4,"Relay 3 ON");
      } else if (R3stat=="OFF"){
        PCF.write(2,LOW);
        Blynk.virtualWrite(V4,"Relay 3 OFF");
      }
}
