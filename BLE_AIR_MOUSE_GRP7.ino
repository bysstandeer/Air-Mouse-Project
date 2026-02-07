// BSCPE 3-5 GRP 7 AIR MOUSE
// Libraries Used
#include <BleMouse.h>
#include <Adafruit_MPU6050.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>

// Defined Pin Buttons and Speed/Sensitivity
#define LEFTBUTTON 27
#define RIGHTBUTTON 13
#define UPBUTTON 25
#define DOWNBUTTON 32
#define SPEED 5

// Objects Created
Adafruit_MPU6050 mpu;
BleMouse bleMouse("BLE AIR MOUSE GRP7");

// Variables to Disable MPU6050
bool sleepMPU = true;
long mpuDelayMillis;

void setup() {
  Serial.begin(115200);

  // Pin Buttons
  pinMode(LEFTBUTTON, INPUT_PULLUP);
  pinMode(RIGHTBUTTON, INPUT_PULLUP);
  pinMode(UPBUTTON, INPUT_PULLUP);
  pinMode(DOWNBUTTON, INPUT_PULLUP);

  // Boot BLE
  bleMouse.begin();
  delay(1000);
 
  // Boot MPU6050 Gyroscope
  if (!mpu.begin()) {
    Serial.println("MPU6050 Not Found!");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  // Disable MPU6050 until BT is Connected
  mpu.enableSleep(sleepMPU);
}

void loop() {
  if (bleMouse.isConnected()) {
    // Enable MPU6050 after BT Connection
    if (sleepMPU) {
      delay(3000);
      Serial.println("MPU6050 awakened!");
      sleepMPU = false;
      mpu.enableSleep(sleepMPU);
      delay(500);
    }

    // MPU6050 Movement
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // Gyroscope Movement Sensor
    bleMouse.move(g.gyro.x * -SPEED, g.gyro.y * -SPEED);

    // Left Mouse Button (LMB)
    if (!digitalRead(LEFTBUTTON)) {
      Serial.println("Left click");
      bleMouse.click(MOUSE_LEFT);
      delay(100);
    }

    // Right Mouse Button (RMB)
    if (!digitalRead(RIGHTBUTTON)) {
      Serial.println("Right click");
      bleMouse.click(MOUSE_RIGHT);
      delay(100);
    }

    // Scroll Up
    if (!digitalRead(UPBUTTON)) {
      Serial.println("Scroll up");
      bleMouse.move(0, 0, 1);
      delay(200);
    }

    // Scroll Down
    if (!digitalRead(DOWNBUTTON)) {
      Serial.println("Scroll down");
      bleMouse.move(0, 0, -1);
      delay(200);
    }
  }
}