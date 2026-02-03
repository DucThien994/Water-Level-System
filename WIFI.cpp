#include "WIFI.h"

String WIFI_MANAGER::LaySoMAC(void) {
  return WiFi.macAddress();
}

void WIFI_MANAGER::KetNoiWiFi(int ThoiGianChoKetNoi) {
  Serial.println("\nConnecting WiFi...");
  Serial.print("SSID: ");
  Serial.println(TenWiFi);

  WiFi.begin(TenWiFi, MatKhauWiFi);

  // hàm đợi kết nối wifi
  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < (unsigned long)ThoiGianChoKetNoi) {
    Serial.print(".");
    delay(500);
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("MAC: ");
    Serial.println(LaySoMAC());
  } else {
    Serial.println("\nCannot connect WiFi!");
  }
}

int WIFI_MANAGER::TinhDoManhCuaWiFi(void) {
  if (WiFi.status() == WL_CONNECTED)
    return WiFi.RSSI();
  else
    return -999;
}