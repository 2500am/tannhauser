#define LED_PIN 7

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // First Turn
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  delay(1000);

  // Second Turn
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
  
  digitalWrite(LED_PIN, 1);
  
  while (1) {
    
  }
}
