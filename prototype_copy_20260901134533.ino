#include <QTRSensors.h>
#include <SD.h>
#include <L298N.h>

//line sennsor array
QTRSensors qtr;

const uint8_t SensorCount = 4;

uint16_t sensorValues[SensorCount];


//ultrasonic pins
const int trig = A0;
const int echo = A1;

  // Define color sensor pins
 
#define S0 5
#define S1 6
#define S2 7
#define S3 8
#define sensorOut 9
// //Motor A
// int IN1 = 1; int IN2 = 2;

//Motor B
int IN3 = 3; int IN4 = 4;
//motor pin assignment
              //       motor A            motor B
              // |------------------||------------------|
L298N myMotors( IN3, IN4);
// Calibration Values
// Get these from Calibration Sketch
 
int redMax = 19; // Red maximum value
int redMin = 243; // Red minimum value
int greenMax = 21; // Green maximum value
int greenMin = 323; // Green minimum value
int blueMax = 18; // Blue maximum value
int blueMin = 269; // Blue minimum value
 
// Variables for Color Pulse Width Measurements
 
int redPW = 0;
int greenPW = 0;
int bluePW = 0;
 
// Variables for final Color values
 
int redValue;
int greenValue;
int blueValue;



//SD card reader
File myFile;

const int CSPin = 10;


void setup()

{
  Serial.begin(9600);

  qtr.setTypeRC();

  pinMode(CSPin, OUTPUT);

  qtr.setSensorPins((const uint8_t[]){A2, A3, A4, A5}, SensorCount);

  //motor pin configuration
  // pinMode(IN1, OUTPUT);
  // pinMode(IN2, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

 // configure the sensors

  // Set S0 - S3 as outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Set Sensor output as input
  pinMode(sensorOut, INPUT);
  
  // Set Frequency scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);

    for (int i = 0; i < 250; i++)
    {   
        

        qtr.calibrate();
        delay(20);
    }



  Serial.println("Initializing SD card...");

  

// Note that even if it's not used as the CS pin, the hardware SS pin 

  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 

  // or the SD library functions will not work. 

   

    if (!SD.begin(CSPin)) {

    Serial.println("initialization failed!");

    return;

  }

  Serial.println("initialization done.");

}


 




void loop()

{
  //LineSensor();
  ColourSensor();
  FollowLine();
}

void LineSensor() {
    // read raw sensor values

  qtr.read(sensorValues);



  // print the sensor values as numbers from 0 to 1023, where 0 means maximum

  // reflectance and 1023 means minimum reflectance

  for (uint8_t i = 0; i < SensorCount; i++)

  {

    Serial.print(sensorValues[i]);

    Serial.print('\t');

  }

  Serial.println();




}


void FollowLine(){
  uint16_t position = qtr.readLineBlack(sensorValues);  
    for (uint8_t i = 0; i < SensorCount; i++)
    {
      Serial.print(sensorValues[i]);
      Serial.print('\t');
    }
    Serial.println(position); 

     if(position>1000)  
    {
        myMotors.forward();
   
    }


    if(position<1000)
    {  
        myMotors.backward();

    }
}


void ColourSensor(){
  // Read Red value
  redPW = getRedPW();
  // Map to value from 0-255
  redValue = map(redPW, redMin,redMax,255,0);
  // Delay to stabilize sensor
  delay(200);
  
  // Read Green value
  greenPW = getGreenPW();
  // Map to value from 0-255
  greenValue = map(greenPW, greenMin,greenMax,255,0);
  // Delay to stabilize sensor
  delay(200);
  
  // Read Blue value
  bluePW = getBluePW();
  // Map to value from 0-255
  blueValue = map(bluePW, blueMin,blueMax,255,0);
  // Delay to stabilize sensor
  delay(200);
  
  // Print output to Serial Monitor
  Serial.print("Red = ");
  Serial.print(redValue);
  Serial.print(" - Green = ");
  Serial.print(greenValue);
  Serial.print(" - Blue = ");
  Serial.println(blueValue);
  
}
 
 
// Function to read Red Pulse Widths
int getRedPW() {
 
  // Set sensor to read Red only
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;
 
}
 
// Function to read Green Pulse Widths
int getGreenPW() {
 
  // Set sensor to read Green only
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;
 
}
 
// Function to read Blue Pulse Widths
int getBluePW() {
 
  // Set sensor to read Blue only
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;
 
}
