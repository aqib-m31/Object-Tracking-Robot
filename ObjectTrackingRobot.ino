/*
  ObjectTrackingRobot.ino
  An autonomous object tracking robot using IR and ultrasonic sensors for real-time following. The robot detects nearby objects with an infrared sensor and maintains a safe following distance using ultrasonic distance measurement, automatically reversing when too close and stopping when the object is out of range.
*/
#include <NewPing.h>

// Pin Definitions
#define IR_FRONT 2  // Front IR sensor
#define TRIG_PIN 3  // Ultrasonic trigger
#define ECHO_PIN 4  // Ultrasonic echo

// Motor Driver Pins (L298N)
#define ENA 9   // Right motor speed (PWM)
#define IN1 8   // Right motor direction
#define IN2 7   // Right motor direction
#define ENB 10  // Left motor speed (PWM)
#define IN3 6   // Left motor direction
#define IN4 5   // Left motor direction

// Distance Settings
#define TOO_CLOSE 7
#define FOLLOW_DISTANCE 17
#define MAX_DISTANCE 500

// Speed Settings
#define MOTOR_SPEED 100

NewPing sonarFront(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

void forward(int speedLeft = MOTOR_SPEED, int speedRight = MOTOR_SPEED) {
  analogWrite(ENA, speedRight);
  analogWrite(ENB, speedLeft);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void reverse(int speedLeft = MOTOR_SPEED, int speedRight = MOTOR_SPEED) {
  analogWrite(ENA, speedRight);
  analogWrite(ENB, speedLeft);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void setup() {
  // IR Sensor
  pinMode(IR_FRONT, INPUT);

  // Ultrasonic
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Motors
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  int irFront = digitalRead(IR_FRONT);
  int distance = sonarFront.ping_cm();

  Serial.print("IR: ");
  Serial.print(irFront);
  Serial.print(" | Distance: ");
  Serial.print(distance);
  Serial.print(" cm | ");

  // IR detects object (LOW)
  if (irFront == LOW && distance >= 0) {
    if (distance <= TOO_CLOSE) {
      // Too close - reverse
      Serial.println("REVERSE");
      reverse(100, 100);
    } else if (distance <= FOLLOW_DISTANCE) {
      // Within follow range - move forward
      Serial.println("FORWARD");
      forward(120, 120);
    } else {
      // Object too far - stop
      Serial.println("STOP");
      stopMotors();
    }
  } else {
    // No object detected - stop
    Serial.println("STOP");
    stopMotors();
  }
}
