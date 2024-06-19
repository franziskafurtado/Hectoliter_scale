const int e18_sensor = 25;

void setup() {
  Serial.begin(9600);
  pinMode (e18_sensor, OUTPUT);

}


void loop() {
  digitalWrite(e18_sensor, HIGH);
  delay(5000);
  digitalWrite(e18_sensor, LOW);
  delay(5000);
}
