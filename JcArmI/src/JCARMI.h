#ifndef JCARMI_h
#define JCARMI_h
#include "Arduino.h"
#include <Servo.h>

class JCARMI {
public:
	static JCARMI &getInstance(); // Accessor for singleton instance
	JCARMI(const JCARMI &) = delete; // no copying
	JCARMI &operator=(const JCARMI &) = delete; //proíbe a atribuição de uma instância a outra instância

  void moveClawTo(double x, double y);
  void rotateBaseTo(int ang);
  void grab(int ang);
  void begin(int servoVerticalPin, int servoHorizontalPin, int servoBasePin, int servoClawPin);
  
  struct Point {
    double x;
    double y;
    Point() {}
    Point(double x, double y) : x(x), y(y) {}
  };
  
  float getVerticalPos();
  float getHorizontalPos();
  
  bool isPointInPolygon(double x, double y);	  

private:
  JCARMI() = default; 	
  Servo servoHorizontal;
  Servo servoVertical;
  Servo servoClaw;
  Servo servoBase;
  
  float posHorizontal;
  float posVertical;  
  
  static Point vertices[16];
  static int nVertices;    
  
  int servoVerticalPin;
  int servoHorizontalPin;
  int servoBasePin;
  int servoClawPin;  
  
};
extern JCARMI &jcArmI;
#endif
