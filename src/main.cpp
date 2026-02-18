#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "logo.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Encoder ve Buzzer
#define ENCODER_DT 2
#define ENCODER_CLK 3
#define ENCODER_SW 4
#define BUZZER_PIN 30

// Menü başlıkları
const char* menuItems[] = {
  "Hareket S. Kablosu",
  "Heater Data Kablosu",
  "Loadcell Kablosu",
  "Z-Ref Kablosu",
  "Ortam Sensoru",
  "Y-Ref Kablosu",
  "IR Temp Sensor",
  "Projektor Data",
  "PC-Led Kablosu",
  "CVR1 Limit Test",
  "CVR2 Limit Test"
};
const int menuLength = sizeof(menuItems)/sizeof(menuItems[0]);
int menuIndex = 0;
#define MAX_VISIBLE_MENU_ITEMS 6
int startMenuIndex = 0;

int lastCLK = HIGH;
bool buttonPressed = false;
unsigned long lastEncoderMoveTime = 0;
const int debounceDelay = 5;

// Pin dizileri
const int hareketSendPins[6] = {22, 23, 24, 25, 26, 27};
const int hareketReceivePins[6] = {40, 41, 42, 43, 44, 45};

const int heaterSendPins[3] = {22, 23, 24};
const int heaterReceivePins[3] = {40, 41, 42};

const int loadcellSendPins[6] = {22, 23, 24, 25, 26, 27};
const int loadcellReceivePins[6] = {40, 41, 42, 43, 44, 45};

const int zrefSendPins[3] = {22, 23, 24};
const int zrefReceivePins[3] = {40, 41, 42};

const int ortamSendPins[6] = {22, 23, 24, 25, 26, 27};
const int ortamReceivePins[6] = {40, 41, 42, 43, 44, 45};

const int yrefSendPins[4] = {22, 23, 24, 25};
const int yrefReceivePins[4] = {40, 41, 42, 43};

const int irtempSendPins[5] = {22, 23, 24, 25, 26};
const int irtempReceivePins[5] = {40, 41, 42, 43, 44};

const int projektorSendPins[7] = {22, 23, 24, 25, 26, 27, 28};
const int projektorReceivePins[7] = {40, 41, 42, 43, 44, 45, 46};

const int pcLedSendPins[4] = {22, 23, 24, 25};
const int pcLedReceivePins[4] = {40, 41, 42, 43};

// CVR buton pinleri
#define CVR1_BUTTON_TOP 11
#define CVR1_BUTTON_BOTTOM 10
#define CVR2_BUTTON_TOP 13
#define CVR2_BUTTON_BOTTOM 12

#define MAX_VISIBLE_TEST_LINES 5
String testResults[20];
int testLineCount = 0;

bool isTestRunning = false;
bool isCVRTest = false;

// Fonksiyonlar
void playOpeningMelody() {
  int melody[] = {659, 659, 0, 659, 0, 523, 659, 0, 784, 0, 0, 0, 392, 0, 0, 0};
  int noteDurations[] = {150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150};
  for(int i=0;i<sizeof(melody)/sizeof(int);i++){
    if(melody[i]==0) noTone(BUZZER_PIN);
    else tone(BUZZER_PIN, melody[i], noteDurations[i]);
    delay(noteDurations[i]+50);
  }
  noTone(BUZZER_PIN);
}

void printCentered(const String& text, int y){
  int16_t x = (128 - text.length()*6)/2;
  display.setCursor(x,y);
  display.println(text);
}

void setup() {
  pinMode(ENCODER_DT, INPUT);
  pinMode(ENCODER_CLK, INPUT);
  pinMode(ENCODER_SW, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(CVR1_BUTTON_TOP, INPUT_PULLUP);
  pinMode(CVR1_BUTTON_BOTTOM, INPUT_PULLUP);
  pinMode(CVR2_BUTTON_TOP, INPUT_PULLUP);
  pinMode(CVR2_BUTTON_BOTTOM, INPUT_PULLUP);

  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    Serial.println(F("OLED bulunamadı!"));
    while(1);
  }

  display.clearDisplay();
  display.drawBitmap(0,0,logo,128,64,SSD1306_WHITE);
  display.display();
  playOpeningMelody();
  delay(1000);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  menuIndex=0;
  startMenuIndex=0;
  showMenu();
}

void loop(){
  if(!isTestRunning){
    readEncoder();
    checkButton();
  } else {
    if(menuIndex==9) readCVR1Buttons();
    else if(menuIndex==10) readCVR2Buttons();

    if(digitalRead(ENCODER_SW)==LOW && !buttonPressed){
      tone(BUZZER_PIN,1500,100);
      isTestRunning=false;
      isCVRTest=false;
      showMenu();
      delay(200);
    }
    if(digitalRead(ENCODER_SW)==HIGH) buttonPressed=false;
  }
}

