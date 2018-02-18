//  Created by Marc Mateu Mateus on 16/10/17.
//  Universitat Politécnica de Catalunya
//  Departament d'Enginyeria Electrónica
//  Grup d'Instrumentació Electrónica i Biomédica

#include "BluetoothSerial.hpp"


#define N 100

#include <Adafruit_NeoPixel.h>
 
#define PIN 8 //INTERNAL NEOPIXEL
// #define PIN 6 //EXTERNAL NEOPIXEL
#define trigPinR 6
#define echoPinR 12
#define vibratorR 3

float vibratorROut = 0;
float temp = 20;
float spdSnd;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);
BSerial *bls; 

void setup() {
  // put your setup code here, to run once  
  bls = new BSerial();
  strip.begin();

  pinMode(trigPinR, OUTPUT);
  pinMode(echoPinR, INPUT);
  
  Serial.begin (9600);
  Serial.print("Distance = ");
}


void loop() {
   spdSnd = 331.4 + (0.606 * temp) + 0.62;
   
// blindEye Right
  digitalWrite(trigPinR, LOW); 
  delayMicroseconds(2);

  digitalWrite(trigPinR, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinR, LOW);

  float durationR, distanceR;
  durationR = pulseIn(echoPinR, HIGH);
  distanceR = (durationR / 2.0) * (spdSnd / 10000);

  delay(5);
  vibratorROut=((distanceR-4.0)/(-4.0/255.0))/100.0;
  analogWrite(vibratorR, vibratorROut);
  delay(2);
  Serial.print("Distance = ");

// bluetooth  
  char buff[N];
  char *cmd, *val;
  uint16_t f,ss,ds,ns;
  int32_t r = bls->read_line(buff,N);
  if(r < 0)
    return;

  uint16_t d = bls->findFirstChar(buff,r+1,';');
 
  cmd = bls->substringFromString(buff,0,d,&ss); //getting the command
 
  val = bls->substringFromString(buff,d+1,r,&ss); //getting the value

  Serial.print("Distance = ");
  
  switch(cmd[0]){ //WARNING all the commands must have 1 character, otherwise the switch function is not good.
     case 'N': //N command
     {
        uint16_t dd = bls->findFirstChar(val,ss,';');
        int16_t neo = atoi(bls->substringFromString(val,0,dd,&ds));
        if(neo == -1){
            for(uint16_t i=0; i<strip.numPixels(); i++) {
                strip.setPixelColor(i, 0);
            }
            
        }else{
        //WARNING: from this point the values are hardcoded!
        char nval[15];
        strncpy(nval,val+d+1,11);
       
        char color[3];
        strncpy(color,nval,3);
        uint8_t R = atoi(color);
        strncpy(color,nval+4,3);
        uint8_t G = atoi(color); 
        strncpy(color,nval+8,3);
        uint8_t B = atoi(color);
         //WARNING all the values must have 3 characters!
//        uint8_t R = atoi(bls->substringFromString(nval,0,3,&ns));
//        uint8_t G = atoi(bls->substringFromString(nval,4,7,&ns));
//        uint8_t B = atoi(bls->substringFromString(nval,8,11,&ns));
        strip.setPixelColor(neo, strip.Color(R, G, B));

        }
        strip.show();
     }
     case 'F':
      Serial.print("FUCK YOU");
      break;   
  }
  delay(1);
  free(val);
  free(cmd);  
}
