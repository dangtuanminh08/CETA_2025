int turns = 0;
float duration, distance;

//Ultra-sonic sensors
int echoPin = 10;
int trigPin = 11;

//IR Sensors
int val1 = A0;
int val2 = A1;
int val3 = A2;
float avgSensor1 = 0;
float avgSensor2 = 0;
float avgSensor3 = 0;
float avgSensorWhite = 0;
int calTime = 100;

//Left Motor 
int enableLeft = 8;  
int MotorLeft1 = 7;
int MotorLeft2 = 2;
//Right Motor
int enableRight = 3;
int MotorRight1 = 9;
int MotorRight2 = 4;

void setup() 
{
  Serial.begin (9600);
  //Motors
  pinMode(enableLeft, OUTPUT);
  pinMode(MotorLeft1, OUTPUT);
  pinMode(MotorLeft2, OUTPUT);
  pinMode(enableRight, OUTPUT);
  pinMode(MotorRight1, OUTPUT);
  pinMode(MotorRight2, OUTPUT);
  //IR Sensors
  pinMode(val1, INPUT);
  pinMode(val2, INPUT);
  pinMode(val3, INPUT);
  //Ultra-sonic
  pinMode(trigPin, OUTPUT);  
	pinMode(echoPin, INPUT);  

}

//Some sort of calibration concept
//much rough

void calibrateSensor() 
{
  while calTime > 0 {
    digitalWrite(enableLeft, 255);
    digitalWrite(enableRight, 255);
    avgSensor1 += analogRead(val1);
    avgSensor2 += analogRead(val2);
    avgSensor3 += analogRead(val3);
    forward();
    calTime -= 1;
  }
  avgSensorWhite = (avgSensor1 + avgSensor2 + avgSensor3) / 3
  Serial.print("Average White Value: ");
  Serial.println(avgSensorWhite);
  avgSensor1 = 0;
  avgSensor2 = 0;
  avgSensor3 = 0;
  calTime = 100;
}

void loop() 
{
  digitalWrite(enableLeft, 255);
  digitalWrite(enableRight, 255);

  sensors();

  //Challenge 1: Running The Fairway
  if(val1 >= avgSensorWhite);
  {
    turn_right();
  }
  else if (val3 >= avgSensorWhite)
  {
    turn_left();
  }
  else
  {
    forward();
  }

  //Challenge 2: IOT
  //unfinished
  
  //Challenge 3: Collision Avoidance
  fire_ultrasonic();
  duration = pulseIn(echoPin, HIGH); 
  distance = duration/58;
  Serial.print("Distance in CM: ");
  Serial.println(distance);
  
  if (distance <= 10)
  {
    turn_around();
  }

  delay(10);
  forward();
}

//read sensors value
void sensors()
{
  analogRead(val1);
  analogRead(val2);
  analogRead(val3);
}

void fire_ultrasonic()
{
  digitalWrite(trigPin, LOW);  
	delayMicroseconds(2);  
	digitalWrite(trigPin, HIGH);  
	delayMicroseconds(10);  
	digitalWrite(trigPin, LOW);
}
//drive functions
void forward()
{
  digitalWrite(MotorLeft1, 255);
  digitalWrite(MotorRight1, 255);
  digitalWrite(MotorLeft2, 0);
  digitalWrite(MotorRight2, 0);
}

void backward()
{
  digitalWrite(MotorLeft1, 0);
  digitalWrite(MotorRight1, 0);
  digitalWrite(MotorLeft2, 255);
  digitalWrite(MotorRight2, 255);
}

void turn_right()
{
  digitalWrite(MotorLeft1, 255);
  digitalWrite(MotorRight1, 0);
  digitalWrite(MotorLeft2, 0);
  digitalWrite(MotorRight2, 255);
}

void turn_left()
{
  digitalWrite(MotorLeft1, 0);
  digitalWrite(MotorRight1, 255);
  digitalWrite(MotorLeft2, 255);
  digitalWrite(MotorRight2, 0);
}

//Perchance
void turn_around()
{
  Serial.print("Turn around");
  // while (not turned 180 degrees)
    // {
    //   turn_left();
    // }
    // turns++;
}

void check_for_end()
{
  Serial.print("Stopped");
}
