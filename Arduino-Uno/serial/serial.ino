/*======================================
 * 
 *      Belajar Arduino Pemula #8
 *          Mengenal Serial
 *        by Tanto Prihartanto
 *     
 *  Copyright(c)2021 By Bestariwebhost   
 * =====================================
 */
int sudutrad ;
float hasil;

void setup() {
  Serial.begin(9600);
  sudutrad =0;
}

void loop() {
  ++sudutrad;
  hasil = sin(sudutrad*3.14/180);
  Serial.println(hasil);
  delay(10);
}
