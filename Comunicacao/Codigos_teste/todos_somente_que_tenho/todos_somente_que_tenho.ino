#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Math.h>
#include <ESP32Servo.h>
#include "HX711.h"

#define LED_DEBUG 2

// --------------------------------------------- ACELEROMETRO
int cont = 0;
float med_x = 0;
float med_y = 0;
float med_z = 0;
float med_z_inicial = 0;
Adafruit_MPU6050 mpu;

// --------------------------------------------- FIM DE CURSO
#define RETURNING_CONTAINER_FIM_CURSO_PIN 34
#define COLLECTING_CONTAINER_PORTINHA_FIM_CURSO_PIN 35
#define MOTOR_PASSO_LIMIT_DIREITA_FIM_CURSO_PIN 17
#define MOTOR_PASSO_LIMIT_ESQUERDA_FIM_CURSO_PIN 16

// --------------------------------------------- SERVO MOTOR POTENCIOMETRO
#define SERVO_COLLECTING_CONTAINER_PIN 27
#define POTENCIOMETER_COLLECTINH_CONTAINER_PIN 26
#define SERVO_MEASURING_CONTAINER_PIN 12
#define POTENCIOMETER_MEASURING_CONTAINER_PIN 14

Servo servoCollectingContainer;
Servo servoMeasuringContainer;

// --------------------------------------------- MOTOR VIBRACAO
#define MOTOR_VIBRACAP_PIN 25

// --------------------------------------------- LEDS
#define LED_COLLECTING_PHASE 4
#define LED_MEASURING_PHASE 2
#define LED_RETURNING_PHASE 15
#define LED_WIFI 13

// --------------------------------------------- CELULA CARGA
#define LOADCELL_DOUT_PIN 19
#define LOADCELL_SCK_PIN 18
HX711 scale;

// --------------------------------------------- SENSOR IR
#define SENSOR_IR 23

// --------------------------------------------- MOTOR PASSO
#define DIR 32
#define STEP 33
#define en 5
const int steps_per_rev = 1500;



// --------------------------------------------- SETUP ACELEROMETRO
void setup_accelerometer(void) {
  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
}

// --------------------------------------------- FIM DE CURSO
void setup_fim_curso(void) {
  pinMode (RETURNING_CONTAINER_FIM_CURSO_PIN, INPUT);
  pinMode (MOTOR_PASSO_LIMIT_ESQUERDA_FIM_CURSO_PIN, INPUT);
  pinMode (MOTOR_PASSO_LIMIT_DIREITA_FIM_CURSO_PIN, INPUT);
  pinMode (COLLECTING_CONTAINER_PORTINHA_FIM_CURSO_PIN, INPUT);
  delay(100);
}

// --------------------------------------------- SERVO MOTOR POTENCIOMETRO
void setup_servo_potenciometro(void ) {
  servoCollectingContainer.setPeriodHertz(50); // Set PWM frequency to 50Hz (standard for servos)
  servoCollectingContainer.attach(SERVO_COLLECTING_CONTAINER_PIN, 500, 2400); // (pin, min pulse width, max pulse width in microseconds)
  servoMeasuringContainer.setPeriodHertz(50); // Set PWM frequency to 50Hz (standard for servos)
  servoMeasuringContainer.attach(SERVO_MEASURING_CONTAINER_PIN, 500, 2400); // (pin, min pulse width, max pulse width in microseconds)
  delay(100);
}

// --------------------------------------------- MOTOR VIBRACAO
void setup_motor_vibracao(void) {
  pinMode (MOTOR_VIBRACAP_PIN, OUTPUT);
  delay(100);
}

// --------------------------------------------- LEDS
void setup_leds(void) {
  pinMode(LED_RETURNING_PHASE, OUTPUT); 
  pinMode(LED_MEASURING_PHASE, OUTPUT); 
  pinMode(LED_COLLECTING_PHASE, OUTPUT); 
  pinMode(LED_WIFI, OUTPUT); 
  delay(100);
}

// --------------------------------------------- CELULA CARGA
void setup_celula_carga(void) {
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  delay(100);
}

// --------------------------------------------- SENSOR IR
void setup_sensor_ir(void) {
  pinMode (SENSOR_IR, INPUT);
  delay(100);
}

// --------------------------------------------- MOTOR PASSO
void setup_motor_passo(void) {
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(en, OUTPUT);
  delay(100);
}


// --------------------------------------------- SETUP MAIN --------------------------------------------- 
void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10);

  // setup_accelerometer();
  setup_fim_curso();
  setup_servo_potenciometro();
  setup_motor_vibracao();  
  setup_sensor_ir();
  setup_motor_passo();
  setup_leds();
  // setup_celula_carga();

  pinMode(LED_DEBUG, OUTPUT);
}

