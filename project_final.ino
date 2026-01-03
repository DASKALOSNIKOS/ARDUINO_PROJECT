/*********************************************************
 * Obstacle Avoiding Robot - Final 
 * Arduino UNO + L298N + HC-SR04 + Servo
 *
 * Final wiring (summary):
 * - L298N: ENA/ENB jumpers ON
 * - IN1->D7, IN2->D6, IN3->D5, IN4->D4
 * - HC-SR04: TRIG->D9, ECHO->D8, VCC->5V(L298N), GND->GND(L298N)
 * - Servo: SIG->D10, VCC->5V(Arduino), GND->GND(Arduino)
 * - Power: Battery +12V -> L298N VMS, Battery- -> L298N GND
 *          +12V -> Arduino VIN, GND common everywhere
 *
 * Behavior:
 * - Move forward when front distance is clear
 * - If obstacle is close: stop, scan left/right, reverse a bit, turn to freer side
 *********************************************************/

#include <Servo.h>

// ---------- HC-SR04 pins ----------
const int TRIG_PIN = 9;
const int ECHO_PIN = 8;

// ---------- Servo pin ----------
const int SERVO_PIN = 10;

// ---------- L298N IN pins (final) ----------
const int IN1 = 7;
const int IN2 = 6;
const int IN3 = 5;
const int IN4 = 4;

// ---------- Thresholds (cm) ----------
const int CLEAR_DISTANCE_CM = 30;   // go forward if distance > 30cm
const int STOP_DISTANCE_CM  = 15;   // avoid if distance <= 15cm

// ---------- Timing (ms) ----------
const int SERVO_SETTLE_MS = 350;    // wait for servo movement + stable measurement
const int BACKUP_MS       = 250;    // reverse before turning
const int TURN_MS         = 650;    // turning time (calibrate for your chassis)
const int LOOP_DELAY_MS   = 40;     // small delay for stability

// ---------- Ultrasonic timeout (us) ----------
const unsigned long ECHO_TIMEOUT_US = 30000UL; // avoids blocking if no echo

// ---------- Servo angles ----------
const int SERVO_CENTER = 90;
const int SERVO_LEFT   = 20;
const int SERVO_RIGHT  = 160;

Servo scanner;

// ---------- Read distance in cm ----------
int readDistanceCm() {
  // Trigger pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Echo pulse width
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

  // If timeout -> invalid
  if (duration == 0) return -1;

  // Convert to cm
  return (int)(duration / 58.2);
}

// ---------- Motor helpers ----------
void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

void moveForward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void moveBackward() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
}

// Pivot left: left motor backward, right motor forward
void turnLeft() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

// Pivot right: left motor forward, right motor backward
void turnRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
}

void setup() {
  Serial.begin(9600);

  // Motor pins
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  // Ultrasonic pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Servo
  scanner.attach(SERVO_PIN);
  scanner.write(SERVO_CENTER);
  delay(SERVO_SETTLE_MS);

  stopMotors();
}

void loop() {
  // Measure front distance
  int front = readDistanceCm();
  Serial.print("Front: ");
  Serial.println(front);

  // Safety: if invalid measurement, stop and retry
  if (front < 0) {
    stopMotors();
    delay(LOOP_DELAY_MS);
    return;
  }

  // Clear path -> forward
  if (front > CLEAR_DISTANCE_CM) {
    scanner.write(SERVO_CENTER);
    moveForward();
    delay(LOOP_DELAY_MS);
    return;
  }

  // Obstacle close -> avoidance
  if (front <= STOP_DISTANCE_CM) {
    stopMotors();
    delay(120);

    // Look left
    scanner.write(SERVO_LEFT);
    delay(SERVO_SETTLE_MS);
    int left = readDistanceCm();
    Serial.print("Left: ");
    Serial.println(left);

    // Look right
    scanner.write(SERVO_RIGHT);
    delay(SERVO_SETTLE_MS);
    int right = readDistanceCm();
    Serial.print("Right: ");
    Serial.println(right);

    // Center again
    scanner.write(SERVO_CENTER);
    delay(150);

    // Convert invalid readings to 0 (treat as blocked/unknown)
    if (left < 0) left = 0;
    if (right < 0) right = 0;

    // Back up a bit
    moveBackward();
    delay(BACKUP_MS);
    stopMotors();
    delay(100);

    // Turn toward freer side
    if (left > right && left > CLEAR_DISTANCE_CM) {
      turnLeft();
      delay(TURN_MS);
    } else if (right >= left && right > CLEAR_DISTANCE_CM) {
      turnRight();
      delay(TURN_MS);
    } else {
      // If neither side is clearly open, do a longer left turn
      turnLeft();
      delay(TURN_MS + 250);
    }

    stopMotors();
    delay(150);
    return;
  }

  // Middle zone (15..30cm): conservative stop
  moveForward();
  delay(LOOP_DELAY_MS);
}