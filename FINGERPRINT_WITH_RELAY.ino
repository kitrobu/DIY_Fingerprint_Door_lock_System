#include <Adafruit_Fingerprint.h>
#include <Arduino.h>

SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

#define RELAY_PIN       4
#define BUZZER_PIN      5
#define GREEN_LED_PIN   6
#define RED_LED_PIN     7
#define ACCESS_DELAY    3000 // Keep lock unlocked for 3 seconds 

void setup()
{
  // Set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);

  if (!finger.verifyPassword()) 
  {
    // Verification failed, stop the program
    while (1) {
      delay(1);
    }
  }

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Switch off relay initially

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
}

void loop()
{
  int fingerprintID = getFingerPrint();

  if (fingerprintID != -1)
  {
    digitalWrite(RELAY_PIN, LOW);

    // Unlock indication
    digitalWrite(GREEN_LED_PIN, HIGH);
    digitalWrite(RED_LED_PIN, LOW);
    tone(BUZZER_PIN, 1000, ACCESS_DELAY);

    delay(ACCESS_DELAY);

    // Lock indication
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, HIGH);
    noTone(BUZZER_PIN);

    digitalWrite(RELAY_PIN, HIGH);
  }

  delay(50); // Add some delay before the next scan.
}

int getFingerPrint() 
{
  int p = finger.getImage();

  if (p != FINGERPRINT_OK)  
    return -1;

  p = finger.image2Tz();

  if (p != FINGERPRINT_OK)  
    return -1;

  p = finger.fingerFastSearch();

  if (p != FINGERPRINT_OK)  
    return -1;

  // Found a match!
  return finger.fingerID;
}
