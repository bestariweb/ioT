/*========================================
 *          SP01 ESP8266 PROGRAM
 *     UNTUK KOMUNIKASI DENGAN ARDUINO
 *         DAN KIRIM DATA KE BLYNK 
 *             DAN THINGSBOARD
 *           
 *  Copyright(C)2021 by Tanto Prihartanto        
 *========================================
 */
#include <ThingsBoard.h>
#include <BlynkSimpleEsp8266.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ArduinoJson.h>

 
//Variables

//Thingsboard & Blynk token data
#define BLYNK_PRINT serial
#define TOKEN "tokenthingsboard-disini"
#define THINGSBOARD_SERVER  "111.112.113.114"  // IP Server Thingsboard

// Initialize ThingsBoard client
WiFiClient espClient;
// Initialize ThingsBoard instance
ThingsBoard tb(espClient);

// the Wifi radio's status
int status = WL_IDLE_STATUS;

const char* BlynkServer = "server-blynk-disini";
const char* Blynkauth = "token-auth-blynk-disini";

BlynkTimer timer;


int i = 0;
int statusCode;
const char* ssid = "text";
const char* passphrase = "text";
String st;
String content;
String incomingByte;
 
 
//Function Decalration
bool testWifi(void);
void launchWeb(void);
void setupAP(void);
 
//Establishing Local server at port 80 whenever required
ESP8266WebServer server(80);
 
