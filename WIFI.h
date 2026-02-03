#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
#include <WiFi.h>

class WIFI_MANAGER {
private:
  //  const char* TenWiFi = "KANA";
  //  const char* MatKhauWiFi = "34thongnhat";
  // const char* TenWiFi = "Thuc Coffee";
  // const char* MatKhauWiFi = "18006230";
  const char* TenWiFi = "Monkey 2";
  const char* MatKhauWiFi = "quancomnho31@";
  // const char* TenWiFi = "Nguyễn Đức Thiện";
  // const char* MatKhauWiFi = "ThePyngu";

public:
  void KetNoiWiFi(int ThoiGianChoKetNoi);
  String LaySoMAC(void);
  int TinhDoManhCuaWiFi(void);
};

#endif