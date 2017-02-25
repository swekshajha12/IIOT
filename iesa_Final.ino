#include <TH02.h>
#include <math.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>
#include <rgb_lcd.h>
#include <Wire.h>
# include <Servo.h>
#include "MQ135.h"
 

# define anaread A0
# define digiread 2
int readAna;
int readDigi;
float ppm;
 
//led parameters
    # define ledsafe 22
    # define ledrisk 26
    # define leddanger 11
 
//LCD_RGB
rgb_lcd lcd;
    //servo variable
    int pos = 0; 
    Servo myservo;
//buzzer pin
# define buzzer 4
 
//temperature/humidity sensor
#define DHTPIN A0 
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
 
//\\light sensor
#define LIGHT_SENSOR A1//Grove - Light Sensor is connected to A0 of Arduino
const int thresholdvalue=10;         //The treshold for which the LED should turn on. Setting it lower will make it go on at more light, higher for more darkness
float Rsensor; //Resistance of sensor in K
   
void setup() {
  Serial.begin(9600);
                  pinMode(22,OUTPUT);
                  pinMode(26,OUTPUT);
                  pinMode(11,OUTPUT);
 
   lcd.begin(16, 2);
    myservo.attach(8); 
    pinMode(buzzer, OUTPUT);     
   // Serial.begin(9600);
    dht.begin();         
}
MQ135 gasSensor = MQ135(anaread);
 
float getPPM();
void loop() {
  // put your main code here, to run repeatedly:
readAna=analogRead(anaread);
readDigi=digitalRead(digiread);
Serial.print("1-analog-");
Serial.println(readAna,DEC);
Serial.print("2-digital-");
Serial.println(readDigi,DEC);
 float rzero = gasSensor.getRZero(); //this to get the rzero value, uncomment this to get ppm value
 float ppm = gasSensor.getPPM(); // this to get ppm value, uncomment this to get rzero value
 float resistence=  gasSensor.getResistance();
 Serial.print("3-rzero-");
 Serial.println(rzero); // this to display the rzero value continuously, uncomment this to get ppm value
 Serial.print("4-ppm-");
 Serial.println(ppm); // this to display the ppm value continuously, uncomment this to get rzero value
  Serial.print("5-resistence-");
  Serial.println(resistence);
delay(10);
ledgreen();
delay(10);
ledyellow();
delay(10);
ledgreen();
delay(10);
tempHumididty();
lightsensor();
}
 

            void playTone(int tone, int duration) 
          {
            for (long i = 0; i < duration * 1000L; i += tone * 2) 
            {
              digitalWrite(buzzer, HIGH);
              delayMicroseconds(tone);
              digitalWrite(buzzer, LOW);
              delayMicroseconds(tone);
            }
          }
 

  void ledgreen()
 {
 
  if(ppm==0.00){
    void display(); 
    lcd.setCursor(0, 1);
    lcd.setRGB(0, 255, 0);
    lcd.print("SAFE!");
    delay(10);
     
     //todo:shift to the ledred()
     
     for ( pos = 0; pos <= 180; pos =pos+ 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos =pos- 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
    
  }
 

        //Todo:shift to someother place
        playTone(4,1000);
  
 }
 
//led funtions initialized
 
 void ledyellow()
 {
 
  if(ppm>1000 && ppm<=2000){
    void display(); 
    lcd.setCursor(0, 1);
    lcd.setRGB(0, 0, 255);
    lcd.print("RISK!");
    delay(10);
   
  }
  
 }
 void ledred()
 {
 /* if(ppm>2000 && ppm<=5000){
    digitalWrite(leddanger,HIGH);
    delay(100);
    digitalWrite(leddanger,LOW);
     delay(100);
  }*/
  if(ppm>2000 && ppm<=5000){
    void display(); 
    lcd.setCursor(0, 1);
    lcd.setRGB(255, 0, 0);
    lcd.print("RISK!");
    delay(10);
   
    for ( pos = 0; pos <= 180; pos =pos+ 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos =pos- 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  } 
    
  }
 }
 
              void tempHumididty()
             {
              // Reading temperature or humidity takes about 250 milliseconds!
              // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
              float h = dht.readHumidity();
              float t = dht.readTemperature();
          
              // check if returns are valid, if they are NaN (not a number) then something went wrong!
              if (isnan(t) || isnan(h)) 
              {
                  Serial.println("Failed to read from DHT");
              } 
              else 
              {
                  Serial.print("Humidity: "); 
                  Serial.print(h);
                  Serial.print(" %\t");
                  Serial.print("Temperature: "); 
                  Serial.print(t);
                  Serial.println(" ^C");
              }
          }
 
          void lightsensor()
          {
             int sensorValue = analogRead(LIGHT_SENSOR); 
              Rsensor = (float)(1023-sensorValue)*10/sensorValue;
              Serial.println("LIGHT SENSOR VALUE ");
              Serial.println(sensorValue);
              Serial.println("the sensor resistance is ");
              Serial.println(Rsensor,DEC);//show the ligth intensity on the serial monitor;
              if(sensorValue>=140 &&sensorValue<=200)
              {
                 
                  digitalWrite(ledsafe,HIGH);
                  delay(10);
                  digitalWrite(ledsafe,LOW);
                  delay(10);
   
              }
              else if(sensorValue>=600 &&sensorValue<=750)
              {
                 
                  digitalWrite(ledsafe,HIGH);
                  delay(10);
                  digitalWrite(ledsafe,LOW);
                  delay(10);
              }
              else{
                  digitalWrite(ledrisk,HIGH);
                  delay(10);
                  digitalWrite(ledrisk,LOW);
                  delay(10);
              }
          }
          
