/****************************************
 *         Menampilkan Nilai A0 
 *         di 7 Segment Display
 * 
 *   Nilai A0 diatur dari Potensio
 *   Copyright(C)2021 by Bestariwebhost
 * 
 * ***************************************
 */





/* Define shift register pins used for seven segment display */
#define LATCH_DIO 4
#define CLK_DIO 7
#define DATA_DIO 8
int ygditekan = 0;
/* Segment byte maps for numbers 0 to 9 */
const byte SEGMENT_MAP[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90};

/* Byte maps to select digit 1 to 4 */
const byte SEGMENT_SELECT[] = {0xF1,0xF2,0xF4,0xF8};
int digit1 =0;
int digit2 =0;
int digit3 =0;
int digit4 =0;

void setup() {

  /* Set DIO pins 7 Segment to outputs */
  pinMode(LATCH_DIO,OUTPUT);
  pinMode(CLK_DIO,OUTPUT);
  pinMode(DATA_DIO,OUTPUT);
  
  pinMode(A0,INPUT);
  Serial.begin(9600);
}

void loop() {

/* Update the display with the current counter value */

WriteNumberToSegment(0 , digit1);
WriteNumberToSegment(1 , digit2);
WriteNumberToSegment(2 , digit3);
WriteNumberToSegment(3 , digit4);

int  potensio = analogRead(A0);

digit1 = (potensio / 1000);
digit2 = ((potensio - digit1*1000) / 100);
digit3 = (potensio - (digit1*1000) - (digit2 * 100)) / 10; 
digit4 = (potensio - (digit1*1000) - (digit2 * 100) - (digit3*10));

}


/* Write a decimal number between 0 and 9 to one of the 4 digits of the display */
void WriteNumberToSegment(byte Segment, byte Value)
{
  digitalWrite(LATCH_DIO,LOW);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_MAP[Value]);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_SELECT[Segment] );
  digitalWrite(LATCH_DIO,HIGH);
}
