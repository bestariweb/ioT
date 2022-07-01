/*
 * Contoh penggunaan library DMDESP
 * 
 * email : bonny@grobak.net - www.grobak.net - www.elektronmart.com
*/
#include <virtuabotixRTC.h>  
#include <DMDESP.h>
#include <fonts/Arial_Black_16.h>
#include <fonts/EMSansSP8x16.h>

//SETUP DMD
#define DISPLAYS_WIDE 3 // Kolom Panel
#define DISPLAYS_HIGH 1 // Baris Panel
DMDESP Disp(DISPLAYS_WIDE, DISPLAYS_HIGH);  // Jumlah Panel P10 yang digunakan (KOLOM,BARIS)
virtuabotixRTC myRTC(2, 4, 5);


String jam,menit,detik,tgl;
char daysOfTheWeek[7][8] = {"MINGGU", "SENIN", "SELASA", "RABU", "KAMIS", "JUM'AT", "SABTU"};
char NamaBulan[12][10] = {"Januari", "Februari", "Maret", "April", "Mei", "Juni", "Juli", "Agustus", "September", "Oktober", "November", "Desember"};
char hariini;
static uint32_t mls;
bool kotor = true;
int kiri;
//----------------------------------------------------------------------
// SETUP

void setup() {

  // DMDESP Setup
  Disp.start(); // Jalankan library DMDESP
  Disp.setBrightness(20); // Tingkat kecerahan
  Disp.setFont(Arial_Black_16); // Tentukan huruf
  mls = millis();
  //myRTC.setDS1302Time(00, 49, 02, 5, 1, 7, 2022);
}



//----------------------------------------------------------------------
// LOOP

void loop() {
  
  Disp.loop(); // Jalankan Disp loop untuk refresh LED
  myRTC.updateTime(); 
  
  //hariini = myRTC.dayofmonth+" "+NamaBulan[myRTC.month];
  if (myRTC.hours < 10){
    jam = " 0"+String(myRTC.hours);
  } else {
    jam = " "+String(myRTC.hours);
  }
  if (myRTC.minutes < 10){
    jam +=":0"+String(myRTC.minutes);
  } else {
    jam +=":"+String(myRTC.minutes);
  }
  if (myRTC.seconds < 10){
    jam += ":0"+String(myRTC.seconds)+" ";
  } else {
    jam += ":"+String(myRTC.seconds)+" ";
  }
  tgl = String(daysOfTheWeek[myRTC.dayofweek])+", "+String(myRTC.dayofmonth)+" "+NamaBulan[myRTC.month-1]+" "+String(myRTC.year)+" Jam"+jam;
  if (millis()-mls < 60000){
  TeksJalan(1, 30); // Tampilkan teks berjalan TeksJalan(posisi y, kecepatan);
  kotor = true;
  //TeksJalan2(8, 70);
  //mls = millis();
  
  } else {
     if (kotor) {
      kotor=false;
      Disp.clear();
     }
     Disp.setFont(EMSansSP8x16);
     kiri = (Disp.width() - Disp.textWidth(jam.c_str())) / 2;
     Disp.drawText(kiri,0,jam.c_str());
     if (millis()-mls > 120000){
      mls = millis();
      Disp.clear();
      
     }
  }

  
  
}


//--------------------------
// TAMPILKAN SCROLLING TEKS


void TeksJalan(int y, uint8_t kecepatan) {

  static uint32_t pM;
  static uint32_t x;
  int width = Disp.width();
  Disp.setFont(Arial_Black_16);
  int fullScroll = Disp.textWidth(tgl.c_str()) + width;
  if((millis() - pM) > kecepatan) { 
    pM = millis();
    if (x < fullScroll) {
      ++x;
    } else {
      x = 0;
      return;
    }
    Disp.drawText(width - x, y, tgl.c_str());
  }  

}
