#include "main.h"
#include <Arduino.h>
#include <Wire.h>

#include "WIFI.h"
#include "Ultrasonic.h"
#include "oled.h"
#include "RTC.h"
#include "Website.h"

#define CORE_0 0
#define CORE_1 1

#define SDA_PIN 33  //21
#define SCL_PIN 32  //22
#define Transitor_Base 13

// Khai báo object cúa class
WIFI_MANAGER wifi;
Ultrasonic sensor(TRIG_PIN, ECHO_PIN);
OLED_DISPLAY oled;
RTC rtc;
ThoiGian dt;

// global var

int RSSI_num;
float mucNuoc;
bool LED_state;
String WiFi_status;
String status = "NORMAL";
float percent;

static unsigned long lastSensorUpdate = 0;
static unsigned long lastTimeUpdate = 0;


TaskHandle_t run_wifi;

WebServer server(80);

void handle_Readings() {
  String data = String(percent, 1) + "," + (LED_state ? "ON" : "OFF") + "," + status;
  server.send(200, "text/plain", data);
}

void VTaskWifi(void *pvParameters) {

  const TickType_t RUN_PERIOD_TICKS = pdMS_TO_TICKS(500);

  for (;;) {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Reconnecting WiFi...");
      wifi.KetNoiWiFi(20);
    }

    RSSI_num = wifi.TinhDoManhCuaWiFi();
    rtc.LayThoiGianTuServer();
    rtc.HienThiThoiGianSerial();

    vTaskDelay(RUN_PERIOD_TICKS);
  }
}
void KhoiTaoHeThong() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000);

  pinMode(ALERT_LED_PIN, OUTPUT);
  digitalWrite(ALERT_LED_PIN, LOW);

  pinMode(Transitor_Base, OUTPUT);
  pinMode(Emitter, OUTPUT);

  Serial.println(F("\n=== SMART WATER LEVEL MONITOR ==="));
  Serial.println("--- System Initialization ---");

  Serial.println("1. Connecting WiFi...");
  wifi.KetNoiWiFi(10000);

  Serial.println("2. Initializing sensor and led...");
  oled.begin();
  sensor.begin();

  Serial.println("3. Initializing RTC...");
  rtc.KhoiTaoRTC();

  Serial.println("4. All components initialized");
  Serial.println(F("=== SYSTEM READY - OLED + WIFI + ULTRASONIC ==="));

  xTaskCreatePinnedToCore(VTaskWifi, "Task Run WIFI", 10000, NULL, 0, &run_wifi, CORE_0);

  server.on("/", []() {
    // Khi client kết nối, hàm này được gọi, và các giá trị global hiện tại
    // sẽ được truyền vào handle_OnConnect
    handle_OnConnect(percent, LED_state, status);
  });
  server.on("/readings", handle_Readings);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP Server started.");
}

void main_loop() {
  server.handleClient();
  digitalWrite(Emitter, LOW);

  if (millis() - lastSensorUpdate >= 500) {
    Serial.println(WiFi.localIP());
    // Đọc mức nước
    mucNuoc = sensor.readWaterLevel(TANK_MAX_DEPTH);
    if (mucNuoc < 0) mucNuoc = 0;
    if (mucNuoc > TANK_MAX_DEPTH) mucNuoc = TANK_MAX_DEPTH;

    // Tính khoảng cách và phần trăm
    float distance = 150.0 - mucNuoc;  // Cảm biến cao 150cm
    percent = (mucNuoc / TANK_MAX_DEPTH) * 100.0;

    // Xác định trạng thái
    if (mucNuoc > HIGH_WATER_LEVEL) {
      status = "HIGH";
      digitalWrite(ALERT_LED_PIN, HIGH);
      LED_state = true;
      digitalWrite(Transitor_Base, HIGH);

    } else if (mucNuoc < LOW_WATER_LEVEL) {
      status = "LOW";
      digitalWrite(ALERT_LED_PIN, HIGH);
      LED_state = true;
      digitalWrite(Transitor_Base, HIGH);
    } else {
      LED_state = false;
      status = "NORMAL";
      digitalWrite(ALERT_LED_PIN, LOW);
      digitalWrite(Transitor_Base, LOW);
    }

    // Hiển thị Serial ========================================
    Serial.print("MUC NUOC: ");
    Serial.print(mucNuoc, 1);
    Serial.print("cm | ");
    Serial.print(percent, 1);
    Serial.print("% | STATUS: ");
    Serial.println(status);

    // Hiển thị OLED
    oled.showWaterLevel(mucNuoc, percent, distance, WiFi.status() == WL_CONNECTED, RSSI_num, status);
    oled.showTime(dt.Ngay, dt.Thang, dt.Gio, dt.Phut);

    lastSensorUpdate = millis();
  }
}
