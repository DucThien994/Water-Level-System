#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>

class Ultrasonic {
private:
  int trigPin; // chân kích
  int echoPin; // chân đo

public:
  Ultrasonic(int trig, int echo); // contructor, khi gọi object thuộc class thì tính chất của constructor này sẽ theo object
  void begin();
  float readWaterLevel(float maxDepth);
};

#endif