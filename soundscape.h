
#define BOUNCE_TIME 100

unsigned int melody_size;
unsigned int current_index;

// Pointer naar de callback functie
void (*callback_on_cue)(unsigned long);

unsigned long *currentMelody;

/*
 * Interrupt handler van de timing pin
 */
void timing_ISR() {
  static unsigned long last_timing_interrupt_time = 0;
  unsigned long interrupt_time = micros();
  // If interrupts come faster than BOUNCE_TIME us, assume it's a bounce and ignore
  if (interrupt_time - last_timing_interrupt_time > BOUNCE_TIME) {
    last_timing_interrupt_time = interrupt_time;
    // Increment and reset if needed
    current_index = (current_index + 1) % melody_size;
    unsigned long data = currentMelody[current_index];
    callback_on_cue(data);
    // Debug info
    Serial.print(current_index);
    Serial.print(": ");
    Serial.println(data);
  }
}

/*
 * Initialiseer het instrument
 *
 * @param pin the pin to attach the timing signal
 * @param callback pointer naar de functie die moet worden aangeroepen
 * @param melody pointer naar de melody array
 * @param array_size de grootte van de melody array
 */
void initInstrument(int pin, void (*callback)(unsigned long), unsigned long melody[], unsigned int array_size) {
  pinMode(pin, INPUT_PULLUP);
  Serial.print("Deze track heeft ");
  melody_size = array_size;
  Serial.print(melody_size);
  Serial.println(" events");
  callback_on_cue = callback;
  currentMelody = melody;
  current_index = 0;
  #ifndef TEST
    attachInterrupt(digitalPinToInterrupt(pin), timing_ISR, RISING);
    Serial.println("Interrupt service routines zijn gekoppeld. Instrument luistert nu naar de dirigent");
  #else
    Serial.println("!TESTMODE! Instrument reageert NIET op de dirigent!");
  #endif
}

/*
 * Hulpje om te testen
 *
 * @param delayTime aantal milliseconden wachttijd tot de volgende cue
 */ 
void testInstrument(int delayTime) {
  static unsigned long timeToNextTestEvent = 0;
  unsigned long now = millis();
  if (now > timeToNextTestEvent) {
    timing_ISR();
    timeToNextTestEvent = now + delayTime;
  }
}
