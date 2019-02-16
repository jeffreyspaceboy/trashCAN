#include <Pixy2.h>
#include <Servo.h>
Servo servo;
int angle=60;

// This is the main Pixy object 
Pixy2 pixy;

void setup()
{
  Serial.begin(115200);
  Serial.print("Starting...\n");
  servo.attach(9);
  servo.write(0);
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
