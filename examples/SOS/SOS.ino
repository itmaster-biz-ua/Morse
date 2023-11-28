// Example testing sketch for Morse code.
#include <Morse.h>
#define PIN 11
#define POLARITY 1

Morse morse(PIN, POLARITY);
 
void setup()
{
  // open a serial port with a speed of 9600 baud
  Serial.begin(9600);
  Serial.print("Morse simulator\n");
  
  // Set a velocity
  int status = morse.setVelocity(DEFAULT_VELOCITY_MINUTES);
  if (ERROR == status) {
    Serial.print("Wrong velocity\n");
  }
  else {  
    Serial.print("Velocity: ");
    Serial.print(DEFAULT_VELOCITY_MINUTES);
    Serial.print(" letters per minute\n");
  }
}
 
void loop()
{
  int status;

  Serial.print("sos");
  Serial.print("\n");

  // Send sos
  status = morse.sendWordToPin("sos", strlen("sos"));
  if (ERROR == status) {
    Serial.print(" sendWordToPin error\n");
  }

  // Delay between words
  delay(SPACE_WORD_TIME_DEFAULT); 
}
