#include <Pixy2.h>
#include <Servo.h>

//PINS
int recycleLEDPin = A0;
int garbageLEDPin = A1;
int compostLEDPin = A2;

int recycleTrigPin=7;
int recycleEchoPin=8;
int garbageTrigPin=A3;
int garbageEchoPin=A4;
int compostTrigPin=5;
int compostEchoPin=6;

int doorFrontServoPin = 12;
int doorBackServoPin = 11;
Servo doorFrontServo;
Servo doorBackServo;

int rightServoPin = 10;
int leftServoPin = 9;
Servo rightServo;
Servo leftServo;

// This is the main Pixy object 
Pixy2 pixy;

struct sonarPins{
  int trigPin;
  int echoPin;
};

class bin{
  public:
    bin(String binName, int ledPin, int tPin, int ePin){
      this->binName = binName;
      binLEDPin = ledPin;
      distanceSensor.trigPin = tPin;
      distanceSensor.echoPin = ePin;
      pinMode(ledPin,OUTPUT);
      pinMode(tPin,OUTPUT);
      pinMode(ePin,INPUT);
    }
    void checkStatus(){
      digitalWrite(distanceSensor.trigPin,LOW);
      delayMicroseconds(5);
      digitalWrite(distanceSensor.trigPin,HIGH);
      delayMicroseconds(10);
      digitalWrite(distanceSensor.trigPin,LOW);
      pinMode(distanceSensor.echoPin,INPUT);
      long duration=pulseIn(distanceSensor.echoPin,HIGH);
      long cm=(duration/2)/29.1;
      if(cm<=10){
        digitalWrite(binLEDPin,HIGH);
      }
      else{
        digitalWrite(binLEDPin,LOW);
      }
    }
  private:
    String binName = "N/A";
    int binLEDPin;
    sonarPins distanceSensor;
};

bool releaseItem(String dropAngle){
  dropAngleSet(dropAngle);
  //Open
  doorFrontServo.attach(doorFrontServoPin);
  doorBackServo.attach(doorBackServoPin);
  doorFrontServo.write(180);
  doorBackServo.write(180);
  delay(2000);
  //Close
  doorFrontServo.write(107);
  doorBackServo.write(115);
  delay(1000);
  doorFrontServo.detach();
  doorBackServo.detach();
  
  rightServo.attach(rightServoPin);
  leftServo.attach(leftServoPin);
  delay(500);
  rightServo.write(55);
  leftServo.write(110);
  delay(500);
  rightServo.detach();
  leftServo.detach();
  return true;
}

String currentDropAngle = "Garbage";
bool dropAngleSet(String newDropAngle){
  if(newDropAngle != currentDropAngle){
    rightServo.attach(rightServoPin);
    leftServo.attach(leftServoPin);
    if(newDropAngle == "Recycle"){
      rightServo.write(10);
      leftServo.write(60);
    }
    else if(newDropAngle == "Compost"){
      rightServo.write(90);
      leftServo.write(170);
    }
    else if(newDropAngle == "Garbage"){
      rightServo.write(55);
      leftServo.write(110);
    }
  }
  delay(1000);
  rightServo.detach();
  leftServo.detach();
  return true;
}

bin recycleBin("Recycle", recycleLEDPin, recycleTrigPin, recycleEchoPin);
bin garbageBin("Garbage", garbageLEDPin, garbageTrigPin, garbageEchoPin);
bin compostBin("Compost", compostLEDPin, compostTrigPin, compostEchoPin);

void detectObject(){
  int i; 
  pixy.ccc.getBlocks();
  if (pixy.ccc.numBlocks)
  {
    delay(1000);
    Serial.print("Detected: ");
    if(pixy.ccc.blocks[i].m_signature == 1 || pixy.ccc.blocks[i].m_signature == 4){
      Serial.print("Compost");
      delay(2000);
      releaseItem("Compost");
    }
    else if(pixy.ccc.blocks[i].m_signature == 2){
      Serial.print("Recycle");
      delay(2000);
      releaseItem("Recycle");
    }
    else if(pixy.ccc.blocks[i].m_signature == 3){
      Serial.print("Garbage");
      delay(2000);
      releaseItem("Garbage");
    }
    Serial.println("");
  }  
  delay(250);
}

void setup()
{
  Serial.begin(115200);
  rightServo.attach(rightServoPin);
  leftServo.attach(leftServoPin);
  
  doorFrontServo.attach(doorFrontServoPin);
  doorBackServo.attach(doorBackServoPin);
  pixy.init();
  rightServo.write(55);
  leftServo.write(110);
      
  doorFrontServo.write(107);
  doorBackServo.write(115);
  
  rightServo.detach();
  leftServo.detach();
  doorFrontServo.detach();
  doorBackServo.detach();
}

void loop()
{ 
  detectObject();
  delay(4000);
  //releaseItem("Recycle");
  //delay(10000);
  //releaseItem("Garbage");
  //delay(10000);
  //releaseItem("Compost");

  //doorFrontServo.write(107);
  //doorBackServo.write(115);
}
