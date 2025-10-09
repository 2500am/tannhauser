// Arduino pin assignment
#define PIN_LED  9
#define PIN_TRIG 12
#define PIN_ECHO 13

// configurable parameters
#define SND_VEL 346.0
#define INTERVAL 50
#define PULSE_DURATION 10
#define _DIST_MIN 100
#define _DIST_MAX 300
#define TIMEOUT ((INTERVAL / 2) * 1000.0)
#define SCALE (0.001 * 0.5 * SND_VEL)

// median filter & EMA configuration
#define N 30
#define EMA_ALPHA 0.5

unsigned long last_sampling_time;
float circular_buffer[N];  
int cb_idx = 0;      
bool buffer_filled = false; 

float dist_ema = 0;            // EMA 값 저장
bool first_measurement = false ; // EMA 초기화 여부

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);

  Serial.begin(57600);

  for(int i=0; i<N; i++) circular_buffer[i] = 0;
}

void loop() {
  if (millis() < last_sampling_time + INTERVAL)
    return;

  float dist_raw = USS_measure(PIN_TRIG, PIN_ECHO);

  
  circular_buffer[cb_idx++] = dist_raw;
  if(cb_idx >= N){
    cb_idx = 0;
    buffer_filled = true;
  }

  // median 계산
  int n = buffer_filled ? N : cb_idx;
  float dist_median = median(circular_buffer, n);

  // EMA 계산
  if(!first_measurement){
    dist_ema = dist_raw;
    first_measurement = true;
  } else {
    dist_ema = EMA_ALPHA * dist_raw + (1 - EMA_ALPHA) * dist_ema;
  }

  
  last_sampling_time += INTERVAL;
  
  // 시리얼 출력
  Serial.print("Min:"); Serial.print(_DIST_MIN);
  Serial.print(",raw:"); Serial.print(dist_raw);
  Serial.print(",median:"); Serial.print(dist_median);
  Serial.print(",ema:"); Serial.print(dist_ema);
  Serial.print(",Max:"); Serial.print(_DIST_MAX);
  Serial.println("");

  // LED 표시
  if(dist_raw < _DIST_MIN || dist_raw > _DIST_MAX)
    digitalWrite(PIN_LED, HIGH); // LED OFF
  else
    digitalWrite(PIN_LED, LOW);  // LED ON

}

// Median 계산 함수
float median(float xarr[], int n){
  float temp[n];
  for(int i=0; i<n; i++) temp[i] = xarr[i];

  // 간단한 버블 소트
  for(int i=0; i<n-1; i++){
    for(int j=i+1; j<n; j++){
      if(temp[i] > temp[j]){
        float t = temp[i];
        temp[i] = temp[j];
        temp[j] = t;
      }
    }
  }

  if(n % 2 == 1) 
    return temp[n/2];
  else          
    return (temp[n/2 - 1] + temp[n/2]) / 2.0;
}

// USS 거리 측정 함수
float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
    digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE;
}
