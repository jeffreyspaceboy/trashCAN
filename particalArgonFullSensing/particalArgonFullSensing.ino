int recycleLEDPin = A2;
int garbageLEDPin = A1;
int compostLEDPin = A0;

int recycleTrigPin=D6;
int recycleEchoPin=D7;
int garbageTrigPin=A3;
int garbageEchoPin=A4;
int compostTrigPin=D2;
int compostEchoPin=D3;

int compostFull = 0;
int recycleFull = 0;

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
      pinMode(binLEDPin,OUTPUT);
      pinMode(tPin,OUTPUT);
      pinMode(ePin,INPUT);
      digitalWrite(binLEDPin, LOW);
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
        isFull = 1;
      }
      else{
        digitalWrite(binLEDPin,LOW);
        isFull = 0;
      }
    }
    int getIsFull(){
        return isFull;
    }
  private:
    String binName = "N/A";
    int binLEDPin;
    sonarPins distanceSensor;
    int isFull = 0;
};

bin recycleBin("Recycle", recycleLEDPin, recycleTrigPin, recycleEchoPin);
bin garbageBin("Garbage", garbageLEDPin, garbageTrigPin, garbageEchoPin);
bin compostBin("Compost", compostLEDPin, compostTrigPin, compostEchoPin);



void setup() {
    Particle.variable("compostFull", compostFull);
    Particle.variable("recycleFull", recycleFull);
    delay(10);
    
}

void loop() {
    compostBin.checkStatus();
    recycleBin.checkStatus();
    compostFull = compostBin.getIsFull();
    recycleFull = recycleBin.getIsFull();
    delay(100);
}