// --------------------------------------------- ACELEROMETRO
void acelerometro(void) {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  Serial.println("------------------- ACELETROMETRO -------------------");

  Serial.print("X:");
  Serial.print(a.acceleration.x);
  Serial.println(",");
  
  Serial.print("Y:"); 
  Serial.print(a.acceleration.y);
  Serial.println(",");
  
  Serial.print("Z:"); 
  Serial.print(a.acceleration.z);
  Serial.println(",");

  med_z = a.acceleration.z;

  if (cont < 10) {
    cont += 1;
    med_z_inicial = a.acceleration.z;
  }

  if ((med_z > med_z_inicial + 1) || (med_z < med_z_inicial - 1)) {
    Serial.println("MEXEU!");
    digitalWrite(LED_RETURNING_PHASE, HIGH);
    digitalWrite(LED_MEASURING_PHASE, HIGH);
  }
  else {
    digitalWrite(LED_RETURNING_PHASE, LOW);
    digitalWrite(LED_MEASURING_PHASE, LOW);
  }
}


// --------------------------------------------- FUNCOES PARA O COLLECTING PHASE
// ---- FIM DE CURSO
int porta_entrada_OU_sem_container_coleta(void) {
  int container_coleta = digitalRead(COLLECTING_CONTAINER_PORTINHA_FIM_CURSO_PIN);
  int porta = digitalRead(COLLECTING_CONTAINER_PORTINHA_FIM_CURSO_PIN);

  if ((container_coleta == LOW) && (porta == LOW)){
    return 0;
  }
  return 1;
}

// ---- SENSOR IR
int verificando_nivel_semente(void) {
  int cont_tempo_sensor_ir = 0;

  if (digitalRead(SENSOR_IR) == LOW) {
    while ((cont_tempo_sensor_ir < 5) && (digitalRead(SENSOR_IR) == LOW)) {
      cont_tempo_sensor_ir += 1;
      delay(500);
    }
    if (cont_tempo_sensor_ir >= 5) {
      return 0;
    }
  }
  return 1;
}

// ---- SERVO MOTOR
void ativar_alcapao_collecting_container(void) {
  for(int posDegrees = 0; posDegrees <= 180; posDegrees++) {
    servoMeasuringContainer.write(posDegrees);
    delay(20);
  }

  for(int posDegrees = 180; posDegrees >= 0; posDegrees--) {
    servoMeasuringContainer.write(posDegrees);
    delay(20);
  }
}

// -----------------------------------------------------------------------------



// --------------------------------------------- COLLECTING PHASE
void collectingPhase(void) {
  // Acender led da fase de coleta                                     <- 1 LED
  digitalWrite(LED_COLLECTING_PHASE, HIGH);
  digitalWrite(LED_MEASURING_PHASE, LOW);
  digitalWrite(LED_RETURNING_PHASE, LOW);

  digitalWrite(LED_DEBUG, LOW);

  // Verificar o sensor IR se está acusando o nivel certo                   <- Sensor IR
  while (verificando_nivel_semente()) {
    digitalWrite(LED_DEBUG, HIGH);
    delay(500);
    digitalWrite(LED_DEBUG, LOW);
  }

  // Ativar o servo motor do alcapao 1                                      <- 1 Servo Motor
  ativar_alcapao_collecting_container();

  // Verificar pelo potenciometro se o servo foi ativao corretamente        <- 1 Potenciometro
  // int servoPosition = map(analogRead(POTENCIOMETER_COLLECTINH_CONTAINER_PIN), 0, 4096, 0, 180);
  // if ((servoPosition > 190) || (servoPosition < -10))
  // Mandar mensagem de erro
}


// --------------------------------------------- FUNCOES PARA O MEASURING PHASE
// ---- MOTOR VIBRACAO
void vibrar_measuring_container(void) {
  digitalWrite(MOTOR_VIBRACAP_PIN, HIGH);

  delay(5000);

  digitalWrite(MOTOR_VIBRACAP_PIN, LOW);
}

// ---- MOTOR PASSO
void nivelamento_amostra(void) {
  int cont_timeout_nivelamento = 0;

  digitalWrite(en, LOW);

  // Gira para a direita primeiro
  digitalWrite(DIR, HIGH);
  while (digitalRead(COLLECTING_CONTAINER_PORTINHA_FIM_CURSO_PIN) == HIGH) {
    digitalWrite(STEP, HIGH);
    delayMicroseconds(100);
    digitalWrite(STEP, LOW);
    delayMicroseconds(100);

    cont_timeout_nivelamento += 1;
    if (cont_timeout_nivelamento == steps_per_rev) {
      break;
    }
  }
  // if (cont_timeout_nivelamento < 7500) {
  //   mensagemErro
  // }
  cont_timeout_nivelamento = 0;
  delay(500); 
  
  // Depois gira para a esquerda
  digitalWrite(DIR, LOW);
  while (digitalRead(COLLECTING_CONTAINER_PORTINHA_FIM_CURSO_PIN) == HIGH) {
    digitalWrite(STEP, HIGH);
    delayMicroseconds(100);
    digitalWrite(STEP, LOW);
    delayMicroseconds(100);

    cont_timeout_nivelamento += 1;
    if (cont_timeout_nivelamento == steps_per_rev) {
      break;
    }
  }
  // if (cont_timeout_nivelamento < 7500) {
  //   mensagemErro
  // }
}

