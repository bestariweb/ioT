//
//    FILE: ACS712_20_AC.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo
//    DATE: 2020-03-18


#include "ACS712.h"
#define RELAYPIN 4

float KoreksiBebanNol =0;
int kejadian = 0;
bool loopberikutnya = false;

// Arduino UNO has 5.0 volt with a max ADC value of 1023 steps
// ACS712 5A  uses 185 mV per A
// ACS712 20A uses 100 mV per A
// ACS712 30A uses  66 mV per A


ACS712  ACS(A0, 5.0, 1023, 185);
// ESP 32 example (requires resistors to step down the logic voltage)
// ACS712  ACS(25, 5.0, 4095, 185);


void setup()
{
  Serial.begin(115200);
  Serial.println(__FILE__);
  pinMode(RELAYPIN,OUTPUT);
  digitalWrite(RELAYPIN,1);
  ACS.autoMidPoint();
  Serial.print("MidPoint: ");
  Serial.print(ACS.getMidPoint());
  Serial.print(". Noise mV: ");
  Serial.println(ACS.getNoisemV());
}


void loop()
{ 
  int mA = 0;
  for (int i =0;i<20;++i){
     mA += ACS.mA_AC();
  }
  float arus = mA/20;
  //int NilaiA0 = analogRead(A0);
  float FaktorDaya = ACS.getFormFactor();
  float daya = 220 * arus * FaktorDaya / 1000;
  //Serial.print("\tA0: ");
  //Serial.print(NilaiA0);
  Serial.print("\tmA: ");
  Serial.print(arus);
  Serial.print("\t Form factor: ");
  Serial.print(FaktorDaya);
  Serial.print("\t Daya: ");
  Serial.println(daya);
  delay(100);
  if ((daya > 50) && (loopberikutnya)){
    digitalWrite(RELAYPIN,0);
    ++kejadian;
    if (kejadian < 3){
      delay(10000);
      digitalWrite(RELAYPIN,1);
    } else {
      while (1) {
        delay(100);
        Serial.println("Wait for RESET");
      }
    }
  }
  loopberikutnya = true;
  
}


// -- END OF FILE --
