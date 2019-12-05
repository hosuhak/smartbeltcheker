//우리가 사용하는 센서는 온습도센서, 초음파센서, 푸시버튼, 부저, lcd모니터, 가속도센서, 블루투스
#include <Adafruit_Sensor.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN 2       
#define DHTTYPE DHT11     
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;
sensor_t sensor;
const int xPin = A10;
const int yPin = A9;
const int zPin = A8;
int minVal = 265;
int maxVal = 402;
int echoPin = 6;
int trigPin = 5;
int sensorPin = A0;     
double alpha = 0.75;
double x;
double y;
double z;
int tmpx=0;
int tmpy=0;
int tmpz=0;
int period = 100; 
LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial BTserial(8,9);
void setup() {
  Serial.begin(9600);
  BTserial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(10, OUTPUT);
  dht.begin();
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;
  
  lcd.init();
  lcd.backlight();
  
}

void loop() {
   delay(100);
   sensors_event_t event_t;  
   dht.temperature().getEvent(&event_t);
   sensors_event_t event_h;  
   dht.humidity().getEvent(&event_h);
   
   Serial.print("Humidity: ");
   BTserial.print("Humidity: ");
   Serial.print(event_h.relative_humidity);
   BTserial.print(event_h.relative_humidity);    
   Serial.print("%\t");  
   BTserial.print("%\t");
   Serial.print("Temperature: ");
   BTserial.print("Temperature: ");  
   Serial.print(event_t.temperature);
   BTserial.print(event_t.temperature);
   Serial.println(" C");
   BTserial.println(" C");
   
   digitalWrite(trigPin, LOW);
   digitalWrite(echoPin, LOW);
   delayMicroseconds(2);
   digitalWrite(trigPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(trigPin, LOW);
   unsigned long duration = pulseIn(echoPin, HIGH); 
   float distance = ((float)(340 * duration) / 10000) / 2;  
   Serial.print(distance);
   BTserial.print("Distance: ");
   BTserial.print(distance);
   Serial.println("cm");
   BTserial.println("cm");
   if(distance<200){
    digitalWrite(10,HIGH);
    delay(1500);
    digitalWrite(10,LOW);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Warning!");
    delay(1000);
   }
   static double oldValue = 0;
   int rawValue = analogRead (sensorPin); 
   double value = alpha * oldValue + (1 - alpha) * rawValue;
   BTserial.print("HeartBeating(Per Minute): ");
   Serial.print (rawValue);
   BTserial.print(rawValue);
   Serial.print (", ");
   BTserial.print(", ");
   Serial.println (value);
   BTserial.println(value);
   oldValue = value;
   int x = analogRead(xPin);
   int y = analogRead(yPin); 
   int z = analogRead(zPin);
   float zero_G = 512.0;              
   float scale = 102.3;
   int x1=((float)x - 331.5)/65*9.8;
   int y1=((float)y - 329.5)/68.5*9.8;
   int z1=((float)z - 340)/68*9.8;
   BTserial.print("X: ");
   Serial.print(x1);  
   Serial.print("\t");
   BTserial.print(x1);  
   BTserial.print("\t");
   BTserial.print("Y: ");
   Serial.print(y1); 
   Serial.print("\t");
   BTserial.print(y1); 
   BTserial.print("\t");
   BTserial.print("Z: ");
   Serial.print(z1);  
   Serial.print("\n");
   BTserial.print(z1);  
   BTserial.print("\n");
   if((tmpx+10<x1 || tmpx-10>x1)&&(tmpy+10<y1 || tmpy-10>y1)&&(tmpz+10<z1 || tmpz-10>z1)){
    for(int i=0; i<3; i++){
      digitalWrite(10, HIGH);
      delay(500);
      digitalWrite(10, LOW);
    }
   }
   tmpx=x1, tmpy=y1, tmpz=z1;
   int A5_data = analogRead(A5);
   Serial.print("A5 : ");
   Serial.println(A5_data);
   BTserial.print("B1 : ");
   BTserial.println(A5_data);
   int A6_data = analogRead(A4);9
   Serial.print("A6 : ");
   Serial.println(A6_data);
   BTserial.print("B2 : ");
   BTserial.println(A6_data);
   int A7_data = analogRead(A7);
   Serial.print("A7 : ");
   Serial.println(A7_data);
   BTserial.print("B3 : ");
   BTserial.println(A7_data);
   int leng=80;
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Waiting");
   BTserial.println("Waist length: Off");
   if(A6_data<=500){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Taking Examine");
    delay(3000);
    int rawValue = analogRead (sensorPin); 
    double value = alpha * oldValue + (1 - alpha) * rawValue;
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(rawValue/8);
    delay(1000);
   }
   if(A7_data<=500){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temp: ");
    lcd.setCursor(6,0);
    lcd.print(event_t.temperature);
    lcd.setCursor(0,1);
    lcd.print("Hum: ");
    lcd.setCursor(5, 1);
    lcd.print(event_h.relative_humidity);
    delay(2500);
   }
   if(A5_data<=500){

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("WAIST_LENGTH: ");
    lcd.setCursor(0,1);
    lcd.println(leng);
    BTserial.println("Waist length: On");
    while(1){
      int A7_data = analogRead(A7);
      A6_data = analogRead(A4);
      A7_data = analogRead(A7);
      
     if(A6_data<=500){
      leng++;

      lcd.setCursor(0,1);
      lcd.print(leng);
      
      delay(100);
     }
     if(A7_data<=500){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Success!");
      delay(1000);
      lcd.clear();
      break;
     }
    } 
   }
}
