#include "pump_control.h"

// bool watering_condition()
// {

// }

// void pump_control(void *pvParameter)
// {
//  //TODO
// }

// Ví dụ: pin bơm
// const int PUMP_PIN = 5; // đổi theo mạch của bạn

void pump_init()
{
    pinMode(PUMP_PIN, OUTPUT);
    digitalWrite(PUMP_PIN, LOW); // bơm tắt
}

// Chế độ: "manual" / "automatic" tuỳ bạn xử lý
void pump_start(uint32_t durationMs, const char *mode)
{
    // 1. Bật bơm   
    digitalWrite(PUMP_PIN, HIGH);

    // 2. Bạn có thể tạo 1 timer / task riêng để tự tắt sau durationMs
    //    Ở đây chỉ in log cho đơn giản
    Serial.print("[PUMP] START for ");
    Serial.print(durationMs);
    Serial.print(" ms, mode = ");
    Serial.println(mode);
}

void pump_stop(uint32_t runTimeMs)
{
    digitalWrite(PUMP_PIN, LOW);
    Serial.print("[PUMP] STOP, runTimeMs = ");
    Serial.println(runTimeMs);
}

void pump_set_mode(const char *mode)
{
    // Bạn lưu trạng thái mode toàn cục nếu cần
    Serial.print("[PUMP] SET MODE = ");
    Serial.println(mode);
}
