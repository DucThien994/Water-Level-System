#ifndef MAIN_H
#define MAIN_H

#define SENSOR_HEIGHT 150.0    // Chiều cao từ đáy bể lên cảm biến (cm)
#define TANK_HEIGHT 130.0      // Chiều cao bể nước (cm)
#define TANK_MAX_DEPTH 130.0   // Mức nước tối đa (cm)

// Ngưỡng cảnh báo
#define HIGH_WATER_LEVEL 115.0
#define LOW_WATER_LEVEL 40.0

// Chân kết nối
#define TRIG_PIN 27
#define ECHO_PIN 26
#define ALERT_LED_PIN 14
#define Emitter 12
void KhoiTaoHeThong();
void main_loop();
void VTaskWifi(void *pvParameters);
void handle_Readings();

#endif