void readEncoder(){
  unsigned long now=millis();
  int currentCLK=digitalRead(ENCODER_CLK);
  if(currentCLK!=lastCLK && currentCLK==LOW){
    if(now-lastEncoderMoveTime>debounceDelay){
      if(digitalRead(ENCODER_DT)!=currentCLK) menuIndex=(menuIndex+1)%menuLength;
      else menuIndex=(menuIndex-1+menuLength)%menuLength;
      showMenu();
      lastEncoderMoveTime=now;
    }
  }
  lastCLK=currentCLK;
}

void checkButton(){
  if(digitalRead(ENCODER_SW)==LOW && !buttonPressed){
    buttonPressed=true;
    tone(BUZZER_PIN,2000,100);
    handleSelection(menuIndex);
    delay(200);
  }
  if(digitalRead(ENCODER_SW)==HIGH) buttonPressed=false;
}

void showMenu(){
  display.clearDisplay();
  printCentered("Kablo Test Menusu",0);
  int yOffset=16;
  if(menuIndex<startMenuIndex) startMenuIndex=menuIndex;
  else if(menuIndex>=startMenuIndex+MAX_VISIBLE_MENU_ITEMS) startMenuIndex=menuIndex-MAX_VISIBLE_MENU_ITEMS+1;

  for(int i=startMenuIndex;i<startMenuIndex+MAX_VISIBLE_MENU_ITEMS && i<menuLength;i++){
    display.setCursor(0,yOffset);
    display.print((i==menuIndex)?"> ":"  ");
    display.println(menuItems[i]);
    yOffset+=8;
  }
  display.display();
}

void handleSelection(int index){
  isTestRunning = true;
  isCVRTest = (index==9 || index==10);
  if(isCVRTest) return;

  int* sendPins;
  int* receivePins;
  int pinCount;

  switch(index){
    case 0: sendPins=(int*)hareketSendPins; receivePins=(int*)hareketReceivePins; pinCount=6; break;
    case 1: sendPins=(int*)heaterSendPins; receivePins=(int*)heaterReceivePins; pinCount=3; break;
    case 2: sendPins=(int*)loadcellSendPins; receivePins=(int*)loadcellReceivePins; pinCount=6; break;
    case 3: sendPins=(int*)zrefSendPins; receivePins=(int*)zrefReceivePins; pinCount=3; break;
    case 4: sendPins=(int*)ortamSendPins; receivePins=(int*)ortamReceivePins; pinCount=6; break;
    case 5: sendPins=(int*)yrefSendPins; receivePins=(int*)yrefReceivePins; pinCount=4; break;
    case 6: sendPins=(int*)irtempSendPins; receivePins=(int*)irtempReceivePins; pinCount=5; break;
    case 7: sendPins=(int*)projektorSendPins; receivePins=(int*)projektorReceivePins; pinCount=7; break;
    case 8: sendPins=(int*)pcLedSendPins; receivePins=(int*)pcLedReceivePins; pinCount=4; break;
    default: return;
  }

  for(int i=0;i<pinCount;i++){
    pinMode(sendPins[i],OUTPUT);
    digitalWrite(sendPins[i],LOW);
    pinMode(receivePins[i],INPUT);
  }

  delay(10);
  testLineCount=0;

  // Pin testi (karşılık gelen pin ile)
  for(int i=0;i<pinCount;i++){
    digitalWrite(sendPins[i],HIGH);
    delay(10);

    int val1=digitalRead(receivePins[i]);
    delayMicroseconds(500);
    int val2=digitalRead(receivePins[i]);
    int val3=digitalRead(receivePins[i]);

    String line="Pin "+String(i+1)+": ";
    if(val1==HIGH && val2==HIGH && val3==HIGH) line+="DOGRU";
    else line+="Kopuk!";

    testResults[testLineCount++]=line;
    digitalWrite(sendPins[i],LOW);
  }

  // Ekrana yazdır
  display.clearDisplay();
  display.setCursor(0,0);
  for(int i=0;i<MAX_VISIBLE_TEST_LINES && i<testLineCount;i++){
    display.println(testResults[i]);
  }
  display.display();

  delay(3000);
  isTestRunning=false;
  showMenu();
}

void readCVR1Buttons(){
  bool topPressed = digitalRead(CVR1_BUTTON_TOP)==LOW;
  bool bottomPressed = digitalRead(CVR1_BUTTON_BOTTOM)==LOW;

  display.clearDisplay();
  printCentered("CVR1 Limit Test",0);
  display.setCursor(0,20);
  display.print("Top: "); display.println(topPressed?"Basilmadi":"Basildi");
  display.print("Bottom: "); display.println(bottomPressed?"Basilmadi":"Basildi");
  printCentered("Menu",56);
  display.display();
}

void readCVR2Buttons(){
  bool topPressed = digitalRead(CVR2_BUTTON_TOP)==LOW;
  bool bottomPressed = digitalRead(CVR2_BUTTON_BOTTOM)==LOW;

  display.clearDisplay();
  printCentered("CVR2 Limit Test",0);
  display.setCursor(0,20);
  display.print("Top: "); display.println(topPressed?"Basilmadi":"Basildi");
  display.print("Bottom: "); display.println(bottomPressed?"Basilmadi":"Basildi");
  printCentered("Menu",56);
  display.display();
}