#include <JCARMI.h>
#define joyHorizontalPin  4
#define joyVerticalPin  5
#define joyRotatePin  1
#define joyClawPin  2

float PosHorizontal = 100;
float PosVertical = 100;
int angRotate = 100;
int angClaw = 30;

void setup() {  
  Serial.begin(9600);
  pinMode(A1, INPUT_PULLUP);
  jcArmI.begin(11, 10, 9, 8);
  jcArmI.moveClawTo(PosHorizontal, PosVertical);
}

void loop() {
  int Jx = analogRead(joyHorizontalPin);
  int Jy = analogRead(joyVerticalPin);
  double Jr = float(analogRead(joyRotatePin));
  int Jc = analogRead(joyClawPin);

  if (Jx > 700) {
    Jx = -1;
  } else if (Jx < 300) {
    Jx = 1;
  } else if ( (Jx < 700) && (Jx > 300) ) {
    Jx = 0;
  }
  if (Jy > 700) {
    Jy = -1;
  } else if (Jy < 300) {
    Jy = 1;
  } else if ( (Jy < 700) && (Jy > 300) ) {
    Jy = 0;
  }
  if (Jr > 700) {
    Jr = 0.3;
  } else if (Jr < 300) {
    Jr = -0.3 ;
  } else if ( (Jr < 700) && (Jr > 300) ) {
    Jr = 0.0;
  }
  if (Jc > 700) {
    Jc = 1;
  } else if (Jc < 300) {
    Jc = -1;
  } else if ( (Jc < 700) && (Jc > 300) ) {
    Jc = 0;
  }

  if (angClaw >= 0 && angClaw <= 60) {
    angClaw += Jc;
  } else if (angClaw < 0) {
    angClaw = 0;
  } else if (angClaw > 60) {
    angClaw = 60;
  }
  
  jcArmI.grab(angClaw);

  if (angRotate >= 0 && angRotate <= 180) {
    angRotate = angRotate + float(Jr);
  } else if (angRotate < 0.0) {
    angRotate = 0.0;
  } else if (angRotate > 180.0) {
    angRotate = 180.0;
  }

  if (jcArmI.isPointInPolygon(PosHorizontal + Jx , PosVertical + Jy )) {
    Serial.println("Posição Válida");
    PosHorizontal += Jx;
    PosVertical += Jy;
    jcArmI.moveClawTo(PosHorizontal, PosVertical);
  } else {
    Serial.println("ERRO - INVALIDO");
  }
  
}
