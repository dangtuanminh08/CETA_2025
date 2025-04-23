int turns = 0;
float duration, distance;

//Ultra-sonic sensors
int echoPin = 10;
int trigPin = 11;

//IR Sensors
int val1 = A0;
int val2 = A1;
int val3 = A2;

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

void calibrateSensor() {
  float avgSensor1 = 0;
  float avgSensor2 = 0;
  float avgSensor3 = 0;
  int caltime = 5000;
  while caltime > 0 {
    float sensor1 = analogRead(pin);
    float sensor2 = analogRead(pin);
    float sensor3 = analogRead(pin);
    avgSensor1 = avgSensor1 + sensor1;
    avgSensor2 = avgSensor2 + sensor2;
    avgSensor3 = avgSensor3 + sensor3;
    moveforward(250);
    caltime = caltime - 250;
  }
  float avgWhite = (avgSensor1 + avgSensor2 + avgSensor3) / 3
  avgSensor1 = 0;
  avgSensor2 = 0;
  avgSensor3 = 0;
  caltime = 5000;
}

void testDecision() {
  if sensor1 >= avgWhite {
    turnRight;
  }
}/////////////


void loop() 
{
  digitalWrite(enableLeft, 255);
  digitalWrite(enableRight, 255);

  sensors();

  //Challenge 1: Running The Fairway
  if(val1 >= 100)
  {
    turn_right();
  }
  else if (val3 >= 100)
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
