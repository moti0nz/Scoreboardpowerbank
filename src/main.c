#include <SPI.h>
#include <Wire.h>
#include <DMD2.h>
#include <fonts/BigNumberFull.h>
#include <fonts/System_Mid5x7.h>
#include <fonts/Mono5x7.h>
#include <fonts/System4x7.h>
#include <SoftwareSerial.h>
#include <String.h>
SoftwareSerial bluetooth (3,2); //pass = "1234"

int Lscore = 0;
int Rscore = 0;
int Round = 1;
int Lfoul = 0;
int Rfoul = 0;
const unsigned long tInterval = 1000UL;
unsigned long pTime;
unsigned long cTime;
int menit = 0;
int saveMenit = 0;
int detik = 7200;
int waktuHabis = 0;
int displayMenit;
int displayDetik;
boolean countStart = true;
char BT;
char dmdBuff[10];
char dmdBuff2[2];
SoftDMD dmd (2,1);
int kecerahan = 20;

void setup() {
  dmd.setBrightness(kecerahan);
  dmd.begin();
  Serial.begin(57600);
  detik += 60 * menit;
  bluetooth.begin(57600);
  showScore();
}

void showScore(){
    dmd.selectFont(BigNumber);
    sprintf(dmdBuff, "%.2d", Lscore);
    dmd.drawString(1, 0, dmdBuff);
    dmd.selectFont(Mono5x7);
    sprintf(dmdBuff, "T1-T2");
    dmd.drawString(18, 9, dmdBuff);

    dmd.selectFont(System_Mid5x7);

    dmd.selectFont(BigNumber);
    sprintf(dmdBuff, "%.2d", Rscore);
    dmd.drawString(50, 0, dmdBuff);
    showTimer();
}

void resetScore(){
  Lscore = 0;
  Rscore = 0;
  waktuHabis = 0;
  menit = saveMenit;
  showScore();
}

void syncApp(){
  String dataValue = String(Lscore) + "," + String(Lfoul) + "," + String(Rscore) + "," + String(Rfoul) + "," + String(Round) + "," + String(waktuHabis) + ",";
  //bluetooth.println(dataValue);
  //Serial.println(dataValue);
}

void MaxScore(){
  if (BT == 'A' && Lscore > 99){
    Lscore = 99;
  }
  if (BT == 'E' && Rscore > 99){
    Rscore = 99;
  }
  if (BT == 'C' && Lfoul > 9){
    Lfoul = 9;
  }
  if (BT == 'G' && Rfoul > 9){
    Rfoul = 9;
  }
  if (BT == 'I' && Round > 2){
    Round = 2;
  }
  showScore();
}

void MinScore(){
  if (BT == 'B' && Lscore < 0){
    Lscore = 0;
  }
  if (BT == 'F' && Rscore < 0){
    Rscore = 0;
  }
  if (BT == 'D' && Lfoul < 0){
    Lfoul = 0;
  }
  if (BT == 'H' && Rfoul < 0){
    Rfoul = 0;
  }
  if (BT == 'J' && Round < 0){
    Round = 0;
  }
  showScore();
}

void timeOut(){
  countStart = false;
  detik = 0;
  waktuHabis = 1;
  syncApp();
}

void showTimer(){
  dmd.selectFont(System4x7);
  displayMenit = detik/60;
  displayDetik = detik % 60; //modulo
  sprintf(dmdBuff, "%.2d", displayMenit);
  sprintf(dmdBuff2, "%.2d", displayDetik);
  dmd.drawFilledBox(22,0,43,6, GRAPHICS_OFF);
  dmd.drawString(20, -1, String(dmdBuff) + ":" + String(dmdBuff2));
}

void loop() {
  while(bluetooth.available()){
    BT = bluetooth.read();
    
    Serial.println(BT);
    switch(BT){
      case 'A':
        Lscore++;
        MaxScore();
        syncApp();
        showScore();
        break;
      case 'B':
        Lscore--;
        MinScore();
        syncApp();
        showScore();
        break;
      case'C':
        Lfoul++;
        MaxScore();
        syncApp();      
        showScore();
        break;
      case 'D':
        Lfoul--;
        MinScore();
        syncApp();      
        showScore();       
        break;
      case 'E':
        Rscore++;
        MaxScore();
        syncApp();
        showScore();
        break;
      case 'F':
        Rscore--;
        MinScore();
        syncApp();
        showScore();
        break;   
      case 'G':
        Rfoul++;
        MaxScore();
        syncApp();
        showScore();        
        break;
      case 'H':
        Rfoul--;
        MinScore();
        syncApp();
        showScore();
        break;
      case 'I':
        Round++;
        MaxScore();
        syncApp();
        showScore();
        break;
      case 'J':
        Round--;
        MinScore();
        syncApp();
        showScore();
        break;
      case 'K':
        resetScore();
        syncApp();
        break;
      case 'N':
        syncApp();
        break;
      case 'O':
        countStart = true;
        break;
      case 'P':
        countStart = false;
        break;
      case 'Q':
        menit++;
        if(menit > 99) menit = 99;
        saveMenit = menit;
        detik = 0;
        detik += 60 * menit;
        showTimer();
        break;
      case 'R':
        menit--;
        if(menit < 0) menit = 0;
        saveMenit = menit;
        detik = 0;
        detik += 60 * menit;
        showTimer();
        break;
      case 'L':
        kecerahan += 10;
        if(kecerahan > 50){
          kecerahan = 50;
        }
        dmd.setBrightness(kecerahan);
        break;
      case 'M':
        kecerahan -= 10;
        if(kecerahan < 0){
          kecerahan = 0;
        }
        dmd.setBrightness(kecerahan);
        break;
    }
  }
  
  cTime = millis();
  if(cTime - pTime >= tInterval){  
    if(countStart == true){
      pTime = cTime;
      detik--;
      if(detik < 0) timeOut();
    showTimer();
    }
    pTime = cTime;
  }
}