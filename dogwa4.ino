#include <Servo.h>

#define PIN_SERVO 10

#define _DUTY_MIN 500   // 0°
#define _DUTY_NEU 1500  // 90°
#define _DUTY_MAX 2500  // 180°
#define INTERVAL 20     // ms

Servo myservo;

float duty_curr;
float duty_target;
float duty_change_per_interval;

unsigned long last_sampling_time = 0;
unsigned long start_time;
int phase = 1; // 1단계(0→180), 2단계(180→90)

// 속도 계산 함수 (deg/s → us 변화량)
float calc_duty_delta(float speed_deg_per_sec) {
  return (_DUTY_MAX - _DUTY_MIN) / 180.0 * speed_deg_per_sec * (INTERVAL / 1000.0);
}

void setup() {
  myservo.attach(PIN_SERVO);
  Serial.begin(57600);

  // 초기 설정: 0°에서 시작
  duty_curr = _DUTY_MIN;
  duty_target = _DUTY_MAX; // 180°로 이동
  myservo.writeMicroseconds(duty_curr);

  start_time = millis();

  // 1단계 속도 계산 (180° / 60초 = 3°/s)
  duty_change_per_interval = calc_duty_delta(3.0);
}

void loop() {
  if (millis() < last_sampling_time + INTERVAL) return;

  unsigned long elapsed_sec = (millis() - start_time) / 1000;

  // 🔸 60초 이후 → 2단계 전환
  if (phase == 1 && elapsed_sec >= 60) {
    phase = 2;
    Serial.println("=== Phase 2 start: move to 90° over 5 minutes ===");

    duty_target = _DUTY_NEU; // 90°
    // 180° → 90° (90° 차이), 300초 동안 이동 = 0.3°/s
    duty_change_per_interval = calc_duty_delta(0.3);
  }

  // 등속 이동
  if (duty_target > duty_curr) {
    duty_curr += duty_change_per_interval;
    if (duty_curr > duty_target) duty_curr = duty_target;
  } else {
    duty_curr -= duty_change_per_interval;
    if (duty_curr < duty_target) duty_curr = duty_target;
  }

  myservo.writeMicroseconds(duty_curr);

  // 상태 출력 (디버깅용)
  Serial.print("phase: "); Serial.print(phase);
  Serial.print(", curr: "); Serial.print(duty_curr);
  Serial.print(", target: "); Serial.print(duty_target);
  Serial.print(", time: "); Serial.println(elapsed_sec);

  last_sampling_time += INTERVAL;
}
