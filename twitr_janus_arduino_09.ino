/*
 @@@@@@ 
 
 This sketch requires a Processing sketch  twitr_janus_processing_control_PC.pde (last known version 12)
 to be running on a linked PC.
 The PC is connecting to the Internet, finding data (e.g. from Twitter) 
 and passing it to the Arduino via a serial connect
 twitr_janus_arduino - [28/7/2012] Can take data from serial port and convert it into pin output to control relay for jaw. NOTE will convert any serial. This needs to be amended so that serial data from Twitter is not confused with serial data from other sources @@@@@@ 
 twitr_janus_arduino_2 - [29/9/2012] This has replaced the delay() fucntion with a timer while based on reading millis
 twitr_janus_arduino_3 - [29/9/2012] Can detect which data source is sending the data, but NOT the data any longer. This will be using abalogue detection
 twitr_janus_arduino_4 - [30/9/2012] attaching servo control!
 twitr_janus_arduino_5 - [01/10/2012] making servo control be based on google data!
 twitr_janus_arduino_6 - [03/10/2012] 
 twitr_janus_arduino_7 - [06/10/2012] trying to replace while loops for timing polling of the external data sources.
 twitr_janus_arduino_8 - [07/10/2012] adding audio peak detection using analog read and outputting to relay driver pin
 */

#include <Firmata.h>
#include <Servo.h> // includes standard arduino servo class

Servo servoUpDown;  // create servo object to control a servo 
Servo servoLeftRight;  // create servo object to control a servo 
//@@
int incomingByte = 0; 
float flagPeriod =1000;// decides how long to flash the indicator
int tweetTimer=0;
int googleTimer = 0;
int analogInput = 0;
int valueAnalogIn = 0;
int speechFlagPinLED = 1;// this is the output pin that drives the relay LED
int speechFlagPin = 2;// this is the output pin that drives the relay
int thresholdAnalogIn = 400; // threshhold number representing voltage over which jaw movement is triggered
int twitterFlagPin=9; // flags that Twitter incoming message received
int googleFlagPin =3; // flags that Google spreadsheet incoming message received
boolean twitterNew= true;
boolean googleNew= true;
int leftRightPin = 5; // output PINs for control signal for servos
int upDownPin = 6; // output PINs for control signal for servos
int blinkVal=0;
int eyeLeftRight = 3; // defaults for eye positioning variable
int eyeUpDown = 3; // defaults for eye positioning variable
// this array defines Google presets based on a 5-sided matrix: top left = (1,1) 
//int eyePos [] = {0,0,0,45,0,90,0,135,0,180,45,0,45,45,45,90,45,135,45,180,90,0,90,45,90,90,90,135,90,180,135,0,135,45,135,90,135,135,135,180,180,0,180,45,180,90,180,135,180,180};
int eyePos [] = {115, 60, 115, 60, 115, 90, 115, 115, 115, 115, 115, 60, 115, 60, 115, 90, 115, 115, 115, 115, 90, 60, 90, 60, 90, 90, 90, 115, 90, 115, 60, 60, 60, 60, 60, 90, 60, 115, 60, 115, 60, 60, 60, 60, 60, 90, 60, 115, 60, 115};

// to call the eye Pos, you need the Google value and to position servo up/down to  

void setup() {
  servoUpDown.attach(upDownPin);  // attaches the servo 
  servoLeftRight.attach(leftRightPin);  // attaches the servo 
  pinMode (googleFlagPin, OUTPUT);
  pinMode (twitterFlagPin, OUTPUT);
  pinMode (speechFlagPin, OUTPUT);
  pinMode (speechFlagPinLED, OUTPUT);
  digitalWrite(speechFlagPin, LOW);
  digitalWrite(speechFlagPinLED, LOW);
servoLeftRight.write(90); // initialise servo/eyeball positions to centre
    servoUpDown.write(90); // initialise servo/eyeball positions to centre
  Serial.begin(115200);
  Serial.println("Serial connection initiated");
  checkSerial ();

}

void loop ()
{
  checkSerial (); //checks for incoming data
  analogPeakCheck(); // checks for audio output levels
  }
void checkSerial ()
{
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    }
     if (incomingByte==30 ) //  data is data coded as an integer  between 0 and 25
    {
          twitterCheck();
}
     
    
    if ((incomingByte<=25) && (incomingByte >0) ) // google data is data coded as an integer  between 0 and 25
    {
        googleCheck();
    } 
    else {// No point calling check functions if not serial data received. This is error handling clause 

        Serial.println("I received nothing ");
    }

  
}// enf of checkSerial

void twitterCheck ()
{
  // this function 
digitalWrite(twitterFlagPin, HIGH);

 digitalWrite(googleFlagPin, LOW);
   
}

void googleCheck()
{
  digitalWrite(twitterFlagPin, LOW);
  eyeLeftRight= 2*(incomingByte-1);
eyeUpDown = (2*incomingByte)-1;
  if (incomingByte<=25 )
  {
     digitalWrite(googleFlagPin, HIGH);

    servoLeftRight.write(eyePos[eyeLeftRight]);
    servoUpDown.write(eyePos[eyeUpDown]);
  
  }

 
  }
void analogPeakCheck()
{
  // @@@@@@@ this function is used if you are using raw audio output from an analog amplifier into the Analog pin 0
  valueAnalogIn = analogRead(analogInput); // This is checking for output above a threshold voltage to trigger jaw signal
  if (valueAnalogIn>thresholdAnalogIn)
  {
digitalWrite(speechFlagPin, HIGH);
 digitalWrite(speechFlagPinLED, HIGH);
  
  }  
  else {
digitalWrite(speechFlagPin, LOW);
digitalWrite(speechFlagPinLED, LOW);
 
   
  }// @@ end threshold checking // 
}


