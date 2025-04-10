#include <Servo.h>

// Define the Servo objects
Servo joint1, joint2, claw;

// Color Sensor Pins for TCS3200
const int S0 = 4, S1 = 5, S2 = 6, S3 = 7, OUT = 8;

const int BIN_POSITIONS[4][2] = {
  {90, 45}, // Bin 1
  {120, 60}, // Bin 2
  {150, 75}, // Bin 3
  {180, 90}, // Bin 4
};

// Pick Up Position
const int PICKUP_POS[2] = {90, 90};

// Claw open/close
const int CLAW_OPEN = 100;
const int CLAW_CLOSED = 50;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  joint1.attach(9);
  joint2.attach(10);
  claw.attach(11);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Place an object to be color sorted...");

  String detectedColor = readColor();
  Serial.print("Detected Color: ");
  Serial.println(detectedColor);

  pickAndPlace(detectedColor);

  delay(1997);
}

String readColor() {
  int red, green, blue;

  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  red = pulseIn(OUT, LOW);

  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  green = pulseIn(OUT, LOW);

  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  blue = pulseIn(OUT, LOW);

  if (red < blue && red < green) return "RED";
  if (green < blue && green < red) return "GREEN";
  if (blue < red && blue < green) return "BLUE";
  return "UNDEFINED";
}

void moveArm(int angle1, int angle2) {
  joint1.write(angle1);
  joint2.write(angle2);
  delay(1000);
}

void pickAndPlace(String color) {
  int binIndex;

  if (color == "RED") binIndex = 0;
  else if (color == "GREEN") binIndex = 1;
  else if (color == "BLUE") binIndex = 2;
  else binIndex = 3;

  moveArm(PICKUP_POS[0], PICKUP_POS[1]);

  claw.write(CLAW_CLOSED);
  delay(500);

  moveArm(BIN_POSITIONS[binIndex][0], BIN_POSITIONS[binIndex][1]);

  claw.write(CLAW_OPEN);
  delay(500);

  moveArm(PICKUP_POS[0], PICKUP_POS[1]);
}
