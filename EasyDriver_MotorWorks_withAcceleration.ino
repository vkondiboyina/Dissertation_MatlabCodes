// Example3 code for Brian Schmalz's Easy Driver Example page
// http://www.schmalzhaus.com/EasyDriver/EasyDriverExamples.html

#include <AccelStepper.h>

// Define a stepper and the pins it will use
AccelStepper stepper(1, 2, 7);
//Declare pin functions on Redboard
#define stp 2
#define dir 7
#define MS1 4
#define MS2 8
#define EN  5


//int pos = 1000;
long post;


//Declare variables for functions
char user_input;
int x;
int y;
int state;
char cycle;

void setup()
{  
  stepper.setMaxSpeed(10);
  stepper.setAcceleration(500);
  Serial.begin(9600);
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(EN, OUTPUT);
  resetEDPins(); //Set step, direction, microstep and enable pins to default states
  
  Serial.println("Begin motor control");
  Serial.println();
  //Print function list for user selection
  Serial.println("Enter number for control option:");
  Serial.println("1. Reverse direction at default microstep mode.");
  Serial.println("2. Forward direction at default microstep mode.");
  Serial.println("3. Forward direction at Smooth mode.");
  Serial.println("4. Slow Ramp 10 mins");
  Serial.println("5. Fast ramp 5 secs");
  Serial.println("6. 1Hz cycle");
  Serial.println("7. 0.1 Hz Cycle");
  Serial.println();
}

void loop()
{   
  
   while(Serial.available()){
      user_input = Serial.read(); //Read user input and trigger appropriate function
     // cycle= Serial.read();
      digitalWrite(EN, LOW); //Pull enable pin low to allow motor control
      if (user_input =='1')
      {
         StepReverseCoarse();
      }
      else if(user_input =='2')
      {
        Serial.println("reverse");
        StepForwardCoarse();
      }
            else if(user_input =='3')
      {
        Serial.println("reverse");
        StepForwardSmooth();
      }
      else if(user_input =='4')
      {
        SlowRampFunction();
      }
      else if(user_input =='5')
      {
        RampFunction();
      }
      else if(user_input =='6')
      {
        OneHzFunction();
      }
      else if(user_input =='7')
      {
        PointOneHzFunction();
      }
      else
      {
        Serial.println("Invalid option entered.");
      }
   }
   resetEDPins();
}

//Reset Easy Driver pins to default states
void resetEDPins()
{
  digitalWrite(stp, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(EN, HIGH);
}

//Default reverse mode function
void StepReverseCoarse()
{
  Serial.println("Moving back at default step mode.");
  digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
  for(x= 0; x<100; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
   }
  Serial.println("Enter new option");
  Serial.println();
}

//Forward default  mode function
void StepForwardCoarse()
{
  Serial.println("Moving forward at default step mode.");
  digitalWrite(dir, HIGH); //Pull direction pin high to move in "reverse"
  for(x= 0; x<100; x++)  //Loop the stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
   Serial.println("Enter new option");
  Serial.println();
}

//Forward Smooth default  mode function
void StepForwardSmooth()
{
  Serial.println("Moving forward 5 steps");
  digitalWrite(dir, HIGH); //Pull direction pin high to move in "reverse"
  for(x= 0; x<1; x++)  //Loop the stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step
    delay(100);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(100);
  }
   Serial.println("Enter new option");
  Serial.println();
}

//// 1/8th microstep foward mode function
//void SmallStepMode()
//{
//  //Serial.println(digitalRead(MS1)); //// Is this moving at full step or 1/8 step?////
//  Serial.println("Alternate between stepping forward and reverse.");
//  stepper.setMaxSpeed(64);
//  stepper.setAcceleration(128);
//  stepper.setCurrentPosition(0);
//  int pos = -100; //// Is this doing anything moving forward?////
//  x=1;
//  int target = stepper.targetPosition();
//    for(y=0; y<100000 && x<=5; y++)
//    { 
//      //Serial.println(millis());  
//     // Serial.println(stepper.distanceToGo());
//        if (stepper.distanceToGo() == 0)
//        { 
//          delay(1);
//          stepper.setCurrentPosition(0);
//          pos = -pos;
//          x++;
//          stepper.moveTo(pos); /// Is the distance moved 2000 steps?
//          //target = stepper.targetPosition();
//          //Serial.println(target);
//         }
//     stepper.run();  /// Is the distance moved 1.35cm at the end of the loop?//   
//    }
// 
//  Serial.println("Enter new option:");
//  Serial.println();
//}
void SlowRampFunction()
{
  Serial.println("Stepping at slow ramp mode.");
  delay(600000);
  stepper.setCurrentPosition(0);
  digitalWrite(dir, HIGH); //Pull direction pin low to move "forward"
  digitalWrite(MS1, HIGH); //Pull MS1, and MS2 high to set logic to 1/8th microstep resolution
  digitalWrite(MS2, HIGH);
  for(x= 0; x<=256; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1172); //1 step/18.75s (9.375x2)
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1172);
    //Serial.println(stepper.currentPosition());
  }
  Serial.println(stepper.currentPosition());
  //Going back to zero strain
  stepper.setCurrentPosition(0);
  digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
    for(x= 0; x<=256; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(100); //1 step/37.5s (18.75x2)
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(100);
    //Serial.println(stepper.currentPosition());
  }
  Serial.println(stepper.currentPosition());
  Serial.println("Enter new option");
  Serial.println();
}

