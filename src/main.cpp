#include <Arduino.h>
#include <vehicle.h>
#include <ESP32Servo.h>
#include <ultrasonic.h>
#include<IRremote.h>

vehicle myCar;
ultrasonic myUltrasonic;
int UT_distance = 0;
Servo myServo;
int closest_object = 1000;
int closest_object_direction = 0;
float ms_per_degree = 11.0;

int IRpin = 4;
unsigned long lastCommandTime; // Record the time of the last received command
const unsigned long commandTimeout = 100; // Set the timeout period (milliseconds)
uint32_t last_decode = 0; // Variable to store the previously decoded raw data
uint32_t current_decode = 0;// Variable to store the currently decoded raw data

#define leftLed 2
#define rightLed 12
#define Anticlockwise Contrarotate
#define buzzer 33
#define servoPin 25
#define IRpin 4 //Declare the pin of the infrared receiver
#define Left_sensor 35 //declare the pin of left tracking sensor
#define Right_sensor 39 //declare the pin of right tracking sensor

IRrecv myIRrecv(IRpin);//Creating an infrared receiver object
int Speed = 167;


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
  myIRrecv.enableIRIn();//Start receiving infrared
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

  // lights
  // pinMode(leftLed, OUTPUT);
  // pinMode(rightLed,OUTPUT);

  // eye sensor
  // Serial.begin(115200);
  // myUltrasonic.Init(13, 14);

  // tone(buzzer,262);

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

/*
myCar.Move(Forward, 255):
myCar.Move(Backward, 255):

myCar.Move(Clockwise, 255):
myCar.move(Contrarotate, 255)
myCar.Move(Anticlockwise, 255):

myCar.Move(Move_Left, 255):
myCar.Move(Move_Right, 255):
*/


void loop()
{
  
  //flash left then right
  digitalWrite(leftLed, HIGH);
  digitalWrite(rightLed, HIGH);

  delay(1000);

  digitalWrite(leftLed, LOW);
  digitalWrite(rightLed, LOW);

  delay(1000);
  

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

  if(myIRrecv.decode()) //Whether infrared is received
if (myIRrecv.decode()) { // Check if an IR signal is received
lastCommandTime = millis(); // Update the last command time
current_decode = myIRrecv.decodedIRData.decodedRawData;
if (myIRrecv.decodedIRData.flags) { // Check if it's a repeated IR code
current_decode = last_decode;
// Set current decode as the previous one
}
Serial.print(current_decode, HEX);
Serial.println("");
switch (current_decode) {
case 0xB946FF00: myCar.Move(Forward, Speed); break;
// Press "up" button to move forward
case 0xEA15FF00: myCar.Move(Backward, Speed); break;
// Press "down" button to move backward
case 0xBB44FF00: myCar.Move(Contrarotate, Speed); break;
// Press "left" button to turn left
case 0xBC43FF00: myCar.Move(Clockwise, Speed); break;
// Press "right" button to turn right
case 0xE916FF00: myCar.Move(Move_Left, Speed); break;
// Press button "1" to move left
case 0xF20DFF00: myCar.Move(Move_Right, Speed); break;
// Press button "3" to move right
case 0xBF40FF00:

}
last_decode = current_decode;
// Update the stored previous decodedRawData
myIRrecv.resume(); // Wait for the next IR signal
}
if (millis() - lastCommandTime > commandTimeout) {
myCar.Move(Stop, 0);
// If no new IR signal within 100 milliseconds, stop the smart car
}
}

/*
Forward B946FF00
Back 
Left 
Right

*/