void setup()
{
 
  Serial.begin(115200); //Initialising if(DEBUG)Serial Monitor
  Serial.println();
  Serial.println("Disconnecting previously connected WiFi");
  WiFi.disconnect();
  EEPROM.begin(512); //Initialasing EEPROM
  delay(10);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println();
  Serial.println();
  Serial.println("Startup");
 
  //---------------------------------------- Read EEPROM for SSID and pass
  Serial.println("Reading EEPROM ssid");
 
  String esid;
  for (int i = 0; i < 32; ++i)
  {
    esid += char(EEPROM.read(i));
  }
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(esid);
  Serial.println("Reading EEPROM pass");
 
  String epass = "";
  for (int i = 32; i < 96; ++i)
  {
    epass += char(EEPROM.read(i));
  }
  Serial.print("PASS: ");
  Serial.println(epass);
 
 
  WiFi.begin(esid.c_str(), epass.c_str());
  if (testWifi())
  {
    Serial.println("Succesfully Connected!!!");
    return;
  }
  else
  {
    Serial.println("Turning the HotSpot On");
    launchWeb();
    setupAP();// Setup HotSpot
  }
 
  Serial.println();
  Serial.println("Waiting.");
  
  while ((WiFi.status() != WL_CONNECTED))
  {
    Serial.print(".");
    delay(100);
    server.handleClient();
  }

  Blynk.config(Blynkauth,BlynkServer,8080);
  Blynk.connect();
 
}
void loop() {
  /*
  if ((WiFi.status() == WL_CONNECTED))
  {
 
    for (int i = 0; i < 10; i++)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);
      digitalWrite(LED_BUILTIN, LOW);
      delay(1000);
    }
 
  }
  else
  {
  }
  */
  if (Blynk.connected()) {  
  Blynk.run();  
  } else {
    Serial.println("Connecting to Blynk Server..");
    Blynk.config(Blynkauth,BlynkServer,8080);
    Blynk.connect();
  }
  if (!tb.connected()) {
    // Connect to the ThingsBoard
    //Serial.print("Connecting to: ");
    //Serial.print(THINGSBOARD_SERVER);
    //Serial.print(" with token ");
    //Serial.println(TOKEN);
    Serial.print("RECONNECT TO");
    Serial.print("THINGSBOARD SERVER");
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      Serial.println("Failed to connect");
      Serial.print("THINGSBOARD SERVER");
      Serial.print("FAILED TO CONNECT");
      return;
    }
  }
  
 timer.run();
  if (Serial.available()) 
  {
    int jumlah;
    String nama;
    int nvar;
    int hurufke = 0;
    String namavar;
    
    // Allocate the JSON document
    // This one must be bigger than for the sender because it must store the strings
    StaticJsonDocument<512> doc;
    StaticJsonDocument<500> datanya;
    StaticJsonDocument<200> detailnya;
    // create an empty array
    //JsonArray array = datanya.to<JsonArray>();
    
    // Read the JSON document from the "link" serial port
    DeserializationError err = deserializeJson(doc, Serial);
    
    if (err == DeserializationError::Ok) 
    {
      // Print the values
      // (we must use as<T>() to resolve the ambiguity)
      // Contoh Data:
      // {"total":5,"datanya":[{"namavar":"CarbonMonoksida","nilai":3.20},{"namavar":"Alcohol","nilai":1.16},{"namavar":"CarbonDioksida","nilai":2.48},{"namavar":"Tolueno","nilai":0.49},{"namavar":"Amonium","nilai":3.90},{"namavar":"Acetona","nilai":34.67}]}
      jumlah = doc["total"].as<int>();
      
      //Serial.print("Jumlah Variable: ");
      //Serial.println(jumlah);
      //Serial.println("Nama Variable: ");
//    datanya = doc["datanya"].as<JsonArray>()
      deserializeJson(datanya, doc["datanya"].as<String>());  
      String Datasebenarnya;
      JsonArray array = datanya.as<JsonArray>();
      nvar =0;
      for(JsonVariant v : array) {
        nvar++;
        Datasebenarnya = v.as<String>();
        //Serial.println(v.as<String>());
        deserializeJson(detailnya, Datasebenarnya);
        //Kirim data ke Cloud disini
        namavar = detailnya["namavar"].as<String>();
        Serial.print(namavar);Serial.print(" : ");Serial.println(detailnya["nilai"].as<float>());
        tb.sendTelemetryFloat(namavar.c_str(), detailnya["nilai"].as<float>());
        switch (nvar) {
          case 1:
            Blynk.virtualWrite(V1,detailnya["nilai"].as<float>());
            break;
          case 2:
            Blynk.virtualWrite(V2,detailnya["nilai"].as<float>());
            break;
          case 3:
            Blynk.virtualWrite(V3,detailnya["nilai"].as<float>());
            break;
          case 4:
            Blynk.virtualWrite(V4,detailnya["nilai"].as<float>());
            break;
          case 5:
            Blynk.virtualWrite(V5,detailnya["nilai"].as<float>());
            break;
          case 6:
            Blynk.virtualWrite(V6,detailnya["nilai"].as<float>());
            break;
          case 7:
            Blynk.virtualWrite(V7,detailnya["nilai"].as<float>());
            break;
          case 8:
            Blynk.virtualWrite(V8,detailnya["nilai"].as<float>());
            break;
          case 9:
            Blynk.virtualWrite(V9,detailnya["nilai"].as<float>());
            break;
          case 10:
            Blynk.virtualWrite(V10,detailnya["nilai"].as<float>());
            break;
          default:
            // if nothing else matches, do the default
            // default is optional
            break;
        }
        
        
      }
      /*
      Serial.print("timestamp = ");
      Serial.println(doc["timestamp"].as<long>());
      Serial.print("value = ");
      Serial.println(doc["value"].as<int>());
      */
    } 
    else 
    {
      // Print error to the "debug" serial port
      Serial.print("deserializeJson() returned ");
      Serial.println(err.c_str());
  
      // Flush all bytes in the "link" serial port buffer
      while (Serial.available() > 0){
        Serial.read();
      }
    }
  }
 
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
  st = "<ol>";
  for (int i = 0; i < n; ++i)
  {
    // Print SSID and RSSI for each network found
    st += "<li>";
    st += WiFi.SSID(i);
    st += " (";
    st += WiFi.RSSI(i);
 
    st += ")";
    st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
    st += "</li>";
  }
  st += "</ol>";
  delay(100);
  WiFi.softAP("how2electronics", "");
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
      content = "<!DOCTYPE HTML>\r\n<html>Hello from ESP8266 at ";
      content += "<form action=\"/scan\" method=\"POST\"><input type=\"submit\" value=\"scan\"></form>";
      content += ipStr;
      content += "<p>";
      content += st;
      content += "</p><form method='get' action='setting'><label>SSID: </label><input name='ssid' length=32><input name='pass' length=64><input type='submit'></form>";
      content += "</html>";
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
      if (qsid.length() > 0) {
        Serial.println("clearing eeprom");
        for (int i = 0; i < 96; ++i) {
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
