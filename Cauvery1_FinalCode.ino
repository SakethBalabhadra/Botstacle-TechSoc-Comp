#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>
//#include <bits/stdc++.h>
//#include <ArduinoSTL.h>

#define trigPin 22
#define echoPin 5

float arr[3];
float m;
int maxangle;
int angle;

#include <ESP32Servo.h>
Servo myServo;
int servoPin = 16;


float mean(float arr[])
{
  return (arr[0] + arr[1] + arr[2])/3;
}
AsyncWebServer server(80);
//stack<int> dist;
const char* ssid = "Galaxy F42 5G1298";
const char* password = "yodg2065";
//const char* ssid = "iQOO Z3 5G";
//const char* password = "43211234";

void recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received Data....");
  String d = "";
  for (int i=0; i < len; i++){
    d += char(data[i]);
}
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // put your setup code here, to run once:
  //this block is for servo motor
  //Serial.begin(115200);
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myServo.setPeriodHertz(50);
  myServo.attach(servoPin, 500, 2450);

  
  if(WiFi.waitForConnectResult() != WL_CONNECTED){
    Serial.println("WiFi Failed!");
    return;
}
Serial.print("IP Address: ");
Serial.println(WiFi.localIP());
WebSerial.begin(&server);
WebSerial.msgCallback(recvMsg);
server.begin(); 
}

void loop() {
  // put your main code here, to run repeatedly:


  // put your main code here, to run repeatedly:
  for(angle = 20 ; angle<=160 ; angle+=10){
      myServo.write(angle);
      //Serial.print("Angle = ");
      //Serial.println(angle);
              digitalWrite(trigPin, LOW);
              delayMicroseconds(2);
              digitalWrite(trigPin, HIGH);
              delayMicroseconds(10);
              digitalWrite(trigPin, LOW);
            
              float duration = pulseIn(echoPin, HIGH);
            
              float distance = duration * 0.01715;

              if( angle <= 40 )
              {
                arr[(angle-20)/10] = distance;
                if(angle == 40)
                {
                  m = mean(arr);
                  maxangle = 30;
                }
              }
              
              else
              {
                 arr[0]=arr[1];
                 arr[1]=arr[2];
                 arr[2]=distance;
                 if(mean(arr) > m)
                 {
                    m = mean(arr);
                    maxangle = angle - 10;
                 }
              }
              //WebSerial.println(distance);
              delay(100);
     
  }
  WebSerial.print("Distance : ");
  WebSerial.print(m);
  WebSerial.print(" ");
  WebSerial.print("Angle : ");
  WebSerial.println(maxangle);
  for (angle = 160 ; angle>=20 ; angle--)
  {
    myServo.write(angle);
    //Serial.print("Angle = ");
    //Serial.println(angle);
    delay(10);
    
  }
  delay(1000);
  WebSerial.println("Came back to 0 degrees");//Might wanna comment these out
  
  
}
