#include <Servo.h>

Servo swivel, joint2, claw;

const int S0 = 4, S1 = 7, S2 = 8, S3 = 12, OUT = 13;

const int SWIVEL_PICKUP = 0;

const int SWIVEL_BIN_POSITIONS[4] {
  20,
  40,
  60,
  80,
};

const int PICKUP_POS[2] = {0, 30};

const int CLAW_OPEN = 100;
const int CLAW_CLOSED = 50;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  swivel.attach(10);
  joint2.attach(5);
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
  String detectedColor = readColor();
  sorting(detectedColor);
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
  	else if (green < blue && green < red) return "GREEN";
  	else if (blue < red && blue < green) return "BLUE";
  	else return "UNDEFINED";
}

void moveArm(int angle2, int swivelAngle) {
  joint2.write(angle2);
  swivel.write(swivelAngle);
  delay(1000);
}

void sorting(String color) {
  int binIndex;

  Serial.println(color);
  if (color == "RED") {
    binIndex = 0;
  } else if (color == "GREEN") {
    binIndex = 1;
  } else if (color == "BLUE") {
    binIndex = 2;
  } else {
    binIndex = 3;
  }
// starting position
  moveArm(PICKUP_POS[0], SWIVEL_PICKUP);
  claw.write(CLAW_CLOSED);
  delay(1000);
  // lifts arm
  moveArm(PICKUP_POS[1], SWIVEL_PICKUP);
  // swivels to bin
  moveArm(PICKUP_POS[1], SWIVEL_BIN_POSITIONS[binIndex]);
  // lowers arm
  moveArm(PICKUP_POS[0], SWIVEL_BIN_POSITIONS[binIndex]);
  claw.write(CLAW_OPEN);
  delay(1000);
  // raises arm
  moveArm(PICKUP_POS[1], SWIVEL_BIN_POSITIONS[binIndex]);
  // returns to starting position
  moveArm(PICKUP_POS[1], SWIVEL_PICKUP);
}
