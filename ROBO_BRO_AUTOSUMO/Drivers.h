#define LEFT_FORWARD 5
#define LEFT_BACKWARD 4
#define RIGHT_FORWARD 7
#define RIGHT_BACKWARD 8

#define POWER_RIGHT 6
#define POWER_LEFT 3

void initDrivers() {
  pinMode(POWER_LEFT, OUTPUT);
  pinMode(POWER_RIGHT, OUTPUT);
  pinMode(LEFT_FORWARD, OUTPUT);
  pinMode(RIGHT_FORWARD, OUTPUT);
  pinMode(LEFT_BACKWARD, OUTPUT);
  pinMode(RIGHT_BACKWARD, OUTPUT);
}


void Move(byte leftState, byte leftPower, byte rightState, byte rightPower)
{
    analogWrite(POWER_LEFT, leftPower);
    digitalWrite(LEFT_FORWARD, leftState);   
    digitalWrite(LEFT_BACKWARD, !leftState); 

    analogWrite(POWER_RIGHT, rightPower);
    digitalWrite(RIGHT_FORWARD, rightState);
    digitalWrite(RIGHT_BACKWARD, !rightState);
}

