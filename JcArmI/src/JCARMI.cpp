#include "JCARMI.h"

JCARMI::Point JCARMI::vertices[] = {
{20,160},
{90,160},
{145,110},
{170,70},
{180,40},
{185,0},
{165,-40},
{130,-70},
{85,-70},
{85,-50},
{45,-10},
{45,0},
{25,0},
{25,20},
{20,20},
{20,160},
};

int JCARMI::nVertices = sizeof(JCARMI::vertices) / sizeof(JCARMI::vertices[0]);


void JCARMI::begin(int servoVerticalPin, int servoHorizontalPin, int servoBasePin, int servoClawPin) {
	this->servoVerticalPin = servoVerticalPin;
    this->servoHorizontalPin = servoHorizontalPin;
    this->servoBasePin = servoBasePin;
    this->servoClawPin = servoClawPin;
    this->servoVertical.attach(servoVerticalPin);
    this->servoHorizontal.attach(servoHorizontalPin);
    this->servoBase.attach(servoBasePin);
    this->servoClaw.attach(servoClawPin);
}

bool JCARMI::isPointInPolygon(double x, double y) {	
	Point p(x,y);
    bool inside = false;
    for (int i = 0, j = nVertices - 1; i < nVertices; j = i++) {
        if (((vertices[i].y > p.y) != (vertices[j].y > p.y)) && (p.x <= (vertices[j].x - vertices[i].x) * (p.y - vertices[i].y) / (vertices[j].y - vertices[i].y) + vertices[i].x))
            inside = !inside;
    }
    return inside;
}

void JCARMI::rotateBaseTo(int ang) {
    servoBase.write(ang);
}

void JCARMI::grab(int ang) {
    servoClaw.write(ang);
}


void JCARMI::moveClawTo(double x, double y) {
  int fc_a1 = 5; //Definir a partir da calibração com a1 em 90º (vertical)
  int fc_a2 = 0 ; //Definir a partir da calibração com a2 em 0º (vertical)
  int atraso = 5;
  int passo = 3;
  float L1 = 95;
  float L2 = 90;
  float pi = M_PI;
  float q;
  float h;
  float phi;
  float theta;
  float alpha;
  float a1;
  float a2;
  float angle_motor1;
  float angle_motor2;
  if (isPointInPolygon(x,y)) {
  h = sqrt(sq(x) + sq(y));
  phi = (atan2(y, x)) * (180 / pi);
  theta = (acos((sq(L2) - sq(L1) - sq(h)) / (-2 * L1 * h))) * (180 / pi);
  alpha = (acos((sq(h) - sq(L1) - sq(L2)) / (-2 * L1 * L2))) * (180 / pi);
  a1 = phi + theta;
  a2 = abs(180 - a1 - alpha);
  q = L1 * sin(a1 * (pi / 180));
  if (q >= y) {
    a2 = -a2;
  }
  angle_motor1 = 180 - a1 + fc_a1;
  angle_motor2 = 90 + a2 + fc_a2;
if (angle_motor1 >= 30 && angle_motor1 <= 180 && angle_motor2 >= 0 && angle_motor2 <= 150) {
    servoVertical.write(angle_motor1);
    servoHorizontal.write(angle_motor2);
    posHorizontal = x;
	posVertical = y;
 }
  delay(10);
  }
}


float JCARMI::getVerticalPos() {
    return posVertical;
}

float JCARMI::getHorizontalPos() {
    return posHorizontal;
}

JCARMI &JCARMI::getInstance() {
  static JCARMI instance;
  return instance;
}

JCARMI &jcArmI = JCARMI::getInstance();
