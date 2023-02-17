#include <Stepper.h>
#include "Wire.h"
#include "LiquidCrystal.h"
#include "DHT.h"
#include "stdio.h"
#include "Servo.h"
#define LDR A0

LiquidCrystal LCD(31,30,29,28,27,26,25,24,23,22);
DHT dht(32, DHT11);
DHT dhtExt(33, DHT11);
const int stepsPerRevolution = 200; 
Servo servo;
Servo servo1;
Stepper myStepper(stepsPerRevolution, 4, 5, 6, 7);
Stepper myStepper2(stepsPerRevolution, 11, 10, 9, 8);
int stepcount = 0;
int indic = 6;

int ctr = 0;

int listeCheck [12] = {0,0,0,0,0,0,0,0,0,0,0,0};



void setup() {
  Serial.begin(9600);
  myStepper.setSpeed(100); 
  servo.attach(2);
  servo1.attach(3);
  LCD.begin(16,2);
  dht.begin();
  dhtExt.begin();
  pinMode(LDR, INPUT);
  
  

}

void loop() {
  temp();
  delay(500);
  checkLum();
  checkRotation();
  
    
    
  
}

void temp() {
   float h = dhtExt.readTemperature();
   float t = dht.readTemperature();

   LCD.setCursor(0,0);
   LCD.print("Temp. Ext: ");
   LCD.print(h);

   LCD.setCursor(0,1);
   LCD.print("Temp.: ");
   LCD.print(t);
   if (t < h || t > h) {
     servo.write(-180);
     servo1.write(-180);
   } else {
     servo.write(180);
     servo1.write(180);
   }

   
  
}
void checkLum() {
  
  for (int i = 0; i < 12; i++) {
      myStepper.step(80);
      delay(500);
      setListeCheck(i,int(analogRead(A0)));
      delay(500);
  }
  delay(1500);
  myStepper.step(-960);  
  
    
}
int checkRotation() {
  int z = getListeCheck(0);
  for (int y = 0; y < 12; y++) {
    int w = getListeCheck(y);
    if (z < w) {
      z=w;
      indic = indic + y - indic;
    }
  }
  int indice = indic*30;
  myStepper2.setSpeed(30);
  myStepper2.step(indice);
  delay(5000);
  myStepper2.step(-indice);
  delay(1000);
  Serial.println(indic);
  Serial.println(z);
}

int setListeCheck (int i, int valeur ){
  listeCheck [i] = valeur;
  
  return valeur; 
}
int getListeCheck(int y) {
  return listeCheck[y];
}

