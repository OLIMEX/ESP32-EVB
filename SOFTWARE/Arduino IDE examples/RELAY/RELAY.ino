//This demo will make the two relays of Olimex ESP32-EVB turn on and off alternatively,
//unless button BUT1 is held pressed which will force a pause. Releasing button BUT1 will
//resume relay switching on and off.

#define RELAY1 32
#define RELAY2 33
#define BUTTON 34

void setup()
{
pinMode (BUTTON, INPUT);
pinMode (RELAY1, OUTPUT);
pinMode (RELAY2, OUTPUT);
}

void loop()
{
while (!digitalRead (BUTTON));
digitalWrite (RELAY1, HIGH);
digitalWrite (RELAY2, LOW);
delay (1000);
digitalWrite (RELAY1, LOW);
digitalWrite (RELAY2, HIGH);
delay (1000);
}
