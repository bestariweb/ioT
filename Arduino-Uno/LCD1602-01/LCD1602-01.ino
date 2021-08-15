#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

/*  Koneksi PIN:
 *   i2C    |  Arduino Uno
 * =========================
 *   VCC    |   5V
 *   Gnd    |   Gnd
 *   SDA    |   A5
 *   SCL    |   A4
 */

 
// Set lcd Class
LiquidCrystal_I2C lcd(0x27,20,4);  

void setup()
{
  lcd.init();                     // initialize the lcd 
  lcd.backlight();                // Nyalakan Baclight
}


void loop()
{
  lcd.clear();                    // Bersihkan atau Hapus layar
  lcd.setCursor(2,0);             // Set Kursor di Digit ke 3 baris 1 digi
  lcd.print("Beli LCD 1602");     // Tampilkan Tulisan
  lcd.setCursor(2,1);             // Set Kursor di Digit ke 3 Baris 2
  lcd.print("di Bestariweb");
  delay(2000);                    // Tunggu / tampilkan selama 2 detik
  lcd.clear();                    // Kemudian bersihkan layar
  lcd.setCursor(2,0);             // Set kursor di Digit ke 3 Baris pertama
  lcd.print("JANGAN LUPA");
  lcd.setCursor(0,1);             // Set kursor di Digit pertama baris ke 2
  lcd.print("LIKE & SUBSCRIBE");
  delay(2000);
}
