const int DIR = 32;
const int STEP = 33;
const int en = 5;
const int steps_per_rev = 1600;
const int speed = 500;
int etapa = 1;
int direcao =  1;

#define MOTOR_PASSO_LIMIT_DIREITA_FIM_CURSO_PIN 16
#define MOTOR_PASSO_LIMIT_ESQUERDA_FIM_CURSO_PIN 17
#define LED_ESQ 4
#define LED_DIR 2
#define LED_ERRO 15
//dir ==1 gira para direita, dir =0 gira para 
void setup()
{
  Serial.begin(115200);
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(en, OUTPUT);
  pinMode (MOTOR_PASSO_LIMIT_DIREITA_FIM_CURSO_PIN, INPUT);
  pinMode (MOTOR_PASSO_LIMIT_ESQUERDA_FIM_CURSO_PIN, INPUT);
  pinMode(LED_ESQ, OUTPUT);
  pinMode(LED_DIR, OUTPUT);
  pinMode(LED_ERRO, OUTPUT);
  
  digitalWrite(en, LOW);
  // Gira para a direita primeiro
  digitalWrite(DIR, direcao);
}
void loop()
{


  int cont_timeout_nivelamento = 0;

  if (etapa == 1){
    digitalWrite(DIR, direcao);
    while ((digitalRead(MOTOR_PASSO_LIMIT_DIREITA_FIM_CURSO_PIN) != HIGH) && (digitalRead(MOTOR_PASSO_LIMIT_ESQUERDA_FIM_CURSO_PIN) != HIGH)) {
      digitalWrite(STEP, HIGH);
      delayMicroseconds(speed);
      digitalWrite(STEP, LOW);
      delayMicroseconds(speed);

      cont_timeout_nivelamento += 1;
      if (cont_timeout_nivelamento >= 6500) {
        break;
      }
    }
  }
  if (digitalRead(MOTOR_PASSO_LIMIT_DIREITA_FIM_CURSO_PIN) == HIGH){
      direcao = 0;
      etapa = 0;
  }
  if (digitalRead(MOTOR_PASSO_LIMIT_ESQUERDA_FIM_CURSO_PIN) == HIGH){
      direcao = 1;
      etapa = 0;
  }
  if (etapa == 0){
      digitalWrite(DIR, direcao);
      while ((digitalRead(MOTOR_PASSO_LIMIT_DIREITA_FIM_CURSO_PIN) == HIGH) || (digitalRead(MOTOR_PASSO_LIMIT_ESQUERDA_FIM_CURSO_PIN) == HIGH)) {
      digitalWrite(STEP, HIGH);
      delayMicroseconds(speed);
      digitalWrite(STEP, LOW);
      delayMicroseconds(speed);
  
      cont_timeout_nivelamento += 1;
      if (cont_timeout_nivelamento >= 7500) {
        break;
      }
    }
    etapa = 1;
  }
  

  // Serial.println("--------------------- VAI COMECAR OS LACOS ---------------------");

  // int cont_timeout_nivelamento = 0;

  // digitalWrite(en, LOW);

  // digitalWrite(LED_DIR, LOW);
  // digitalWrite(LED_ESQ, LOW);
  // digitalWrite(LED_ERRO, LOW);

  // // Gira para a direita primeiro
  // digitalWrite(DIR, HIGH);
  // do {
  //   digitalWrite(STEP, HIGH);
  //   delayMicroseconds(100);
  //   digitalWrite(STEP, LOW);
  //   delayMicroseconds(100);

  //   // Serial.println("Ta girando DIREITA");
  //   // Serial.println("ESQUERDA");
  //   // Serial.println(digitalRead(MOTOR_PASSO_LIMIT_ESQUERDA_FIM_CURSO_PIN));
  //   // delay(100);
  //   // Serial.println("DIREITA");
  //   // Serial.println(digitalRead(MOTOR_PASSO_LIMIT_DIREITA_FIM_CURSO_PIN));
  //   // delay(1000);

  //   cont_timeout_nivelamento += 1;
  //   digitalWrite(LED_DIR, HIGH);
  //   if (cont_timeout_nivelamento == 10000) {
  //     digitalWrite(LED_ERRO, HIGH);
  //     break;
  //   }
  // } while ((digitalRead(MOTOR_PASSO_LIMIT_DIREITA_FIM_CURSO_PIN) == LOW) && (digitalRead(MOTOR_PASSO_LIMIT_ESQUERDA_FIM_CURSO_PIN) == LOW));

  // if (cont_timeout_nivelamento > 7500) {
  //   // mensagemErro
  //   Serial.println("--------------------- ERRO DIREITA ---------------------");
  //   Serial.printf("Deu o timeout n ativo o switch DIREITA");
  // }
  // cont_timeout_nivelamento = 0;
  // Serial.println("--------------------- TA FORA DO LOOP DIREITA ---------------------");
  // delay(1000); 
  
  // // Depois gira para a esquerda, deixa rodar um pouco
  // digitalWrite(DIR, LOW);
  // for(int i = 0; i<1000; i++)
  // {
  //   digitalWrite(STEP, HIGH);
  //   delayMicroseconds(100);
  //   digitalWrite(STEP, LOW);
  //   delayMicroseconds(100);
  // }
  // delay(2000);

  // digitalWrite(LED_DIR, LOW);
  // digitalWrite(LED_ESQ, LOW);
  // digitalWrite(LED_ERRO, LOW);
  // // Ai pode girar para o outro sentido
  // do {
  //   digitalWrite(STEP, HIGH);
  //   delayMicroseconds(100);
  //   digitalWrite(STEP, LOW);
  //   delayMicroseconds(100);

  //   // Serial.println("Ta girando ESQUERDA");
  //   // Serial.println("ESQUERDA");
  //   // Serial.println(digitalRead(MOTOR_PASSO_LIMIT_ESQUERDA_FIM_CURSO_PIN));
  //   // delay(100);
  //   // Serial.println("DIREITA");
  //   // Serial.println(digitalRead(MOTOR_PASSO_LIMIT_DIREITA_FIM_CURSO_PIN));
  //   // delay(1000);

  //   digitalWrite(LED_ESQ, HIGH);

  //   cont_timeout_nivelamento += 1;
  //   if (cont_timeout_nivelamento == 10000) {
  //     digitalWrite(LED_ERRO, HIGH);
  //     break;
  //   }
  // }
  // while ((digitalRead(MOTOR_PASSO_LIMIT_DIREITA_FIM_CURSO_PIN) == LOW) && (digitalRead(MOTOR_PASSO_LIMIT_ESQUERDA_FIM_CURSO_PIN) == LOW));

  // if (cont_timeout_nivelamento > 7500) {
  //   // mensagemErro
  //   Serial.println("--------------------- ERRO ESQUERDA ---------------------");
  //   Serial.println("Deu o timeout n ativo o switch ESQUERDA");
  // }

  // Serial.println("--------------------- TA FORA DO LOOP ESQUERDA ---------------------");

  // delay(1000);
  // // Por fim, deixa fora do fim de curso novamente
  // digitalWrite(DIR, HIGH);
  // for(int i = 0; i<1000; i++)
  // {
  //   digitalWrite(STEP, HIGH);
  //   delayMicroseconds(100);
  //   digitalWrite(STEP, LOW);
  //   delayMicroseconds(100);
  // }
  // delay(2000);

  // Serial.println("--------------------- TERMINO OS LACOS ---------------------");
}