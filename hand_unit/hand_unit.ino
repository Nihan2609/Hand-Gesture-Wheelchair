#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>
#include <MPU6050.h>

// ================= MPU =================
MPU6050 mpu;

// ================= DATA STRUCT ===================
typedef struct {
  int x;
  int y;
} GestureData;

GestureData dataToSend;

// ===== Wheelchair MAC =====
uint8_t wheelchairMAC[6] = {0xF4,0x2D,0xC9,0x6C,0x3F,0x34};

// ===== SEND CALLBACK =====
void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "OK" : "FAIL");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("===== HAND UNIT START =====");

  Wire.begin(21, 22);
  mpu.initialize();

  WiFi.mode(WIFI_STA);
  Serial.print("Hand Unit MAC: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, wheelchairMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  Serial.println("Hand Unit Ready");
  Serial.println("---------------------------");
}

void loop() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  dataToSend.x = ax;
  dataToSend.y = ay;

  esp_now_send(wheelchairMAC,
               (uint8_t *)&dataToSend,
               sizeof(dataToSend));

  Serial.print("Sent X: ");
  Serial.print(ax);
  Serial.print(" Y: ");
  Serial.println(ay);

  delay(100);
}