//Ramp stepping function
void RampFunction()
{
  //Serial.println(digitalRead(MS1)); //// Is this moving at full step or 1/8 step?////
  Serial.println("Ramp Function 0.1 Hz");
  //digitalWrite(MS1, HIGH); //Pull MS1, and MS2 high to set logic to 1/8th microstep resolution
  //digitalWrite(MS2, LOW);
  stepper.setMaxSpeed(6.4); //v=0.2133,a= 0.00035 for half step upto 64 stps
  stepper.setAcceleration(2.56);
  stepper.setCurrentPosition(0);
  int pos = -32; //// Is this doing anything moving forward?////
  x=1;
  int target = stepper.targetPosition();
    for(y=0; y<100000 && x<=2; y++)
    { 
      //Serial.println(millis());  
     // Serial.println(stepper.distanceToGo());
        if (stepper.distanceToGo() == 0)
        { 
          delay(1);
          stepper.setCurrentPosition(0);
          pos = -pos;
          x++;
          stepper.moveTo(pos); /// Is the distance moved 2000 steps?
          //stepper.currentPosition();
          //target = stepper.targetPosition();
          //Serial.println(target);
         }
     stepper.run();  /// Is the distance moved 1.35cm at the end of the loop?//   
    }
    delay(600000); //Imaging Time
    // Going back to zero strain
   stepper.setCurrentPosition(0);
  digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
    for(x= 0; x<=32; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(100); //1 step/37.5s (18.75x2)
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(100);
    //Serial.println(stepper.currentPosition());
  }
  Serial.println("Enter new option:");
  Serial.println();
}

//1Hz function
void OneHzFunction()
{
  //Serial.println(digitalRead(MS1)); //// Is this moving at full step or 1/8 step?////
  Serial.println("1Hz 1 cycles");
 // delay(300000);
  stepper.setMaxSpeed(64);
  stepper.setAcceleration(256);
  stepper.setCurrentPosition(0);
  int pos = -32; //// Is this doing anything moving forward?////
  x=1;
 // stepper.setCurrentPosition(0);
  int target = stepper.targetPosition();
    for(y=0; y<100000 && x<=3; y++)
    { 
      //Serial.println(millis());  
     // Serial.println(stepper.distanceToGo());
        if (stepper.distanceToGo() == 0)
        { 
          delay(1);
          stepper.setCurrentPosition(0);
          pos = -pos;
          x++;
          stepper.move(pos); /// Is the distance moved 2000 steps?
          //target = stepper.targetPosition();
          //Serial.println(target);
         }
     stepper.run();  /// Is the distance moved 1.35cm at the end of the loop?//   
    }
 
  Serial.println("Enter new option:");
  Serial.println();
}

//0.1Hz function
void PointOneHzFunction()
{
  //Serial.println(digitalRead(MS1)); //// Is this moving at full step or 1/8 step?////
  Serial.println("0.1Hz 1 cycles");
  stepper.setMaxSpeed(6.4);
  stepper.setAcceleration(2.56);
  stepper.setCurrentPosition(0);
  int pos = -32; //// Is this doing anything moving forward?////
  x=1;
  int target = stepper.targetPosition();
    for(y=0; y<100000 && x<=3; y++)
    { 
      //Serial.println(millis());  
     // Serial.println(stepper.distanceToGo());
        if (stepper.distanceToGo() == 0)
        { 
          delay(1);
          stepper.setCurrentPosition(0);
          pos = -pos;
          x++;
          stepper.moveTo(pos); /// Is the distance moved 2000 steps?
           
          //target = stepper.targetPosition();
          //Serial.println(target);
         }
     stepper.run();  /// Is the distance moved 1.35cm at the end of the loop?// 
     //Serial.println(stepper.currentPosition()) ;
    }
 
  Serial.println("Enter new option:");
  Serial.println();}
