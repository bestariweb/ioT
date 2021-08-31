/*========================================
 *          ESP01 ESP8266 PROGRAM
 *     UNTUK ON OFF RELAY MELALUI WIFI
 *           
 *  Copyright
 *  (C)2021 by Tanto Prihartanto        
 *  (C)2021 by Bestariwebhost
 *========================================
 */

#include <BlynkSimpleEsp8266.h>

#include <ESP8266WiFi.h>
#define BLYNK_PRINT serial

WiFiClient espClient;

const char* BlynkServer = "isi blynk server disini";
const char* Blynkauth = "isi blynk auth disini";
const char* ssid = "Bestariwebhost.id";
const char* ssidpassword = "";

BlynkTimer timer;

 
//Function Decalration
bool testWifi(void);
 
void setup()
{
 
  Serial.begin(115200); //Initialising if(DEBUG)Serial Monitor
  digitalWrite(0,HIGH);
  Serial.println("Disconnecting previously connected WiFi");
  WiFi.disconnect();
   
  WiFi.begin(ssid,ssidpassword);
  if (testWifi())
  {
    Serial.println("Wifi Succesfully Connected!!!");
    
    Blynk.config(Blynkauth,BlynkServer,8080);
    Blynk.connect();
    
    return;
  }
  else
  {
    Serial.println("Tidak dapat terkoneksi ke wifi");
  }

}




void loop() {

  if (Blynk.connected()) {  
  Blynk.run();  
  Serial.println("Blynk Connected.");
  } else {
    Serial.println("Try to reconnect...");
    Serial.println("Re Connecting to Blynk Server..");
    Blynk.config(Blynkauth,BlynkServer,8080);
    Blynk.connect();
   
  }

  
 timer.run();
 
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
