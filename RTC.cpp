#include "RTC.h"
#include <Wire.h>
#include <WiFi.h>

RTC::RTC()
  : timeClient(ntpUDP, "asia.pool.ntp.org", 25200, 60000) {}  // 7 * 3600 = 25200

void RTC::KhoiTaoRTC(void) {
  timeClient.begin();
  Serial.println("RTC da duoc khoi tao");
}

// 5phut 1 lan
void RTC::LayThoiGianTuServer(void) {
  timeClient.update();
  // Lấy thời gian trực tiếp từ NTP client
  dt.Gio = timeClient.getHours();
  dt.Phut = timeClient.getMinutes();
  dt.Giay = timeClient.getSeconds();

  // Lấy ngày tháng năm từ epoch time
  unsigned long epochTime = timeClient.getEpochTime();  // epochTime số giây trôi qua từ 00:00:00 01/01/1970
  struct tm *ptm = gmtime((time_t *)&epochTime);        // dựa vào epochtime tính ra ngày tháng năm thứ // struct tm_mday
  dt.Ngay = ptm->tm_mday;
  dt.Thang = ptm->tm_mon + 1;    // tính từ month 0-11
  dt.Nam = ptm->tm_year + 1900;  // gmtime trả ra year kể từ năm 1900 2025 - 1900 = 125
  dt.Thu = ((ptm->tm_wday == 0) ? 7 : ptm->tm_wday) +1;

  HienThiThoiGianSerial();
}

//======================HIENTHITHOIGIAN 2-------------
void RTC::HienThiThoiGianSerial(void) {
  static unsigned long lastPrint = millis();
  
  if (millis() - lastPrint >= 6000) {
    Serial.print("THOI GIAN: ");
    Serial.print(dt.Gio);
    Serial.print(":");
    Serial.print(dt.Phut);
    Serial.print(":");
    Serial.print(dt.Giay);
    Serial.print(" ");
    Serial.print(dt.Ngay);
    Serial.print("/");
    Serial.print(dt.Thang);
    Serial.print("/");
    Serial.print(dt.Nam);
    Serial.print(" (Thu ");
    if(dt.Thu == 8){
      String S_thu = "Chu Nhat";
      Serial.print(S_thu);
    }
    else Serial.print(dt.Thu);
   
    Serial.println(")");

    lastPrint = millis();
  }
}

String RTC::LayChuoiThoiGian(void) {
  static char buf[30];
  static unsigned long last = millis();
  if (millis() - last > 1000) {
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d %02d/%02d/%04d",
             dt.Gio, dt.Phut, dt.Giay, dt.Ngay, dt.Thang, dt.Nam);
    last = millis();
  }
  return String(buf);
}

//
//===========================================lay thoi gian thuc=====================================
int RTC::day(unsigned long t) {
  return (((t / 86400L) + 4) % 7) + 1;  // 0 là Chủ nhật
}

int RTC::month(unsigned long t) {
  struct tm *tm = gmtime((time_t *)&t);
  return tm->tm_mon + 1;
}

int RTC::year(unsigned long t) {
  struct tm *tm = gmtime((time_t *)&t);
  return tm->tm_year + 1900;
}

int RTC::weekday(unsigned long t) {
  return (((t / 86400L) + 4) % 7);  //  1 là Chủ nhật
}
//======================================end lay thoi gian thuc=================================================