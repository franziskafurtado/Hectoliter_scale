const int e18_sensor = 23;
#define LED 5

void setup() {
  Serial.begin(9600);
  pinMode (e18_sensor, INPUT);
  pinMode(LED,OUTPUT);

}


void loop() {
  if(digitalRead(e18_sensor) == LOW){
    Serial.println(digitalRead(e18_sensor));
    digitalWrite(LED,HIGH);
  }
  else {
    Serial.println(digitalRead(e18_sensor));
    digitalWrite(LED,LOW);
  }
  
  delay(500);
}
  