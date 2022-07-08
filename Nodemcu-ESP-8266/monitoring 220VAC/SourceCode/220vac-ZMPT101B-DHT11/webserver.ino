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
  Serial.print("Hotspot Password: ");
  Serial.println(ehpass);

  
}

void scanssid(){
  int n = WiFi.scanNetworks();
  if (n == 0){
    Serial.println("no networks found");
  }
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
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
    st += "\"";
    if (WiFi.SSID(i)==essid){
      st += " selected ";
    }
    st += ">";
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
      
      content = "<!DOCTYPE HTML>\r\n<html>\r\n";
      content += "<head>\r\n<title>Bestariweb ioT Dashboard</title>\r\n";
      content +="<meta http-equiv=\"refresh\" content=\"5;url=/dashboard\">";
      content += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
      content += "<style>\r\n";
      content += "html{background:#333;color:#fff;}body{font-family:arial;margin:0;min-height:800px;}form{margin:10px 10px 150px 10px;}label{font-size:14px;width:140px;float:left;padding:8px;}input[type=password],input[type=number],input[type=text]{font-size:16px;border: none;padding: 8px;width:45%;float:left;background: #dbdb07;box-shadow: inset 2px 2px 2px rgba(0,0,0,0.5);border-radius: 6px;color: #333;max-width:150px}h1{font-size:18px;border-bottom: solid 1px #aaa;padding: 5px 0;margin: 10px 0;text-transform: uppercase;}";
      content += "input[type=number]::-webkit-inner-spin-button {opacity: 1;}p{float:left;width:100%}";
      content += ".vleft{font-size:16px;padding: 8px;margin: 10px 0;text-transform: uppercase;width:45%;float:left;text-align:right;}.vright{font-size:16px;border: none;padding: 8px;margin: 10px 0;text-transform: uppercase;width:45%;float:left;background: #dbdb07;box-shadow: inset 2px 2px 2px rgba(0,0,0,0.5);border-radius: 6px;color: #333;max-width:150px}";
      content += ".tombol{width:90%;max-width:300px;clear:both;background:#aa0000;color:#ffffff;margin:10px auto;border-radius:10px;}.tombol a{color:#fff;text-align:center;display:block;padding:20px;text-decoration:none}";
      content += ".topmenu{width:95%;margin:10px auto;border-bottom:solid 1px #aaf;height:29px;}.menuitem{border-radius:5px 5px 0 0;float:left;background:#007;color:#fff;padding:5px 8px;margin:0 3px 0 0;}";
      content += ".menuitem a{text-decoration:none;color:#aac}.topmenu > .active {background:#00a!important;border: 1px #aaf solid;border-bottom:none;box-shadow: 10px -1px 3px rgba(0,0,0,0.6);z-index: 1000;position: relative;}";
      content += "\r\n</style>\r\n";
      content += "</head><body><div class=\"topmenu\"><div class=\"menuitem active\">DASHBOARD</div><div class=\"menuitem\"><a href=\"/setting\">SETTING</a></div></div>";
      content += "<h1>BATTERY &amp; PV DATA</h1>";
      content += "<div class=\"vleft\">Tegangan </div><div class=\"vright\">";
      content += String(VLN);
      content += "&nbsp;Volt</div><div class=\"vleft\">Suhu</div><div class=\"vright\">";
      content += String(suhu);
      content += "</div><div class=\"vleft\">Kelembaban</div><div class=\"vright\">";
      content += String(humidity);
      content += "</div><div style=\"clear:both;margin:20px 0;height:50px;\"></div>"; 
      content += "<div class=\"vleft\"><a style=\"background:#00a;color:#fff;padding:10px;\"href=\"/reboot\">REBOOT / RESTART</a></div>";
      content += "<div style=\"clear:both;margin:150px 0;height:200px;\"></div>";
      content += "<div style=\"position:fixed;bottom:0;display:block;font-size:10px;background:#ff6000;color:#ffffff;width:100%;padding:8px 0;text-align:center;\">";
      content += "MAC:";
      content += WiFi.macAddress();
      content += "<br>Copyright&copy;2021 Bestariweb</div>";
      content += "</body></html>";
      server.send(200, "text/html", content);
    });

    server.on("/setting", []() {
      webopened = true;
      content = "<!DOCTYPE HTML>\r\n<html>\r\n";
      content += "<head>\r\n<title>Bestariweb ioT SETTING</title>\r\n";
      content += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
      content += "<style>\r\n";
      content += "html{background:#333;color:#fff;}body{font-family:arial;margin:0;min-height:800px;}form{margin:10px 10px 150px 10px;}label{font-size:14px;width:140px;float:left;padding:8px;}input[type=password],input[type=number],input[type=text]{font-size:16px;border: none;padding: 8px;width:45%;float:left;background: #dbdb07;box-shadow: inset 2px 2px 2px rgba(0,0,0,0.5);border-radius: 6px;color: #333;max-width:150px}h1{font-size:18px;border-bottom: solid 1px #aaa;padding: 5px 0;margin: 10px 0;text-transform: uppercase;}";
      content += "input[type=number]::-webkit-inner-spin-button {opacity: 1;}p{float:left;width:100%}";
      content += ".vleft{font-size:16px;padding: 8px;margin: 10px 0;text-transform: uppercase;width:45%;float:left;text-align:right;}.vright{font-size:16px;border: none;padding: 8px;margin: 10px 0;text-transform: uppercase;width:45%;float:left;background: #dbdb07;box-shadow: inset 2px 2px 2px rgba(0,0,0,0.5);border-radius: 6px;color: #333;max-width:150px}";
      content += ".tombol{width:90%;max-width:300px;clear:both;background:#aa0000;color:#ffffff;margin:10px auto;border-radius:10px;}.tombol a{color:#fff;text-align:center;display:block;padding:20px;text-decoration:none}";
      content += ".topmenu{width:95%;margin:10px auto;border-bottom:solid 1px #aaf;height:29px;}.menuitem{border-radius:5px 5px 0 0;float:left;background:#007;color:#fff;padding:5px 8px;margin:0 3px 0 0;}";
      content += ".menuitem a{text-decoration:none;color:#aac}.topmenu > .active {background:#00a!important;border: 1px #aaf solid;border-bottom:none;box-shadow: 10px -1px 3px rgba(0,0,0,0.6);z-index: 1000;position: relative;}";
      content += "\r\n</style>\r\n";
      content += "</head><body><div class=\"topmenu\"><div class=\"menuitem \"><a href=\"/dashboard\">DASHBOARD</a></div><div class=\"menuitem active\">SETTING</div></div>";
      content += "<form action=\"/savesetting\">";
      content += "<h1>Access Point Setting</h1>"+st;
      content += "<p><label>Wifi Password: </label><input type='password' name='pass' value='"+epass+"' length=64></p>";
      content += "<p><label>Blynk Server: </label><input type='text' value='"+eblserver+"' name='blserver' length=64></p>";
      content += "<p><label>Blynk Auth: </label><input type='text' value='"+eblauth+"' name='blauth' length=64></p>";    
      content += "<h1>Hotspot Setting</h1>";
      content += "<p><label>SSID: </label><input type='text' name='hssid' value='"+ehssid+"' length=64></p>";
      content += "<p><label>Password: </label><input type='password' name='hpass' value='"+ehpass+"' length=64></p>";
      content += "<input type='submit' name='tombol' value='SAVE'><input type='submit' value='SAVE &amp; REBOOT'><div style=\"clear:both;margin:150px 0;height:200px;\"></div></form>";
      
      content += "<div style=\"position:fixed;bottom:0;display:block;font-size:10px;background:#ff6000;color:#ffffff;width:100%;padding:8px 0;text-align:center;\">";
      content += "MAC:";
      content += WiFi.macAddress();
      content += "<br>Copyright&copy;2021 Bestariweb</div>";
      content += "</body></html>";
      server.send(200, "text/html", content);
    });
 server.on("/reboot", []() {
  content = "{\"Success\":\"Device akan segera reboot\"}";
  server.send(200, "text/json", content);
  ESP.reset();
 });
 
 server.on("/savesetting", []() {
      String qssid = server.arg("ssid");
      String qpass = server.arg("pass");
      String qblauth = server.arg("blauth");
      String qblserver = server.arg("blserver");
      String qhssid = server.arg("hssid");
      String qhpass = server.arg("hpass");
      String qbtn = server.arg("tombol");
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
      
      //server.send(200, "text/html", content);
      if (qbtn !="SAVE") {
        content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
        server.send(200, "text/html", content);
        ESP.reset();
      } else {
      content = "<!DOCTYPE HTML>\r\n<html>\r\n";
      content += "<head>\r\n<title>Bestariweb ioT Connection Seting</title>\r\n";
      content += "<meta http-equiv=\"refresh\" content=\"0;url=/setting\">";
      content += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";

      content += "<style>\r\n";
      content += "\r\n</style>\r\n</head>\r\n<body></BODY></HTML>\r\n";
      
// AKHIR ISI WEB ======================
      server.send(200, "text/html", content);
      }
    });



 }
}
