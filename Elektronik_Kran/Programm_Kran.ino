#include <Servo.h>

Servo meinServo;

// Pins
const int tasterPin = 2;
const int servoPin = 9;

// Einstellbare Werte
int startWinkel = 180;
int endWinkel   = 90;
int schrittZeit = 1;

// Zustände
bool zielZustand = false;
bool letzterTasterStatus = HIGH;

int aktuellerWinkel;
bool servoAktiv = true;

// Timing
unsigned long zielErreichtZeit = 0;
const unsigned long abschaltDelay = 500; // 0,5 Sekunden

void setup() {
  pinMode(tasterPin, INPUT_PULLUP);

  meinServo.attach(servoPin);
  aktuellerWinkel = startWinkel;
  meinServo.write(aktuellerWinkel);
}

void loop() {
  bool tasterStatus = digitalRead(tasterPin);

  // Flankenerkennung
  if (letzterTasterStatus == HIGH && tasterStatus == LOW) {
    zielZustand = !zielZustand;

    // Servo wieder aktivieren, falls er abgeschaltet war
    if (!servoAktiv) {
      meinServo.attach(servoPin);
      servoAktiv = true;
    }

    delay(50); // Entprellung
  }

  letzterTasterStatus = tasterStatus;

  int zielWinkel = zielZustand ? endWinkel : startWinkel;

  // Bewegung
  if (servoAktiv) {
    if (aktuellerWinkel < zielWinkel) {
      aktuellerWinkel++;
      meinServo.write(aktuellerWinkel);
      delay(schrittZeit);
      zielErreichtZeit = millis();
    } 
    else if (aktuellerWinkel > zielWinkel) {
      aktuellerWinkel--;
      meinServo.write(aktuellerWinkel);
      delay(schrittZeit);
      zielErreichtZeit = millis();
    } 
    else {
      // Ziel erreicht → Zeit merken
      if (zielErreichtZeit == 0) {
        zielErreichtZeit = millis();
      }

      // Nach 0,5 s abschalten
      if (millis() - zielErreichtZeit >= abschaltDelay) {
        meinServo.detach();
        servoAktiv = false;
        zielErreichtZeit = 0; // zurücksetzen
      }
    }
  }
}