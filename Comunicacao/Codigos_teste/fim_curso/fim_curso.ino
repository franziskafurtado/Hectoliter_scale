const int e18_sensor = 34;
const int e18_sensor2 = 35;
const int e18_sensor3 = 17;
const int e18_sensor4 = 16;

#define LED1 15
#define LED2 5
#define LED3 4
#define LED_DEBU 18

void setup() {
  Serial.begin(9600);
  pinMode (e18_sensor, INPUT);
  pinMode (e18_sensor2, INPUT);
  pinMode (e18_sensor3, INPUT);
  pinMode (e18_sensor4, INPUT);

  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(LED_DEBU,OUTPUT);
}

void loop() {
  if(digitalRead(e18_sensor) == LOW) {
    digitalWrite(LED1,HIGH);
  }
  else {
    digitalWrite(LED1,LOW);
  }

  if(digitalRead(e18_sensor2) == LOW) {
    digitalWrite(LED2,HIGH);
  }
  else {
    digitalWrite(LED2,LOW);
  }

  if(digitalRead(e18_sensor3) == LOW) {
    digitalWrite(LED3,HIGH);
  }
  else {
    digitalWrite(LED3,LOW);
  }

  if(digitalRead(e18_sensor4) == LOW) {
    digitalWrite(LED_DEBU,HIGH);
  }
  else {
    digitalWrite(LED_DEBU,LOW);
  }

  delay(300);
  
}
