const int DIR = 32;
const int STEP = 33;
const int en = 5;
const int steps_per_rev = 1600;

void setup()
{
  Serial.begin(115200);
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(en, OUTPUT);
}
void loop()
{
  digitalWrite(en, LOW);
  digitalWrite(DIR, HIGH);
  Serial.println("Girando no sentido horário...");
  
  for(int i = 0; i<steps_per_rev; i++)
  {
    digitalWrite(STEP, HIGH);
    delayMicroseconds(100);
    digitalWrite(STEP, LOW);
    delayMicroseconds(100);
  }
  delay(500); 
  
  digitalWrite(DIR, LOW);
  Serial.println("Girando no sentido anti-horário...");

  for(int i = 0; i<steps_per_rev; i++)
  {
    digitalWrite(STEP, HIGH);
    delayMicroseconds(100);
    digitalWrite(STEP, LOW);
    delayMicroseconds(100);
  }
  delay(500);
}