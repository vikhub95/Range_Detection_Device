/**
    Viktor Silivanov
    3/22/2018
    Portable distance detection device using the HC-SR04 Ultrasonic sensor with distance adjustment for triggering the buzzer.
**/

//******************** Libraries ****************************************************************************************************************************************************************************************************************
#include <NewPing.h>

//******************** Pins ********************************************************************************************************************************************************************************************************************

//Analog Pins
#define wiper        A5
//Digital Pins
#define echo         2
#define trig         3
#define ON_LED       7
#define buzzer       8
#define buzzer_LED   9

//******************** Global Variables *********************************************************************************************************************************************************************************************************

//User Modification Allowed
int frequency =      700;                                                   //Frequency in Hz
int lower_bound =    60;                                                    //Distance in mm
int upper_bound =    200;                                                   //Distance in mm

//Do Not Modify
int buzz_time =      250;                                                   //Time in milliseconds for buzzer right upon startup
int buzz_delay =     280;                                                   //Time in milliseconds for buzzer delay right upon startup
int max_distance =   400;                                                   //Argument for object in the NewPing library
int i =              0;
int distance;
int threshold;
 
NewPing sonar(trig, echo, max_distance);                                    //Instanciation of object "sonar"

//******************** Setup ********************************************************************************************************************************************************************************************************************
//This function runs only once

void setup() {
  Serial.begin(9600);                                                       //Allows use of serial monitor with a baud rate of 9600

  //Pin I/O Type Declaration
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(buzzer_LED, OUTPUT);
  pinMode(ON_LED, OUTPUT);

  digitalWrite(ON_LED, HIGH);                                               //Turns on green LED; Stays on while unit is on

  //Buzzer beeps 3 times upon turning on device 
  while(i < 3){
    digitalWrite(buzzer_LED, HIGH);
    tone(buzzer, frequency, buzz_time);
    delay(buzz_delay);  
    i++;
  }
  digitalWrite(buzzer_LED, LOW);
}

//******************** Runs Endlessly ***********************************************************************************************************************************************************************************************************

void loop() {
  distance = sonar.ping_cm();                                              //Object "sonar" utilizes ping_cm() function from the library, which returns a value in cm. Distance then take on that value
  threshold = analogRead(wiper);                                           //Obtain analog value from wiper of the potentiometer
  threshold = map(threshold, 0, 1023, lower_bound, upper_bound);           //Translate analog readings into trigger range. Update values of threshold
  
  if(distance != 0 && distance < threshold){                               //Condition to trigger the buzzer
    tone(buzzer, frequency);
    digitalWrite(buzzer_LED, HIGH);
  }
  else{                                                                    //State of buzzer & LED otherwise
    noTone(buzzer);
    digitalWrite(buzzer_LED, LOW);
  }

  //serial_test();                                                           //Uncommment this to view data from the serial monitor
}

//******************** Test Function ************************************************************************************************************************************************************************************************************
//Utilizes serial monitor to display some values. Used for testing/debugging purposes.

void serial_test(){
  Serial.print("Ping: ");
  Serial.print(distance);
  Serial.println("cm");

  Serial.print("Range: [");
  Serial.print(lower_bound);
  Serial.print(", ");
  Serial.print(upper_bound);
  Serial.println("] cm");
  
  Serial.print("Threshold: ");
  Serial.print(threshold);
  Serial.println("cm");
  delay(100);
}
