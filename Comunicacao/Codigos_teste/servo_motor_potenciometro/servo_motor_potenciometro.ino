#include <ESP32Servo.h>

// Define the servo control pin
#define SERVO1_PIN 12
#define POTENCIOMETER1_PIN 14
#define SERVO2_PIN 27
#define POTENCIOMETER2_PIN 26

Servo myServo1;
Servo myServo2;
int val = 1;
int num = 0;

void setup() {
  // Attach the servo to the specified pin
  myServo1.setPeriodHertz(50); // Set PWM frequency to 50Hz (standard for servos)
  myServo1.attach(SERVO1_PIN, 500, 3000); // (pin, min pulse width, max pulse width in microseconds)
  myServo2.setPeriodHertz(50); // Set PWM frequency to 50Hz (standard for servos)
  myServo2.attach(SERVO2_PIN, 500, 3000); // (pin, min pulse width, max pulse width in microseconds)
  
  Serial.begin(115200);
  Serial.println("Servo control initialized");

  // myServo1.write(0);

  delay(100);
}

void loop() {
  // myServo1.write(0);
  // delay(100);
  // if (val % 200 == 0 && val <= 100000) {
  //   Serial.println("O servo deveria mexer?");
  //   myServo1.write(num);

  //   num += 20;
  // }
  // if (val > 100000) {
  //   myServo1.attach(SERVO_PIN_TEMP);
  //   Serial.println("O servo deveria mexer?");
  // }
  // Serial.println("---------------------- Leitura do Servo ----------------------");
  // Serial.println(myServo1.read());
  
  // // Small delay to stabilize the servo and reduce Serial input checking frequency
  // val += 1;
  // if (val > 10000000) {
  //   Serial.println("Reseto Val?");
  //   myServo1.attach(SERVO_PIN, 500, 2400);
  //   val = 0;
  // }


  // int servoPosition1 = map(analogRead(POTENCIOMETER1_PIN), 0, 4096, 0, 180);




  // if (val % 2 == 0) {
  //   delay(5000);
  //   myServo1.write(0);
  //   delay(5000);
  //   myServo1.write(180);
  //   delay(5000);
  // }
  // else {
  //   delay(5000);
  //   myServo2.write(0);
  //   delay(5000);
  //   myServo2.write(180);
  //   delay(5000);
  // }

  myServo1.write(0);
  delay(5000);
  myServo1.write(180);
  delay(5000);

  // val += 1;

  // if (val > 5000) {
  //   val = 1;
  // }







  // Serial.println("Posicao servo 1");
  // Serial.println(servoPosition1);

  // int servoPosition2 = map(analogRead(POTENCIOMETER2_PIN), 0, 4096, 0, 180);
  // myServo2.write(servoPosition2);
  // Serial.println("Posicao servo 2");
  // Serial.println(servoPosition2);


  
}