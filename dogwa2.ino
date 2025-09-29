#define PIN_LED 7
unsigned int toggle, period, var;
double cur_stat, duty;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  toggle = 1;
  period = 100;
   = 500000/period;
  duty=0;
  
}

void loop() {
  toggle = set_period(toggle);
  var = set_duty(duty);
  digitalWrite(PIN_LED, toggle);
  delayMicroseconds(double(period/100)*(var));
  toggle = set_period(toggle);
  digitalWrite(PIN_LED, toggle);
  delayMicroseconds(double(period/100)*(100-var));
}

int set_period(int toggle) {
  return !toggle;
}



int set_duty(double duty_val){
  duty = duty_val+(100/cur_stat);
  if(duty >= 100 || duty <= 0) cur_stat= -cur_stat; 
  return int(duty);
} 
