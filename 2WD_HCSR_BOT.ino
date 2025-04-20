// -------------------- Motor driver control pins --------------------
const int ENA = 5;      // PWM for left motor
const int ENB = 6;      // PWM for right motor
const int IN1 = 8;
const int IN2 = 9;
const int IN3 = 10;
const int IN4 = 11;

// -------------------- Ultrasonic pins -- ------------------
const int trigPin = 2;
const int echoPin = 3;

// -------------------- Motor Speeds --------------------
int motorSpeed = 180;   // Forward speed
int turnSpeed = 160;    // Turn speed
const int threshold = 20;  // Distance threshold in cm

void setup() {
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);
}

void loop() {
  int dist = getDistance();
  Serial.print("Front Distance: "); Serial.println(dist);

  if (dist > threshold) {
    moveForward();
  } else {
    stopMotors();
    delay(300);

    int left = scan("left");
    delay(200);
    int right = scan("right");
    delay(200);

    Serial.print("Left: "); Serial.print(left);
    Serial.print(" | Right: "); Serial.println(right);

    if (left > right) {
      turnLeft();
    } else {
      turnRight();
    }

    delay(500); // time to complete the turn
  }

  delay(100);  // loop delay
}

// -------------------- Motor Control Functions --------------------

void moveForward() {
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);

  digitalWrite(IN2, HIGH); digitalWrite(IN1, LOW);   // Left motor forward
  digitalWrite(IN4, LOW);  digitalWrite(IN3, HIGH);  // Right motor forward (reversed)
}

void turnLeft() {
  analogWrite(ENA, turnSpeed);
  analogWrite(ENB, turnSpeed);

  digitalWrite(IN2, LOW); digitalWrite(IN1, HIGH);   // Left motor backward
  digitalWrite(IN4, LOW); digitalWrite(IN3, HIGH);   // Right motor forward
}

void turnRight() {
  analogWrite(ENA, turnSpeed);
  analogWrite(ENB, turnSpeed);

  digitalWrite(IN2, HIGH); digitalWrite(IN1, LOW);   // Left motor forward
  digitalWrite(IN4, HIGH); digitalWrite(IN3, LOW);   // Right motor backward
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

// -------------------- Scanning Function --------------------

int scan(String direction) {
  if (direction == "left") {
    turnLeft();
  } else {
    turnRight();
  }

  delay(350);  // turn and scan position
  stopMotors();
  delay(100);
  int dist = getDistance();

  // return to original orientation
  if (direction == "left") {
    turnRight();
  } else {
    turnLeft();
  }
  delay(350);
  stopMotors();

  return dist;
}

// -------------------- Ultrasonic Distance Function --------------------

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000); // max wait: 30ms
  int distance = duration * 0.034 / 2;

  if (distance == 0 || distance > 200) {
    return 200;  // default to max range
  }

  return distance;
}
