// defines pins numbers
const int trigLeftPin = 8;
const int trigCenterPin = 9;
const int trigRightPin = 10;

const int echoLeftPin = 11;
const int echoCenterPin = 12;
const int echoRightPin = 13;

const int sensorLeftPin = 6;
const int sensorRightPin = 7;

//distances
int distanceLeft = 0;
int distanceCenter = 0;
int distanceRight = 0;

//sensors
int sensorLeftValue = 0;
int sensorRightValue = 0;

bool doingManeuver = false;

int movingOption = 0;

void setup() {
  for(int trigPin = trigLeftPin; trigPin <= trigRightPin; trigPin++){
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(trigPin + 3, INPUT); // Sets the echoPin as an Input
  }
  pinMode(sensorLeftPin, INPUT);
  pinMode(sensorRightPin, INPUT);

  Serial.begin(9600); // Starts the serial communication
  //delay(5000);//delay for start fight
}

void loop() {
  setDefaultValue();
  bool wayBroken = checkBreakWay();
  if(!wayBroken)
    findEnemy();

  setMotionOptiom();
  motion();

  Serial.print("Recommendation: ");
  Serial.println(movingOption); 
}

void motion(){
   switch (movingOption){
      case 0: //cruise
        motionCruise();
        break;
      case 1: //turn left
        motionLeft();
        break;
      case 2: //go straight
        motionStraight();
        break; 
      case 3: //turn right
        motionRight();
        break;  
      case 4: //turn back left
        motionTurnBackLeft();
        break;  
      case 5: //turn back right
        motionTurnBackRight();
        break;     
      case 6: //turn back all
        motionTurnBackAll();
        break; 
      default://cruise
        motionCruise();
        break;    
    }
}

void motionCruise(){
  
}

void motionLeft(){
  
}

void motionRight(){
  
}

void motionStraight(){
  
}

void motionTurnBackLeft(){
  
}

void motionTurnBackRight(){
  
}
void motionTurnBackAll(){
  
}

void setDefaultValue(){
  distanceLeft = 0;
  distanceCenter = 0;
  distanceRight = 0;
}

bool checkBreakWay(){
  sensorLeftValue = digitalRead(sensorLeftPin);
  sensorRightValue = digitalRead(sensorRightPin);
  return sensorLeftValue == 1 || sensorRightValue == 1;
}

void findEnemy(){
    for(int trigPin = trigLeftPin; trigPin <= trigRightPin; trigPin++){
      // Clears the trigPin
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      // Sets the trigPin on HIGH state for 10 micro seconds
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      // Reads the echoPin, returns the sound wave travel time in microseconds
      long duration = pulseIn(trigPin + 3, HIGH);
      // Calculating the distance
      int distance = calculateDistance(duration);
      if(distance < 100){
        SetDistance(trigPin, distance);
    }
  }
}

int calculateDistance(long duration){
  return duration*0.034/2;
}

//void showDistances(){
//    Serial.print("Left: ");
//    Serial.print(distanceLeft);
//    Serial.print("Center: ");
//    Serial.print(distanceCenter);
//    Serial.print("Right: ");
//    Serial.println(distanceRight);
//}

void SetDistance(int pin, int distance){
    switch (pin){
      case trigLeftPin: 
        distanceLeft = distance;
        break;
      case trigCenterPin: 
        distanceCenter = distance;
        break;
      case trigRightPin: 
        distanceRight = distance;
        break;    
      default:
        break;    
    }
}

//0-cruise;1-turn left;2-go straight;3-turn right;4-turn back left;5-turn back right;6-turn back all;
void setMotionOptiom(){

    if (sensorLeftValue == 1 && sensorRightValue == 1){
      movingOption = 6;//turn back all
    }
    else if (sensorLeftValue == 1){
      movingOption = 4;//turn back left
    }
    else if (sensorRightValue == 1){
      movingOption = 5;//turn back right
    }
    else if (distanceLeft == 0 && distanceCenter == 0 && distanceRight == 0){
      movingOption = 0;//cruise
    }
    else if (distanceLeft > 0 && (distanceLeft < distanceCenter || distanceCenter == 0)){
      movingOption = 1;//turn left
    }
    else if (distanceCenter > 0 && (distanceCenter <= distanceLeft || distanceLeft == 0) && (distanceCenter <= distanceRight || distanceRight == 0)){
      movingOption = 2;//go straight
    }
    else if (distanceRight > 0 && (distanceRight < distanceCenter || distanceCenter == 0)){
      movingOption = 3;//turn right
    }
    else{
      movingOption = 0;//cruise
      //showDistances();
    }
}
