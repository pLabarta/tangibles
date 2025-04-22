const int pinRojo = 23;
const int pinVer = 22;
const int pinAzu = 1;

const int pinSensorTrig = 3; // Trigger pin of the proximity sensor
const int pinSensorEcho = 21; // Echo pin of the proximity sensor
const int pinSensorTrig2 = 19; // Trigger pin of the second proximity sensor
const int pinSensorEcho2 = 18; // Echo pin of the second proximity sensor

#define ROLLING_AVG_SIZE 5

int distanceReadings[ROLLING_AVG_SIZE] = {0};
int currentIndex = 0;

long duration;
int distance;

void setup() {
  pinMode(pinRojo, OUTPUT);
  pinMode(pinVer, OUTPUT);
  pinMode(pinAzu, OUTPUT);
  pinMode(pinSensorTrig, OUTPUT);
  pinMode(pinSensorEcho, INPUT);
  pinMode(pinSensorTrig2, OUTPUT);
  pinMode(pinSensorEcho2, INPUT);
  distance = 0;
}

void loop() {
  // Send a 10-microsecond pulse to the first trigger pin
  digitalWrite(pinSensorTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinSensorTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinSensorTrig, LOW);

  // Measure the duration of the echo pulse for the first sensor
  long duration1 = pulseIn(pinSensorEcho, HIGH);

  // Send a 10-microsecond pulse to the second trigger pin
  digitalWrite(pinSensorTrig2, LOW);
  delayMicroseconds(2);
  digitalWrite(pinSensorTrig2, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinSensorTrig2, LOW);

  // Measure the duration of the echo pulse for the second sensor
  long duration2 = pulseIn(pinSensorEcho2, HIGH);

  // Convert the durations to distances (in cm)
  int distance1 = duration1 * 0.034 / 2;
  int distance2 = duration2 * 0.034 / 2;

  // Average the distances from both sensors
  int currentDistance = (distance1 + distance2) / 2;

  // Update the rolling average array
  distanceReadings[currentIndex] = currentDistance;
  currentIndex = (currentIndex + 1) % ROLLING_AVG_SIZE;

  // Calculate the rolling average
  int rollingSum = 0;
  for (int i = 0; i < ROLLING_AVG_SIZE; i++) {
    rollingSum += distanceReadings[i];
  }
  distance = rollingSum / ROLLING_AVG_SIZE;

  // Determine which LED to light up based on distance thresholds
  if (distance <= 45) { // Close
    digitalWrite(pinRojo, HIGH);
    digitalWrite(pinVer, LOW);
    digitalWrite(pinAzu, LOW);
  } else if (distance > 45 && distance <= 120) { // Near
    digitalWrite(pinRojo, LOW);
    digitalWrite(pinVer, HIGH);
    digitalWrite(pinAzu, LOW);
  } else if (distance > 120 && distance <= 375) { // Far away
    digitalWrite(pinRojo, LOW);
    digitalWrite(pinVer, LOW);
    digitalWrite(pinAzu, HIGH);
  } else { // Very far (more than 375 cm)
    digitalWrite(pinRojo, HIGH);
    digitalWrite(pinVer, HIGH);
    digitalWrite(pinAzu, HIGH);
  }

  delay(50); // Small delay for stability
}