#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>
#include <MPU6050.h>
#include <math.h>

// ================= MPU6050 =================
MPU6050 mpu;

// ================= MOTOR (L298N) =================
#define ENA 25
#define IN1 26
#define IN2 27
#define IN3 14
#define IN4 12
#define ENB 33

// ================= ULTRASONIC ====================
#define TRIG 18
#define ECHO 19

// ================= BUZZER ========================
#define BUZZER 23

// ================= PARAMETERS ====================
#define OBSTACLE_DISTANCE 20     // cm
#define TILT_LIMIT        40     // degree
#define MOTOR_SPEED       200
#define SIGNAL_TIMEOUT    500    // ms

// ================= ESP-NOW DATA ==================
typedef struct {
  int x;
  int y;
} GestureData;

GestureData receivedData = {0, 0};
unsigned long lastPacketTime = 0;

// ================= MOTOR FUNCTIONS =================
void stopMotor() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

void forward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void backward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void left() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void right() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

// ================= ULTRASONIC =====================
long getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, 25000); // 25 ms timeout

  if (duration == 0) return -1;

  long dist = duration * 0.034 / 2;

  if (dist < 2 || dist > 400) return -1;

  return dist;
}

// ================= ESP-NOW RECEIVE =================
void OnDataRecv(const esp_now_recv_info_t *info,
                const uint8_t *data, int len) {
  if (len == sizeof(GestureData)) {
    memcpy(&receivedData, data, sizeof(receivedData));
    lastPacketTime = millis();
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("================================");
  Serial.println(" SMART WHEELCHAIR SYSTEM START ");
  Serial.println("================================");

  // -------- Motor Pins --------
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  analogWrite(ENA, MOTOR_SPEED);
  analogWrite(ENB, MOTOR_SPEED);

  Serial.println("[OK] Motors Initialized");

  // -------- Ultrasonic --------
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  Serial.println("[OK] Ultrasonic Initialized");

  // -------- Buzzer --------
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
  Serial.println("[OK] Buzzer Initialized");

  // -------- MPU6050 --------
  Wire.begin(21, 22);
  mpu.initialize();

  if (mpu.testConnection())
    Serial.println("[OK] MPU6050 Connected");
  else
    Serial.println("[ERROR] MPU6050 NOT Connected");

  // -------- ESP-NOW --------
  WiFi.mode(WIFI_STA);
  Serial.print("Wheelchair MAC: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("[ERROR] ESP-NOW Init Failed");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("[OK] ESP-NOW Ready");

  Serial.println("--------------------------------");
}

void loop() {

  // ===== SIGNAL LOST SAFETY =====
  if (millis() - lastPacketTime > SIGNAL_TIMEOUT) {
    stopMotor();
    Serial.println("⚠ SIGNAL LOST → MOTOR STOP");
    delay(200);
    return;
  }

  // ===== OBSTACLE CHECK =====
  long distance = getDistance();

  if (distance == -1) {
    Serial.println("Ultrasonic: NO ECHO");
  }
  else if (distance < OBSTACLE_DISTANCE) {
    stopMotor();
    tone(BUZZER, 1200, 150);
    Serial.print("🚧 OBSTACLE: ");
    Serial.print(distance);
    Serial.println(" cm");
    delay(200);
    return;
  }

  // ===== TILT CHECK =====
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  float roll  = atan2(ay, az) * 180 / PI;
  float pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180 / PI;
  float tilt  = max(abs(roll), abs(pitch));

  if (tilt > TILT_LIMIT) {
    stopMotor();
    tone(BUZZER, 1000, 150);
    Serial.print("⚠ TILT DETECTED: ");
    Serial.println(tilt);
    delay(200);
    return;
  }

  // ===== MOVEMENT (GESTURE) =====
  if (receivedData.y > 8000)       forward();
  else if (receivedData.y < -8000) backward();
  else if (receivedData.x > 8000)  right();
  else if (receivedData.x < -8000) left();
  else                             stopMotor();

  // ===== DEBUG OUTPUT =====
  static unsigned long t = 0;
  if (millis() - t > 1000) {
    Serial.print("X: "); Serial.print(receivedData.x);
    Serial.print(" Y: "); Serial.print(receivedData.y);
    Serial.print(" Dist: "); Serial.print(distance);
    Serial.print(" Tilt: "); Serial.println(tilt);
    t = millis();
  }
}
