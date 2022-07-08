void tampilkan(){
  //lcd.clear();
  lcd.setCursor(0,0);lcd.print(" MONITORING 220VAC  ");
  lcd.setCursor(0,1);lcd.print("TEG. PLN : ");
  lcd.setCursor(0,2);lcd.print("SUHU     : ");
  lcd.setCursor(0,3);lcd.print("HUMIDITY : ");
  lcd.setCursor(11,1);lcd.print(VLN);lcd.print("V ");
  lcd.setCursor(11,2);lcd.print(suhu);lcd.print((char) 223);lcd.print("C");
  lcd.setCursor(11,3);lcd.print(humidity);lcd.print("%");

  
}
