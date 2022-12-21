// TEST mode
// Als deze regel niet is uitgecomment zal het Instrument in test mode staan.
// Dit betekent dat hij onafhankelijk van de dirigent de `event()` functie
// aanroept. Hiermee kun je je instrument testen.
//#define TEST

// Include de benodigde code
#include "soundscape.h"

// Aantal milliseconde tussen twee test-events
// Hoe kleiner dit getal, des te sneller loopt de test
#define TEST_DELAY 500

// Snelheid van communicatie over de USB kabel. Zorg ervoor dat de seriële
// monitor van de Arduino IDE op dezelfde snelheid is ingesteld.
#define BAUD_RATE 115200

// Stel de timing pin in
#define INTERRUPT_PIN_TIMING 3

#define MELODYSIZE 1 // Moet altijd exact overeenkomen met de grootte van de melody array
// Definieer je melodie hier. Doordat er unsigned long getallen worden gebruikt,
// heb je in feite 4 bytes aan data die je zelf kunt inrichten. 
// Je kunt de array dus vullen met gewoon decimale getallen, bijv:
// { 12, 5, 542, 21, }
// Maar het is mogelijk makkelijker wanneer je solenoids aanstuurt om 
// de binaire representatie te gebruiken. Dan zie je precies welke solenoid
// je met welke noot aanstuurt, bijv:
// { 0b000, 0b001, 0b010, 0b011, 0b100, 0b101, 0b110, 0b111, }
// RGB leds, die je met PWM signalen wilt aansturen zou je dan in hexadecimale
// notatie kunnen opzetten, waarbij steeds één byte voor een kleur staat. In 
// hexadecimale notatie is één byte namlijk altijd twee cijfers, bijv:
// { 0xFFFFFF, 0x00FFFF, 0x12E0FF, }
// Als je dit niet wilt gebruiken zet dan MELODYSIZE op 1 en zet een 1 tussen de { }
const unsigned long melody[MELODYSIZE] = { 1 };

/*
 * De `setup()` functie wordt één keer uitgevoerd na opstarten of een reset
 */
void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println("Instrument is bezig met opstarten");

  // Configureer de pins. de LED_BUILTIN is pin 13: de ingebouwde LED
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(10, OUTPUT);

  // Initialiseer het instrument.
  initInstrument(INTERRUPT_PIN_TIMING, &event, melody, MELODYSIZE);
  Serial.println("Instrument is opgestart");
}

int aanTijd = 0;

/*
 * De `loop()` functie wordt iedere keer herhaald totdat de Arduino wordt 
 * uitgezet of gereset.
 */
void loop() {
  #ifdef TEST
    testInstrument(TEST_DELAY);
  #endif
  if (aanTijd > 0) {
    digitalWrite(10, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    aanTijd--;    
  } else {
    digitalWrite(10, LOW);
    digitalWrite(LED_BUILTIN, LOW);
  }
  delay(10);
}

/*
 * De `event()` functie wordt iedere keer aangeroepen als dit instrument een
 * teken krijgt van de dirigent. De meegegeven parameter `data` bevat de informatie
 * over wat dit instrument tijdens dit event moet doen.
 */
void event(unsigned long data) {
  aanTijd = 3;
}
