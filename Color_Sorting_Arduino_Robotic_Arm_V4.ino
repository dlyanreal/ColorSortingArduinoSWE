#include <Servo.h>

// Define the Servo objects
Servo joint1, joint2, claw, swivel;

// Color Sensor Pins for TCS3200
// S2 & S3 measure the color
// S0 & S1 affect intensity of output
const int S0 = 4, S1 = 7, S2 = 8, S3 = 12, OUT = 13;

int red_count = 0, green_count = 0, blue_count = 0, undefined_count = 0;

int BIN_POSITIONS[4][2] = {
  {90, 80}, // Bin 1
  {90, 80}, // Bin 2
  {90, 80}, // Bin 3
  {90, 80}, // Bin 4
};

// 
const int SWIVEL_PICKUP = 90;
const int SWIVEL_BIN_POSITIONS[4] = {
	30,
	80,
	130,
	180,
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

void moveArm(int angle1, int angle2, int swivelAngle) {
  joint1.write(angle1);
  joint2.write(angle2);
  swivel.write(swivelAngle);
  delay(1997);
}

void sorting(String color) {
  int binIndex;
  int joint1_target = PICKUP_POS[0];
  int joint2_target = PICKUP_POS[1];

// sort colors into appropriate bins, progressively closer to the base of the arm
  if (color == "RED") {
    if (red_count < 13) {
      binIndex = 0;
      red_count +=1;
      joint1_target = 90 - (red_count*5);
      joint2_target = 80 + (red_count*5);
    } else {
      Serial.println("RED Color Bin is full!");
      return;
    }
  } else if (color == "GREEN") {
    if (green_count < 13) {
      binIndex = 1;
      green_count += 1;
      joint1_target = 90 - (green_count*5);
      joint2_target = 80 - (green_count*5);
    } else {
      Serial.println("GREEN Color Bin is full!");
      return;
    }
  } else if (color == "BLUE") {
    if (blue_count < 13) {
      binIndex = 2;
      blue_count += 1;
      joint1_target = 90 - (blue_count*5);
      joint2_target = 80 - (blue_count*5);
    } else {
      Serial.println("BLUE Color Bin is full!");
      return;
    }
  } else {
    if (undefined_count < 13) {
      binIndex = 3;
      undefined_count += 1;
      joint1_target = 90 - (undefined_count*5);
      joint2_target = 80 - (undefined_count*5);
    } else {
      Serial.println("UNDEFINED Color Bin is full!");
      return;
    }
  }

  moveArm(PICKUP_POS[0], PICKUP_POS[1], SWIVEL_PICKUP);

  claw.write(CLAW_CLOSED);
  delay(1000);

  moveArm(joint1_target, joint2_target, SWIVEL_BIN_POSITIONS[binIndex]);

  claw.write(CLAW_OPEN);
  delay(1997);

  moveArm(PICKUP_POS[0], PICKUP_POS[1], SWIVEL_PICKUP);
}