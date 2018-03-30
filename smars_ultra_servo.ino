#include <Servo.h>
#include <AFMotor.h>
AF_DCMotor R_motor(1);           // defines Right motor connector
AF_DCMotor L_motor(2);           // defines Left motor connector
int trig = 18;                   // defines trigger pin ultrasonic
int echo = 19;                   // defines echo pin ultrasonic
int distancecm=0;
int duration;
int distanceStop=10;              // distanza massima prima di sondare altra direzione
//servo
Servo monServo;
const char PWM_SERVO = 9;         // defines pin servo 
int positionDuServo = 50;
int valoremaggiore=0;
int posdelvaloremaggio=0;
long valorePrec=0;
void setup() {
  Serial.begin(9600);           // sets up Serial library at 9600 bps  
  L_motor.setSpeed(130);        // sets L motor speed
  R_motor.setSpeed(200);        // sets R motor speed
  R_motor.run(RELEASE);         //turns L motor on
  L_motor.run(RELEASE);         //turns R motor on
  monServo.attach(PWM_SERVO);                                                             //Servo connecté sur la broche DIGITAL 9
  monServo.write(positionDuServo);                                                //Servo commandé dans sa position initiale
}
void loop() {
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);
  distancecm=mdistance();
  if(distancecm<distanceStop){
    L_motor.run(RELEASE);      //moves motor L Forward
    R_motor.run(RELEASE);      //moves motor L Forward
    delay(1000);
    for (positionDuServo = 50; positionDuServo <= 100; positionDuServo++) {
      monServo.write(positionDuServo);
      delay(50);
      distancecm=mdistance();
      if (valoremaggiore<distancecm){
        valoremaggiore=distancecm;
        posdelvaloremaggio=positionDuServo;
      }
      delay(20);
    }
    //Serial.print("valore maggiore50 ");
    //Serial.println(valoremaggiore);
    for (positionDuServo = 100; positionDuServo>= 0; positionDuServo--) {
      monServo.write (positionDuServo);
      delay(50);
      distancecm=mdistance();
      if (valoremaggiore<distancecm){
        valoremaggiore=distancecm;
        posdelvaloremaggio=positionDuServo;
      }
      delay(20);
    }
    for (positionDuServo = 0; positionDuServo<= 50; positionDuServo++) {
      monServo.write (positionDuServo);
      delay(50);
      distancecm=mdistance();
      if (valoremaggiore<distancecm){
        valoremaggiore=distancecm;
        posdelvaloremaggio=positionDuServo;
      }
      delay(20);
    }
    if (valoremaggiore<=distanceStop){
      if (posdelvaloremaggio>50){
        L_motor.run(RELEASE);
        R_motor.run(BACKWARD);
        delay(1000);
      }
      else{
        L_motor.run(BACKWARD);
        R_motor.run(RELEASE);
        delay(1000);
      }
    }
    if (posdelvaloremaggio>50){
      L_motor.run(RELEASE);
      R_motor.run(FORWARD);
      delay(500);
      L_motor.run(FORWARD);
      R_motor.run(FORWARD);
      delay(1000);
    }
    else if(posdelvaloremaggio<50){
      L_motor.run(FORWARD);
      R_motor.run(RELEASE);
      delay(500);
      L_motor.run(FORWARD);
      R_motor.run(FORWARD);
      delay(1000);
    }
    else {
      L_motor.run(FORWARD);
      R_motor.run(FORWARD);
      delay(1000);
    }
  }
  else{
     L_motor.run(FORWARD); 
     R_motor.run(FORWARD); 
  }
  valoremaggiore=0;
}
int mdistance(){              //funzione controllo distanza
  float distance =0;
  long time_value=0;
  digitalWrite(trig,LOW);
  //delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  time_value=pulseIn(echo,HIGH,15000);
  if (time_value > 38000 || time_value == 0){
    //mdistance();
    time_value=valorePrec;
  }
  else {
    valorePrec=time_value;
  }
  distance=0.033*time_value/2;
  Serial.print("val ");
  Serial.println(distance);
  return distance;
}