// ---- CELULA CARGA
void medicao_amostra(void) {
  if (scale.is_ready()) {
    // ------- Soh para mostrar enquanto nao enviamos dados
    digitalWrite(LED_DEBUG, LOW);
    scale.set_scale();    
    Serial.println("Tare... remove any weights from the scale.");
    delay(2000);
    scale.tare();
    Serial.println("Tare done...");
    Serial.print("Place a known weight on the scale...");
    delay(2000);
    long reading = scale.get_units(10);
    Serial.print("Result: ");
    Serial.println(reading);
  } 
  else {
    Serial.println("HX711 not found.");
    digitalWrite(LED_DEBUG, HIGH);
  }
}

// ---- SERVO MOTOR
void ativar_alcapao_measuring_container(void) {
  for(int posDegrees = 0; posDegrees <= 180; posDegrees++) {
    servoMeasuringContainer.write(posDegrees);
    delay(20);
  }

  for(int posDegrees = 180; posDegrees >= 0; posDegrees--) {
    servoMeasuringContainer.write(posDegrees);
    delay(20);
  }
}

// -----------------------------------------------------------------------------

// --------------------------------------------- MEASURING PHASE
void measuringPhase(void) {
  // Acender o led da fase de medicao                                       <- 1 LED
  digitalWrite(LED_COLLECTING_PHASE, LOW);
  digitalWrite(LED_MEASURING_PHASE, HIGH);
  digitalWrite(LED_RETURNING_PHASE, LOW);

  digitalWrite(LED_DEBUG, HIGH);

  // Fazer o motor de vibracao acionar                                      <- Motor de Vibracao
  vibrar_measuring_container();

  // Fazer o motor de passo acionar para frente e para tras                 <- Motor de passo
  //       deixa em um loop ateh acionar os fim de cursos                   <- 2 Fim de Curso
  nivelamento_amostra();

  // Faz a medida da amostra                                                <- Celula de carga
  // medicao_amostra();

  // Ativa o servo motor para liberar o alcapao                             <- 1 Servo Motor
  ativar_alcapao_measuring_container();

  // Verificar pelo potenciometro se o servo foi ativao corretamente        <- 1 Potenciometro
  // int servoPosition = map(analogRead(POTENCIOMETER_MEASURING_CONTAINER_PIN), 0, 4096, 0, 180);
  // if ((servoPosition > 190) || (servoPosition < -10))
  // Mandar mensagem de erro
  
}

// --------------------------------------------- RETURNING PHASE
void returningPhase(void) {
  // Acender o led da fase de retorno                                       <- 1 LED
  digitalWrite(LED_COLLECTING_PHASE, LOW);
  digitalWrite(LED_MEASURING_PHASE, LOW);
  digitalWrite(LED_RETURNING_PHASE, HIGH);

  // Vai verificando se o container de retorno saiu ou nao pelo fim de curso
  delay(5000);
  while(porta_entrada_OU_sem_container_coleta()){
    delay(100);
  }
}

// --------------------------------------------- LOOP MAIN --------------------------------------------- 
void loop() {
  // ---------------- Isso eu ainda nao vou implementar, estah pronto mas ainda n vou fazer funcionar junto aqui
  // Verificar a conexao com o wifi para comecar a maquina    <- Modilo WIFI
  // -------------------------------------------------
  // -------------------------------------------------




  digitalWrite(LED_DEBUG, HIGH);
  // Verificar se a porta de cima e o container de retorno estao no lugar   <- 2 FIM DE CURSO
  while(porta_entrada_OU_sem_container_coleta()){
    digitalWrite(LED_DEBUG, LOW);
    delay(500);
    digitalWrite(LED_DEBUG, HIGH);
  }


  

  collectingPhase();
  // Acender led da fase de coleta                                     <- 1 LED
  // Verificar o sensor IR se está acusando o nivel certo                   <- Sensor IR
  // Ativar o servo motor do alcapao 1                                      <- 1 Servo Motor
  // Verificar pelo potenciometro se o servo foi ativao corretamente        <- 1 Potenciometro



  
  measuringPhase();
  // Acender o led da fase de medicao                                       <- 1 LED
  // Fazer o motor de vibracao acionar                                      <- Motor de Vibracao
  // Fazer o motor de passo acionar para frente e para tras                 <- Motor de passo
  //       deixa em um loop ateh acionar os fim de cursos                   <- 2 Fim de Curso
  // Faz a medida da amostra                                                <- Celula de carga
  // Ativa o servo motor para liberar o alcapao                             <- 1 Servo Motor
  // Verificar pelo potenciometro se o servo foi ativao corretamente        <- 1 Potenciometro



  returningPhase();
  // Acender o led da fase de retorno                                       <- 1 LED
  // Vai verificando se o container de retorno saiu ou nao pelo fim de curso
  
  delay(500);
}