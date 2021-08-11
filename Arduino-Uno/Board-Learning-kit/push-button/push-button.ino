const byte LED[] = {13,12,11,10};
 
#define BUTTON1 A1
#define BUTTON2 A2
 
void setup()
{
	// initialize the digital pin as an output.
	/* Set each pin to outputs */
	pinMode(LED[0], OUTPUT);
	pinMode(LED[1], OUTPUT);
	pinMode(LED[2], OUTPUT);
	pinMode(LED[3], OUTPUT);
}
 
void loop()
{
	if(!digitalRead(BUTTON1))
	{
		digitalWrite(LED[0], HIGH);
		digitalWrite(LED[1], HIGH);
		digitalWrite(LED[2], HIGH);
		digitalWrite(LED[3], HIGH);
	}
 
	if(!digitalRead(BUTTON2))
	{
		digitalWrite(LED[0], LOW);
		digitalWrite(LED[1], LOW);
		digitalWrite(LED[2], LOW);
		digitalWrite(LED[3], LOW);
		}
}
