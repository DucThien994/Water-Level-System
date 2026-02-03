#include "Ultrasonic.h"

// hàm cấu hình cho các chân của cảm biến
Ultrasonic::Ultrasonic(int trig, int echo) {
  trigPin = trig;
  echoPin = echo;
}

// hàm khởi tạo cho cảm biến 
void Ultrasonic::begin() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW);
  delay(100);
  Serial.println("Ultrasonic sensor initialized");
}

// hàm đọc dữ liệu và biến đổi dữ liệu của cảm biến 
float Ultrasonic::readWaterLevel(float maxDepth) {

  // kích hoạt sr04m hoạt động
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); // 2ms
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);// 10ms
  digitalWrite(trigPin, LOW);

  //tránh treo khi không có tín hiệu phản hồi cho SR04M (khoảng cách quá xa)
  unsigned long timeout = micros() + 40000; // 40ms timeout
  unsigned long startTime = 0;
  unsigned long endTime = 0;

  // Chờ ECHO HIGH
  while (digitalRead(echoPin) == LOW && micros() < timeout);
  if (micros() >= timeout) return -1;
  startTime = micros();

  // Chờ ECHO LOW
  while (digitalRead(echoPin) == HIGH && micros() < timeout);
  if (micros() >= timeout) return -1;
  endTime = micros();

  long duration = endTime - startTime;
  if (duration <= 0 || duration > 35000) return -1;

  /*
  công thức tính khoảng cách từ cảm biến đến mặt nước 
  2S = v*t 
  S = (v*t)/2 
  v tốc độ âm thanh 
  t là thời gian đi và về của cảm biến
  */
  float distance = duration * 0.0343 / 2.0;
  
  // lọc nhiễu 
  static float lastValid = 100.0;
  if (distance > 2 && distance < 400) {
    lastValid = distance;
  }
  
  float waterLevel = 150.0 - lastValid; // Chiều cao cảm biến
  if (waterLevel < 0) waterLevel = 0;
  if (waterLevel > maxDepth) waterLevel = maxDepth;

  return waterLevel;
}