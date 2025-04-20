#include <Servo.h>

// Define the Servo objects
Servo joint1, joint2, claw, swivel;

// Color Sensor Pins for TCS3200
const int S0 = 4, S1 = 7, S2 = 8, S3 = 12, OUT = 13;

const int BIN_POSITIONS[4][2] = {
  {90, 45}, // Bin 1
  {120, 60}, // Bin 2
  {150, 75}, // Bin 3
  {180, 90}, // Bin 4
};

// 
const int SWIVEL_PICKUP = 90;
const int SWIVEL_BIN_POSITIONS[4] = {
	30,
	60,
	120,
	150
};

// Pick Up Position
const int PICKUP_POS[2] = {90, 90};

// Claw open/close
const int CLAW_OPEN = 100;
const int CLAW_CLOSED = 50;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  joint1.attach(3);
  joint2.attach(5);
  claw.attach(6);
  swivel.attach(10);

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

  sorting(detectedColor);

  delay(4000);
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
  	else if (green < blue && green < red) return "GREEN";
  	else if (blue < red && blue < green) return "BLUE";
  	else return "UNDEFINED";
}

void moveArm(int angle1, int angle2, int swivelAngle) {
  joint1.write(angle1);
  joint2.write(angle2);
  swivel.write(swivelAngle);
  delay(1997);
}

void sorting(String color) {
  int binIndex;

  if (color == "RED") binIndex = 0;
  else if (color == "GREEN") binIndex = 1;
  else if (color == "BLUE") binIndex = 2;
  else binIndex = 3;

  moveArm(PICKUP_POS[0], PICKUP_POS[1], SWIVEL_PICKUP);

  claw.write(CLAW_CLOSED);
  delay(1997);

  moveArm(BIN_POSITIONS[binIndex][0], BIN_POSITIONS[binIndex][1], SWIVEL_BIN_POSITIONS[binIndex]);

  claw.write(CLAW_OPEN);
  delay(1997);

  moveArm(PICKUP_POS[0], PICKUP_POS[1], SWIVEL_PICKUP);
}