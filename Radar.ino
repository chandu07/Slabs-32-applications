#include <Servo.h>
#include <TFT_ST7735.h>

TFT_ST7735 tft = TFT_ST7735();

#define BLACK 0x0000
#define WHITE 0xFFFF
#define GREY  0x5AEB
#define RED  0x001F
#define GREEN  0x07E0

const int trigPin = 4;
const int echoPin = 5;

int16_t h = 128;
int16_t w = 160;

long duration;
int distance;

Servo Rservo;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Rservo.attach(2);
  tft.init();
  tft.setRotation(1);
  tft.invertDisplay(true);
  tft.setTextColor(WHITE,GREY);
  setupScreen();
  Serial.begin(115200);
}

void setupScreen(){
  tft.fillScreen(BLACK);
  tft.drawCircle(w/2,h,w/2+20,WHITE);
  tft.drawCircle(w/2,h,w/2-20,WHITE);
  tft.drawCircle(w/2,h,w/2-60,WHITE);
}

void loop() {
  for(int i=0;i<=180;i++){  
    Rservo.write(i);
    delay(10);
    distance = calculateDistance();
    if(distance>150){
    distance = 150;}
    udateScreen(distance, i);
//    Serial.println(distance);
  }
  setupScreen();

  for(int i=180;i>0;i--){  
    Rservo.write(i);
    delay(10);
    distance = calculateDistance();
    if(distance>150){
    distance = 150;}
    udateScreen(distance, i);
//    Serial.println(distance);
  }
  setupScreen();

}

int calculateDistance(){ 
  
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;
  return distance;
}

void udateScreen(int distance, int angle){
  float sin_val = sin(angle*(3.14/180));
  float cos_val = cos(angle*(3.14/180));

  distance = map(distance,0,150,30,100);
  tft.drawPixel(80-(100*cos_val),128-(100*sin_val),WHITE);
  tft.drawPixel(80-(60*cos_val),128-(60*sin_val),WHITE);
  tft.drawPixel(80-(20*cos_val),128-(20*sin_val),WHITE);
  tft.drawLine(80,128,80-(100*cos_val),128-(100*sin_val),GREEN);

  tft.fillCircle(80-(distance*cos_val),128-(distance*sin_val),2,RED);
  
  }
