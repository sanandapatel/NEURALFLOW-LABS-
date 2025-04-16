#define TRIG_PIN 9
#define ECHO_PIN 10
#define LED_PIN 7

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  long duration;
  float distance;

  // Send a 10us HIGH pulse to TRIG
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the duration of the echo pulse
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate distance in cm
  distance = duration * 0.0343 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Check if distance is 15 cm or less
  if (distance <= 15.0) {
    digitalWrite(LED_PIN, HIGH);  // Turn LED ON
  } else {
    digitalWrite(LED_PIN, LOW);   // Turn LED OFF
  }

  delay(200); // Add a small delay for stability
}
