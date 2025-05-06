// CETA 2025 
// Code from Devious Birds 
// Made by Abir Khaund-Hazarika(a_beer), Aaron Chan, Minh Dang, Darvin Gutierrez 
// Challenge 3: Collision Avoidance
// Circuit diagram @ https://wokwi.com/projects/428782660635648001 
// Using L293D motor driver with QRE1113 IR Sensors and HC-05 Distance Sensor

// IR Sensor Pins
#define left A0
#define center A1
#define right A2

// Motor Control Pins
int enableLeft = 5;
int MotorLeft1 = 7;
int MotorLeft2 = 2;
int enableRight = 3;
int MotorRight1 = 9;
int MotorRight2 = 4;

// Sensor values
int leftVal, centerVal, rightVal;
int avgWhite = 300;

// Ultra-sonic sensor pins
int echoPin = 10;
int trigPin = 11;
long duration;
long distance;

// Button control
int buttonState = 0;
int previousState = 0;
unsigned long pressStartTime = 0;
const long holdDuration = 3000;
bool toggleState = false;

// Counters
int lineCounter = 0;
int counter = 0;
bool inBlackArea = false;

void setup() {
  Serial.begin(9600);
  pinMode(enableLeft, OUTPUT);
  pinMode(MotorLeft1, OUTPUT);
  pinMode(MotorLeft2, OUTPUT);
  pinMode(enableRight, OUTPUT);
  pinMode(MotorRight1, OUTPUT);
  pinMode(MotorRight2, OUTPUT);
  pinMode(left, INPUT);
  pinMode(center, INPUT);
  pinMode(right, INPUT);
  pinMode(12, INPUT);
  // Set ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);  
  pinMode(echoPin, INPUT);  
}

void loop() {
  leftVal = analogRead(left);
  centerVal = analogRead(center);
  rightVal = analogRead(right);

  digitalWrite(trigPin, LOW);  
  delayMicroseconds(2);  
  digitalWrite(trigPin, HIGH);  
  delayMicroseconds(10);  
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance in centimeters
  distance = duration / 58.2;  // 58.2 is the factor for cm (sound speed in air)
  
  // Output the distance to the Serial Monitor
  Serial.print("Distance in CM: ");
  Serial.print(distance);

  // Print the sensor readings for debugging
  Serial.print(" Line Counter: ");
  Serial.print(lineCounter);
  Serial.print(" Position: (");
  Serial.print(leftVal);
  Serial.print(", ");
  Serial.print(centerVal);
  Serial.print(", ");
  Serial.print(rightVal);
  Serial.print(") ");

  buttonState = digitalRead(12);
  if (buttonState == HIGH && previousState == LOW) {
    pressStartTime = millis();
  }
  if (buttonState == LOW && previousState == HIGH) {
    unsigned long pressDuration = millis() - pressStartTime;
    if (pressDuration >= holdDuration) {
      calibrate_sensor();
    } else {
      toggleState = !toggleState;
    }
  }

  if (toggleState) {
    if (lineCounter == 2) {
      forward();
      Serial.println("FINAL FORWARD");
      delay(500);
      stop();
      while (true) {}  // Halt
    }
    
    if (distance <= 10)
    {
      turn_around();
      delay(450);
      while (analogRead(left) <= avgWhite) {
        turn_around();
      }
    }

    if (lineCounter < 1) {
      forward();
      Serial.println("FORWARD INIT");
      delay(150);
      lineCounter++;
    } else if (leftVal >= avgWhite && centerVal >= avgWhite && rightVal >= avgWhite) {
      if (!inBlackArea) {
        lineCounter++;
        inBlackArea = true;
        Serial.print("Line Counter incremented to: ");
        Serial.println(lineCounter);
      }
      if (lineCounter < 5) {
        /*turn_around();
        delay(450);
        while (analogRead(left) <= avgWhite) {
          turn_around();
        }*/
      }
    } else {
      inBlackArea = false;
    }

    if (leftVal >= avgWhite) {
      turn_left();
      counter = 1;
      Serial.println("LEFT");
    } else if (rightVal >= avgWhite) {
      turn_right();
      counter = 2;
      Serial.println("RIGHT");
    } else if (leftVal <= avgWhite && rightVal <= avgWhite && centerVal <= avgWhite) {
      if (counter == 1) {
        sharp_left();
        Serial.println("SHARP LEFT");
      } else if (counter == 2) {
        sharp_right();
        Serial.println("SHARP RIGHT");
      } else {
        forward();
        Serial.println("FORWARD");
      }
    } else {
      forward();
      counter = 0;
      Serial.println("FORWARD");
    }
  } else {
    stop();
  }

  previousState = buttonState;
  Serial.println();
}

void calibrate_sensor() {
  Serial.println("\nCALIBRATING...");
  int avgSensorLeft = 0, avgSensorCenter = 0, avgSensorRight = 0;
  int sensorReadings = 1000;

  for (int i = 0; i < sensorReadings; i++) {
    avgSensorLeft += analogRead(left);
    avgSensorCenter += analogRead(center);
    avgSensorRight += analogRead(right);
  }

  avgWhite = (avgSensorLeft + avgSensorCenter + avgSensorRight) / (3 * sensorReadings);
  Serial.print("Average White Value: ");
  Serial.println(avgWhite);
  delay(100);
}

void forward() {
  analogWrite(enableLeft, 255);
  analogWrite(enableRight, 255);
  digitalWrite(MotorLeft1, HIGH);
  digitalWrite(MotorRight1, HIGH);
  digitalWrite(MotorLeft2, LOW);
  digitalWrite(MotorRight2, LOW);
}

void stop() {
  digitalWrite(MotorLeft1, LOW);
  digitalWrite(MotorRight1, LOW);
  digitalWrite(MotorLeft2, LOW);
  digitalWrite(MotorRight2, LOW);
}

void turn_right() {
  analogWrite(enableLeft, 255);
  analogWrite(enableRight, 150);
  digitalWrite(MotorLeft1, HIGH);
  digitalWrite(MotorRight1, HIGH);
  digitalWrite(MotorLeft2, LOW);
  digitalWrite(MotorRight2, LOW);
}

void turn_left() {
  analogWrite(enableLeft, 150);
  analogWrite(enableRight, 255);
  digitalWrite(MotorLeft1, HIGH);
  digitalWrite(MotorRight1, HIGH);
  digitalWrite(MotorLeft2, LOW);
  digitalWrite(MotorRight2, LOW);
}

void sharp_right() {
  analogWrite(enableLeft, 255);
  analogWrite(enableRight, 0);
  digitalWrite(MotorLeft1, HIGH);
  digitalWrite(MotorRight1, HIGH);
  digitalWrite(MotorLeft2, LOW);
  digitalWrite(MotorRight2, LOW);
}

void sharp_left() {
  analogWrite(enableLeft, 0);
  analogWrite(enableRight, 255);
  digitalWrite(MotorLeft1, HIGH);
  digitalWrite(MotorRight1, HIGH);
  digitalWrite(MotorLeft2, LOW);
  digitalWrite(MotorRight2, LOW);
}

void turn_around() {
  analogWrite(enableLeft, 255);
  analogWrite(enableRight, 255);
  digitalWrite(MotorLeft1, LOW);
  digitalWrite(MotorRight1, HIGH);
  digitalWrite(MotorLeft2, HIGH);
  digitalWrite(MotorRight2, LOW);
}
