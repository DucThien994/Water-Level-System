#ifndef _RTC_h
#define _RTC_h

#include <NTPClient.h>
#include <WiFiUdp.h>

typedef struct {
  int Thu;
  int Ngay;
  int Thang;
  int Nam;
  int Gio;
  int Phut;
  int Giay;
} ThoiGian;

class RTC {
private:
  WiFiUDP ntpUDP;
  NTPClient timeClient;
  
  int day(unsigned long t);
  int month(unsigned long t);
  int year(unsigned long t);
  int weekday(unsigned long t);
  // ================================

public:
  RTC();
  void KhoiTaoRTC(void);
  void LayThoiGianTuServer(void);
  void HienThiThoiGianSerial(void);
  String LayChuoiThoiGian(void);
};

extern ThoiGian dt;
#endif