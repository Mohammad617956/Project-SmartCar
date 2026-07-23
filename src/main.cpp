#include <Arduino.h>
#include <vehicle.h>
#include <ultrasonic.h>
#include <ESP32Servo.h>

/*
myCar.Move(Foward,255);
myCar.Move(Backward, 255);
myCar.Move(Clockwise,255);
myCar.Move(Anticlockwise,255)
myCar.Move(Move-Left,255)
myCar.Move(Move_Right,255)
*/


vehicle myCar;
Servo myServo;


#define leftLED 2
#define rightLED 12
#define Anticlockwise Contrarotate
#define buzzer 33
#define servopin 25

void setup()
{
 Serial.begin(115200);
 pinMode(leftLED, OUTPUT);
 pinMode(rightLED, OUTPUT);

myCar.Init();
myServo.attach(servoPin);
myServo.write(0);

}

void loop()
{
digitalWrite(leftLED, HIGH);
digitalWrite(rightLED, LOW);
//myCar.Move(Move_Left, 255);
//tone(buzzer, 156);

delay(1000);

digitalWrite(leftLED, LOW);
digitalWrite(rightLED, HIGH);
//myCar.Move(Move_Right , 255);
//noTone(buzzer);
delay(1000);
}
