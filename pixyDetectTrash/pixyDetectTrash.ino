#include <Pixy2.h>
#include <Servo.h>

//PINS



int recycleLEDPin = A0;
int garbageLEDPin = A1;
int compostLEDPin = A2;


int compostTrigPin=5;
int compostEchoPin=6;
int recycleTrigPin=7;
int recycleEchoPin=8;
int garbageTrigPin=A3;
int garbageEchoPin=A4;

int doorFrontServoPin = 12;
int doorBackServoPin = 11;
Servo doorFrontServo;
Servo doorBackServo;


int rightServoPin = 10;
int leftServoPin = 9;
Servo rightServo;
Servo leftServo;




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

// This is the main Pixy object 
Pixy2 pixy;

bool releaseItem(String dropAngle){
  dropAngleSet(dropAngle);
  //Open
  doorFrontServo.write(20);
  doorBackServo.write(60);
  delay(1000);
  //Close
  doorFrontServo.write(20);
  doorBackServo.write(60);
}


String currentDropAngle = "Garbage";
bool dropAngleSet(String newDropAngle){
  if(newDropAngle != currentDropAngle){
    if(newDropAngle == "Recycle"){
      rightServo.write(20);
      leftServo.write(60);
    }
    else if(newDropAngle == "Compost"){
      rightServo.write(90);
      leftServo.write(150);
    }
    else if(newDropAngle == "Garbage"){
      rightServo.write(55);
      leftServo.write(110);
    }
  }
  delay(500);
  return true;
}


bin recycleBin("Recycle", int ledPin, int tPin, int ePin);
bin garbageBin("Garbage", int ledPin, int tPin, int ePin);
bin compostBin("Compost", int ledPin, int tPin, int ePin);

void setup()
{
  Serial.begin(115200);
  rightServo.attach(rightServoPin);
  leftServo.attach(leftServoPin);
  
  doorFrontServo.attach(doorFrontServoPin);
  doorBackServo.attach(doorBackServoPin);
  pixy.init();
}

void loop()
{ 
  int i; 
  // grab blocks!
  pixy.ccc.getBlocks();
  
  // If there are detect blocks, print them!
  if (pixy.ccc.numBlocks)
  {
    Serial.print("Detected: ");
    if(pixy.ccc.blocks[i].m_signature == 1){
      Serial.print("Compost");
      servo.write(0);
    }
    else if(pixy.ccc.blocks[i].m_signature == 2){
      Serial.print("Recycle");
      servo.write(90);
    }
    else{
      Serial.print("Garbage");
      servo.write(180);
    }
    Serial.println("");
  }  
  delay(250);
}
