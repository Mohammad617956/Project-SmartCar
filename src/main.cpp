#include <Arduino.h>
#include <vehicle.h>
#include <ESP32Servo.h>
#include <ultrasonic.h>

vehicle myCar;
ultrasonic myUltrasonic;
int UT_distance = 0;
Servo myServo;
int closest_object = 1000;
int closest_object_direction = 0;
float ms_per_degree = 11.0;

#define leftLed 2
#define rightLed 12
#define Anticlockwise Contrarotate
#define buzzer 33
#define servoPin 25

/*
myCar.Move(Forward, 255):
myCar.Move(Backward, 255):

myCar.Move(Clockwise, 255):
myCar.move(Contrarotate, 255)
myCar.Move(Anticlockwise, 255):

myCar.Move(Move_Left, 255):
myCar.Move(Move_Right, 255):
*/

void turn_left(int degrees)
{
  myCar.Move(Anticlockwise, 255);
  delay(degrees * ms_per_degree);
  myCar.Move(Stop, 0);
}

void turn_right(int degrees)
{
  myCar.Move(Clockwise, 255);
  delay(degrees * ms_per_degree);
  myCar.Move(Stop, 0);
}

void setup()
{
  Serial.begin(115200);
  myCar.Init();
  myUltrasonic.Init(13, 14);
  /*
 myCar.Move(Forward,255);
 delay(2000);
 myCar.Move(Clockwise,255);
 delay(2000);
 myCar.Move(Forward,255);
 delay(2000);
 myCar.Move(Clockwise,255);
 delay(2000);
 myCar.Move(Forward,255);
 delay(2000);
 myCar.Move(Contrarotate, 255);
 delay(2000);
 myCar.Move(Forward,255);
 delay(2000);
 myCar.Move(Contrarotate, 255);
 delay(2000);
 myCar.Move(Forward,255);
 delay(2000);
 myCar.Move(Move_Right,255);
 delay(2000);
 myCar.Move(Stop,0);
 */

  // lights
  // pinMode(leftLed, OUTPUT);
  // pinMode(rightLed,OUTPUT);

  // eye sensor
  // Serial.begin(115200);
  // myUltrasonic.Init(13, 14);

  // tone(buzzer,262);

  myServo.attach(servoPin); // initialize servo motor)
  myServo.write(0);
  // eye sensor v2 - best route
  for (int angle = 0; angle <= 180; angle++)
  {
    // Servo motor from 0 degress to 180 degrees
    myServo.write(angle);
    // Serial.println(angle);
    delay(10);
    UT_distance = myUltrasonic.Ranging();
    if (UT_distance < closest_object)
    {
      closest_object = UT_distance;
      closest_object_direction = angle;
    }
  }

  myServo.write(90);
  int relative_object_angle = closest_object_direction - 90;
  int required_turn_degrees = 180 - closest_object_direction;
  Serial.print("Closest Object Direction: ");
  Serial.print(closest_object_direction);
  Serial.print(", ");
  Serial.print("Relative Object Angle: ");
  Serial.print(relative_object_angle);
  Serial.print(", ");
  Serial.print("Required Turn Degress: ");
  Serial.println(required_turn_degrees);
  if (required_turn_degrees > 0)
  {
    turn_left(required_turn_degrees);
  }
  else if (required_turn_degrees < 0)
  {
    turn_right(abs(required_turn_degrees));
  }
}

void loop()
{
  /*
  //flash left then right
  digitalWrite(leftLed, HIGH);
  digitalWrite(rightLed, LOW);

  delay(1000);

  digitalWrite(leftLed, LOW);
  digitalWrite(rightLed, HIGH);

  delay(1000);
  */

  /*Eye sensor v1
  UT_distance = myUltrasonic.Ranging();
  if (UT_distance <= 25)
  {
    myCar.Move(Anticlockwise, 180);
    delay(1500); // The turning time is moified to realize the rotation of different angles
    myCar.Move(Stop, 0);
  }
  else
  {
    myCar.Move(Forward, 150);
  }

  Serial.print(UT_distance);
  // The serial port shows the distance of ultrasonic detection
  Serial.println("cm");
  delay(100);
  */

  //   for(int angle =180;angle >= 0;angle--){
  //     myServo.write(angle);
  //     delay(10);
  // }
}