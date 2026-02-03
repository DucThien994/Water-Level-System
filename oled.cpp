#include "oled.h"

OLED_DISPLAY::OLED_DISPLAY() {
  // &wire là dùng cổng I2C để giao tiếp
  // -1 là không dùng nút reset riêng 
  display = new Adafruit_SH1106G(128, 64, &Wire, -1);
  displayFound = false;
}

bool OLED_DISPLAY::begin() {
  Serial.println("Khoi tao OLED SH1106G...");

  for (int i = 0; i < 5; i++) {
    if (display->begin(0x3C, true)) {
      displayFound = true;
      Serial.println("OLED SH1106G khoi tao THANH CONG!");
      break;
    }
  }

  if (!displayFound) {
    Serial.println("LOI: Khong tim thay OLED SH1106G tai dia chi 0x3C");
    return false;
  }
  
  display->clearDisplay();    
  display->setTextSize(1);    // 6x8 pixels
  display->setTextColor(SH110X_WHITE, SH110X_BLACK);    // chữ chữ trắng nền đen 
  display->setCursor(0, 20);  // vị trí hiển thị // (x = 0; y = 20)
  display->println("WATER LEVEL SYSTEM");     // in ra chữ tại vị trí (0, 20)
  display->setCursor(20, 40); // xét tiếp vị trí tiếp theo
  display->println("STARTING...");  // in ra dòng chữ tại vị trí (20, 40)
  display->display();         // hiển thị 
  delay(2000);

  return true;
}

void OLED_DISPLAY::clear() {
  if (displayFound) display->clearDisplay();
}

void OLED_DISPLAY::showWaterLevel(float waterLevel, float percent, float distance, 
                                  bool wifiConnected, int rssi, String status) {
  if (!displayFound) return;

  display->clearDisplay();
  display->setTextSize(1);
  display->setTextColor(SH110X_WHITE);

  // Bar level 
  int barX = 14, barY = 0, barW = 100, barH = 20;
  display->drawRect(barX, barY, barW, barH, SH110X_WHITE);
  int temp = (int)(constrain(percent, 0, 100) * (barW - 4) / 100.0);
  display->fillRect(barX + 2, barY + 2, temp, barH - 4, SH110X_WHITE);

  // % hihi được rồi nèeeeeeeeeeeeee fuck 
  char buf[20];

  snprintf(buf, sizeof(buf), "%.1f%%", percent);
  display->setTextSize(2);

  int fillWidth = (int)(constrain(percent, 0, 100) * (barW - 4) / 100.0);

  // VẼ THANH TIẾN TRÌNH
  display->drawRect(barX, barY, barW, barH, SH110X_WHITE);
  display->fillRect(barX + 2, barY + 2, fillWidth, barH - 4, SH110X_WHITE);

  int textStartX = 28;
  int charWidth = 12;
  
  for (int i = 0; i < strlen(buf); i++) {
    int charX = textStartX + (i * charWidth);
    int charLeft = charX;
    int charRight = charX + charWidth;

    // Tính phần trăm ký tự nằm trên phần được tô
    int overlapStart = max(charLeft, barX + 2);
    int overlapEnd = min(charRight, barX + 2 + fillWidth);
    int overlapWidth = max(0, overlapEnd - overlapStart);
    float overlapPercent = (float)overlapWidth / charWidth;

    // Quyết định màu dựa trên phần trăm overlap
    if (overlapPercent > 0.5) {
      // >50% ký tự nằm trên phần tô -> chữ ĐEN trên nền TRẮNG
      display->setTextColor(SH110X_BLACK, SH110X_WHITE);
    } else {
      // <=50% ký tự nằm trên phần tô -> chữ TRẮNG trên nền ĐEN
      display->setTextColor(SH110X_WHITE, SH110X_BLACK);
    }

    display->setCursor(charX, 3);
    display->write(buf[i]);
  }

  // ĐẶT LẠI MÀU CHO CÁC TEXT SAU
  display->setTextColor(SH110X_WHITE);

  // Thông tin
  display->setTextSize(1);
  snprintf(buf, sizeof(buf), "Level: %3.1f cm", waterLevel);
  display->setCursor(0, 21); 
  display->println(buf);

  snprintf(buf, sizeof(buf), "Dist:  %3.1f cm", distance);
  display->setCursor(0, 31); 
  display->println(buf);

  // Status
  display->setCursor(0, 41);  
  display->print("Status: ");
  display->println(status);
  display->setTextColor(SH110X_WHITE);

  // WiFi
  display->setCursor(85, 41);
  if (wifiConnected) {
    display->print("(");
    display->print(rssi);
    display->print(")");
  } else {
    display->print("X");
  }
}
void OLED_DISPLAY::showTime(int ngay, int thang, int gio, int phut){
  // hiển thị thời gian theo thời gian thực 
  String s_time = "";

  String s_Gio, s_Phut, s_Ngay, s_Thang;

    
  if(ngay < 10){
    s_Ngay = "0"+ String(ngay);
  }
  else{
    s_Ngay = String(ngay);
  }

  if(thang < 10){
    s_Thang = "0"+ String(thang);
  }
  else{
    s_Thang = String(thang);
  }

  if(gio < 10){
    s_Gio = "0"+ String(gio);
  }
  else{
    s_Gio = String(gio);
  }

  if(phut < 10){
    s_Phut = "0"+ String(phut);
  }
  else{
    s_Phut = String(phut);
  }

  s_time = s_Gio + ":" + s_Phut + " " + s_Ngay + "/" + s_Thang;
  display->setCursor(30, 54);
  
  display->println(s_time);

  display->display();
}
