#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver idektep = Adafruit_PWMServoDriver(0x40);

// PWM signal boundaries for servo motors
#define SERVOMIN  100
#define SERVOMAX  600

int count = 0;              // Variable to count how many times the function runs
const int maxCount = 10;    // Set the desired number of repetitions

// Define servo numbers
#define servo1 0  // Base
#define servo2 1  // Elbow
#define servo3 2  // Lift
#define servo4 3  // Wrist
#define servo5 4  // Gripper

// Function to convert angle to PWM pulse
int angleToPulse(int ang) {
  int pulse = map(ang, 0, 180, SERVOMIN, SERVOMAX);
  return pulse;
}

void setup() {
  Serial.begin(115200);
  idektep.begin();
  idektep.setPWMFreq(60); // Set PWM frequency to 60Hz

  // Move the robot to home position
  moveToHome();
}

void loop() {
  if (count < maxCount) {
    pickAndPlace();   // Execute the pick and place function
    count++;          // Increase the counter
    delay(3000);      // Wait 3 seconds before repeating
  }
  else {
  Serial.println("Task completed. Stopped after 10 cycles.");
  while (1); // Stop the loop completely
}
}

// Function to move the robot to its home position
void moveToHome() {
  idektep.setPWM(servo1, 0, angleToPulse(90));  // Center base
  idektep.setPWM(servo2, 0, angleToPulse(90));  // Fold elbow
  idektep.setPWM(servo3, 0, angleToPulse(90));  // Middle lift
  idektep.setPWM(servo4, 0, angleToPulse(90));  // Straight wrist
  idektep.setPWM(servo5, 0, angleToPulse(90));  // Open gripper
  delay(1000);
}

// Function for picking up and placing an object
void pickAndPlace() {
  Serial.println("Moving to pick position...");
  // Move to the object position
  idektep.setPWM(servo1, 0, angleToPulse(45));   // Rotate base to the left
  delay(800);
  idektep.setPWM(servo2, 0, angleToPulse(80));  // Extend elbow
  idektep.setPWM(servo3, 0, angleToPulse(120));  // Lower the arm
  idektep.setPWM(servo4, 0, angleToPulse(80));   // Wrist 
  idektep.setPWM(servo5, 0, angleToPulse(130));   // Open gripper
  delay(1500);

  Serial.println("Gripping object...");
  // Grab the object
  idektep.setPWM(servo5, 0, angleToPulse(15));  // Close gripper
  delay(800);

  Serial.println("Lifting object...");
  // Lift the object
  idektep.setPWM(servo3, 0, angleToPulse(70));   // Raise the arm
  delay(1000);

  Serial.println("Moving to place position...");
  // Move base to target position
  idektep.setPWM(servo1, 0, angleToPulse(150));  // Rotate base to the right
  delay(1000);

  Serial.println("Placing object...");
  // Release the object
  idektep.setPWM(servo3, 0, angleToPulse(120));  // Lower the arm
  delay(800);
  idektep.setPWM(servo5, 0, angleToPulse(90));   // Open gripper
  delay(800);

  Serial.println("Returning ");
  // Raise the arm
  idektep.setPWM(servo3, 0, angleToPulse(70));   // Raise the arm
  delay(100);
}
