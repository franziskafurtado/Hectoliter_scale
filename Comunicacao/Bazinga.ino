#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Math.h>
#include <ESP32Servo.h>
#include "HX711.h"
// --------------------------------------------- BIBLIOTECAS COMUNICACAO COM BANCO
#include <WiFi.h>
#include <HTTPClient.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <ArduinoJson.h>

// --------------------------------------------- VARIAVEIS COMUNICACAO COM BANCO
int status = 0;
int iniciar = 0;
int enviarFim = 0;
int errorsMVector[20];
int errorsMVectorSize = 0;

int errorsPVector[20];
int errorsPVectorSize = 0;

double endHectoliter = 0.0;
double endWeight = 0.0;
int procStatus = 0;

int errorsProcessGui[8] = {0, 0, 0, 0, 0, 0, 0, 0};

const char* ssid = "Renies e Camis 2.4G";
const char* password = "abba2610";
const char* apiUrl = "https://joaopedrogalera.pythonanywhere.com:80/machineSync";
int WiFiStatus = 0;

// --------------------------------------------- ACELEROMETRO
float med_x = 0;
float med_y = 0;
float med_z = 0;
float med_x_inicial = 0;
float med_y_inicial = 0;
float med_z_inicial = 0;
Adafruit_MPU6050 mpu;

int cont = 0;

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
  pinMode (MOTOR_PASSO_LIMIT_ESQUERDA_FIM_CURSO_PIN, INPUT_PULLDOWN);
  pinMode (MOTOR_PASSO_LIMIT_DIREITA_FIM_CURSO_PIN, INPUT_PULLDOWN);
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

// --------------------------------------------- COMUNICACAO BANCO
void setup_comunicacao_banco(void) {
  WiFi.begin(ssid, password);

  // Cria threads
  xTaskCreate(task_comunicacao, "Comunicacao", 4096, NULL, 5, NULL); 
  xTaskCreate(task_processo, "Processo", 4096, NULL, 5, NULL);
  // xTaskCreate(task_monitor, "Monitoramento", 128, NULL, 5, NULL);
}

// --------------------------------------------- SETUP MAIN --------------------------------------------- 
void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10);

  setup_accelerometer();
  setup_fim_curso();
  setup_servo_potenciometro();
  setup_motor_vibracao();  
  setup_sensor_ir();
  setup_motor_passo();
  setup_leds();
  setup_celula_carga();
  setup_comunicacao_banco();
}

// --------------------------------------------- FUNCOES PARA OS SETS DA COMUNICACAO
void setIdle(){
  status = 0;
}

void setRunning(){
  status = 1;
}

int setError(int* list, int size){
  Serial.print("Erro size");
  Serial.println(size);
  if(!size){
    return 0;
  }
  int i = 0;
  
  errorsMVectorSize = size;
  
  for(i = 0; i < size; i++){
    errorsMVector[i] = list[i];
  }

  status = 2;
}

void fim_processo(double weight, double hectoliter)
{
    iniciar = 0;
    int i = 0, n = 0;
    
    
    endHectoliter = hectoliter;
    endWeight = weight;
  
    for(i = 0; i < 8; i++){
      if(errorsProcessGui[i]){
        errorsPVector[n] = errorsProcessGui[i];
        n++;
      }
    }

    procStatus = n ? 2 : 1;
    errorsPVectorSize = n;

    Serial.print("ProcStatus");
    Serial.println(procStatus);
    Serial.print("ProcErrn");
    Serial.println(errorsPVectorSize);

    enviarFim = 1;
    status = 3;
    
}

// --------------------------------------------- ACELEROMETRO
void acelerometro(void) {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  med_x = a.acceleration.x;
  med_y = a.acceleration.y;
  med_z = a.acceleration.z;

  if (((med_x > med_x_inicial + 1) || (med_x < med_x_inicial - 1)) || 
      ((med_y > med_y_inicial + 1) || (med_y < med_y_inicial - 1)) ||
      ((med_z > med_z_inicial + 1) || (med_z < med_z_inicial - 1))) {
    errorsProcessGui[6] = 70;
  }
}

void resetServos() {
  servoCollectingContainer.write(175);
  delay(2000);
  servoMeasuringContainer.write(175);
  delay(1000);
}

// --------------------------------------------- FUNCOES PARA O COLLECTING PHASE
// ---- FIM DE CURSO
int porta_entrada_OU_sem_container_coleta(void) {
  int container_coleta = digitalRead(RETURNING_CONTAINER_FIM_CURSO_PIN);
  int porta = digitalRead(COLLECTING_CONTAINER_PORTINHA_FIM_CURSO_PIN);

  if ((container_coleta == HIGH) && (porta == HIGH)){
    return 0;
  }
  return 1;
}

// ---- SENSOR IR
void verificando_nivel_semente(void) {
  delay(1000);

  if (digitalRead(SENSOR_IR) == HIGH) {
    errorsProcessGui[4] = 0;
  }
}

// ---- SERVO MOTOR
void ativar_alcapao_collecting_container(void) {
  delay(5000);
  acelerometro();
  servoCollectingContainer.write(175);
  delay(5000);
  // Verificar pelo potenciometro se o servo foi ativao corretamente        <- 1 Potenciometro
  // int servoPosition = map(analogRead(POTENCIOMETER_COLLECTINH_CONTAINER_PIN), 0, 4096, 0, 180);
  // if ((servoPosition > 200) || (servoPosition < -20)) {
  //   errorsProcessGui[0] = 10;
  // }
  acelerometro();
  servoCollectingContainer.write(0);
  delay(5000);
  // Verificar pelo potenciometro se o servo foi ativao corretamente        <- 1 Potenciometro
  // servoPosition = map(analogRead(POTENCIOMETER_COLLECTINH_CONTAINER_PIN), 0, 4096, 0, 180);
  // if ((servoPosition > 200) || (servoPosition < -20)) {
  //   errorsProcessGui[0] = 10;
  // }
  acelerometro();
  servoCollectingContainer.write(175);
  delay(2000);
  // Verificar pelo potenciometro se o servo foi ativao corretamente        <- 1 Potenciometro
  // servoPosition = map(analogRead(POTENCIOMETER_COLLECTINH_CONTAINER_PIN), 0, 4096, 0, 180);
  // if ((servoPosition > 200) || (servoPosition < -20)) {
  //   errorsProcessGui[0] = 10;
  // }
}

// -----------------------------------------------------------------------------



// --------------------------------------------- COLLECTING PHASE
void collectingPhase(void) {
  // Acender led da fase de coleta                                     <- 1 LED
  digitalWrite(LED_COLLECTING_PHASE, HIGH);
  digitalWrite(LED_MEASURING_PHASE, LOW);
  digitalWrite(LED_RETURNING_PHASE, LOW);

  errorsProcessGui[4] = 50;
  delay(100);

  // Verificar o sensor IR se está acusando o nivel certo                   <- Sensor IR
  verificando_nivel_semente();
  delay(500);
  acelerometro();

  // Ativar o servo motor do alcapao 1                                      <- 1 Servo Motor
  ativar_alcapao_collecting_container();
  acelerometro();
}


// --------------------------------------------- FUNCOES PARA O MEASURING PHASE
// ---- MOTOR VIBRACAO
void vibrar_measuring_container(void) {
  acelerometro();
  digitalWrite(MOTOR_VIBRACAP_PIN, HIGH);

  delay(5000);

  acelerometro();
  digitalWrite(MOTOR_VIBRACAP_PIN, LOW);
}

// ---- MOTOR PASSO
void nivelamento_amostra(int etapa_parametro, int direcao_parametro) {
  int quantidade = 0;
  int cont_timeout_nivelamento = 0;
  int etapa = etapa_parametro;
  int direcao = direcao_parametro;

  digitalWrite(en, LOW);

  while (quantidade < 2) {
    cont_timeout_nivelamento = 0;

    if (etapa == 1){
      digitalWrite(DIR, direcao);

      while ((digitalRead(MOTOR_PASSO_LIMIT_DIREITA_FIM_CURSO_PIN) != HIGH) && (digitalRead(MOTOR_PASSO_LIMIT_ESQUERDA_FIM_CURSO_PIN) != HIGH)) {
        acelerometro();
        digitalWrite(STEP, HIGH);
        delayMicroseconds(500);
        digitalWrite(STEP, LOW);
        delayMicroseconds(500);

        cont_timeout_nivelamento += 1;
        if (cont_timeout_nivelamento >= 15000) {
          errorsProcessGui[2] = 30;
          quantidade += 1;
          cont_timeout_nivelamento = 0;
          break;
        }
      }
    }
    if (digitalRead(MOTOR_PASSO_LIMIT_DIREITA_FIM_CURSO_PIN) == HIGH){
        direcao = 0;
        etapa = 0;
        quantidade += 1;
        cont_timeout_nivelamento = 0;
        acelerometro();
    }
    else if (digitalRead(MOTOR_PASSO_LIMIT_ESQUERDA_FIM_CURSO_PIN) == HIGH){
        direcao = 1;
        etapa = 0;
        quantidade += 1;
        cont_timeout_nivelamento = 0;
        acelerometro();
    }
    if (etapa == 0){
        digitalWrite(DIR, direcao);

        while ((digitalRead(MOTOR_PASSO_LIMIT_DIREITA_FIM_CURSO_PIN) == HIGH) || (digitalRead(MOTOR_PASSO_LIMIT_ESQUERDA_FIM_CURSO_PIN) == HIGH)) {
          acelerometro();
          digitalWrite(STEP, HIGH);
          delayMicroseconds(500);
          digitalWrite(STEP, LOW);
          delayMicroseconds(500);
      
          cont_timeout_nivelamento += 1;
          if (cont_timeout_nivelamento >= 15000) {
            quantidade += 1;
            cont_timeout_nivelamento = 0;
            break;
          }
        }
      etapa = 1;
    }

    // delay(1000);

  }
}

// ---- CELULA CARGA
int medicao_amostra(void) {
  if (scale.is_ready()) {
    acelerometro();
    delay(1000);
    scale.set_scale(639737);
    acelerometro();
    delay(500);
    scale.tare();
    long reading = scale.read();
    Serial.print("HX711 reading: ");
    Serial.println(reading);

    acelerometro();
    delay(500);
    return reading;
  } else {
    Serial.println("HX711 not found.");
  }
  delay(500);

  return -1;
}

// ---- SERVO MOTOR
void ativar_alcapao_measuring_container(void) {
  delay(5000);
  acelerometro();
  servoMeasuringContainer.write(175);
  delay(5000);
  // Verificar pelo potenciometro se o servo foi ativao corretamente        <- 1 Potenciometro
  // int servoPosition = map(analogRead(POTENCIOMETER_MEASURING_CONTAINER_PIN), 0, 4096, 0, 180);
  // if ((servoPosition > 200) || (servoPosition < -20)) {
  //   errorsProcessGui[1] = 20;
  // }
  acelerometro();
  servoMeasuringContainer.write(0);
  delay(5000);
  // Verificar pelo potenciometro se o servo foi ativao corretamente        <- 1 Potenciometro
  // int servoPosition = map(analogRead(POTENCIOMETER_MEASURING_CONTAINER_PIN), 0, 4096, 0, 180);
  // if ((servoPosition > 200) || (servoPosition < -20)) {
  //   errorsProcessGui[1] = 20;
  // }
  acelerometro();
  servoMeasuringContainer.write(175);
  delay(2000);
  // Verificar pelo potenciometro se o servo foi ativao corretamente        <- 1 Potenciometro
  // int servoPosition = map(analogRead(POTENCIOMETER_MEASURING_CONTAINER_PIN), 0, 4096, 0, 180);
  // if ((servoPosition > 200) || (servoPosition < -20)) {
  //   errorsProcessGui[1] = 20;
  // }
}

// -----------------------------------------------------------------------------

// --------------------------------------------- MEASURING PHASE
int measuringPhase(void) {
  // Acender o led da fase de medicao                                       <- 1 LED
  digitalWrite(LED_COLLECTING_PHASE, LOW);
  digitalWrite(LED_MEASURING_PHASE, HIGH);
  digitalWrite(LED_RETURNING_PHASE, LOW);

  // Fazer o motor de vibracao acionar                                      <- Motor de Vibracao
  vibrar_measuring_container();

  // Fazer o motor de passo acionar para frente e para tras                 <- Motor de passo
  //       deixa em um loop ateh acionar os fim de cursos                   <- 2 Fim de Curso
  nivelamento_amostra(1, 1);

  // Faz a medida da amostra                                                <- Celula de carga
  // int peso = medicao_amostra();

  // Ativa o servo motor para liberar o alcapao                             <- 1 Servo Motor
  ativar_alcapao_measuring_container();

  // return peso;
  return 0;
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
    acelerometro();
    delay(100);
  }

  resetServos();
}

void loop() {}

// --------------------------------------------- LOOP MAIN --------------------------------------------- 
void loop_processo(void) {
  // ---------------- Isso eu ainda nao vou implementar, estah pronto mas ainda n vou fazer funcionar junto aqui
  // Verificar a conexao com o wifi para comecar a maquina    <- Modilo WIFI
  // -------------------------------------------------
  // -------------------------------------------------


  // Verificar se a porta de cima e o container de retorno estao no lugar   <- 2 FIM DE CURSO
  resetServos();
  while(porta_entrada_OU_sem_container_coleta()){
    delay(100);
    int container_coleta = digitalRead(RETURNING_CONTAINER_FIM_CURSO_PIN);
    int porta = digitalRead(COLLECTING_CONTAINER_PORTINHA_FIM_CURSO_PIN);

    int err[2];
    int n = 0;
    if (container_coleta == LOW){
      err[n] = 40;
      n++;
    }
    if (porta == LOW){
      err[n] = 60;
      n++;
    }
    setError(err, n);
  }
  setRunning();

  // Iniciando os valores padroes do acelerometro
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  med_x_inicial = a.acceleration.x;
  med_y_inicial = a.acceleration.y;
  med_z_inicial = a.acceleration.z;


  

  collectingPhase();
  // Acender led da fase de coleta                                     <- 1 LED
  // Verificar o sensor IR se está acusando o nivel certo                   <- Sensor IR
  // Ativar o servo motor do alcapao 1                                      <- 1 Servo Motor
  // Verificar pelo potenciometro se o servo foi ativao corretamente        <- 1 Potenciometro



  
  int peso = measuringPhase();
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

  // Chama a funcao para mandar os dados pro banco
  // fim_processo(peso, 0.712);
  fim_processo(144.2, 0.712);
}

// --------------------------------------------- VOID LOOP --------------------------------------------- 
void task_processo(void *pvParameters)
{
    while (1)
    {
      while(!iniciar){
        delay(500);
      }
      char i;
      for(i = 0; i < 8; i++){
        errorsProcessGui[i] = 0;
      }
      loop_processo();
      // if(Serial.available()){
      //   int err[] = {10, 20};
      //   switch(Serial.read()){
      //     case '0':
      //       setIdle();
      //       break;
      //     case '1':
      //       setRunning();
      //       break;
      //     case '2':
      //       setError(err, 2);
      //       break;
      //     case '3':
      //       fim_processo(1, 10.0, 10.0, err, 0);
      //       break;
      //   }
      // }
      // delay(1000);
    }
}

void task_monitor()
{
    if(status == 1){
      int container_coleta = digitalRead(RETURNING_CONTAINER_FIM_CURSO_PIN);
      int porta = digitalRead(COLLECTING_CONTAINER_PORTINHA_FIM_CURSO_PIN);

      if (container_coleta == LOW){
        errorsProcessGui[3] = 40;
      }
      if (porta == LOW){
        errorsProcessGui[5] = 60;
      }
    }
}

void task_comunicacao(void *pvParameters)
{
    while (1)
    {
        digitalWrite(LED_WIFI, WiFiStatus);
        Serial.println("comunica");
        Serial.println(iniciar);
        //Espera WiFi subir
        while(WiFi.status() != WL_CONNECTED){
          WiFiStatus = 0; 
          task_monitor();
          delay(50);
        }
        WiFiStatus = 1;
        task_monitor();
        WiFiClient client;
        HTTPClient http;
      
        //Inicia HTTPClient
        http.begin(client, apiUrl);

        http.addHeader("Content-Type", "application/json");
        http.setConnectTimeout(3000000);
        
        StaticJsonDocument<2000> doc;
        
        doc["uuid"] = "4688356cecb043a78b15e4e101b7fdfd";
        doc["secret"] = "6KMayg5Vw5PTUb52iMz8OUT7JvSFH21i5PSpdvRFPosfjFoAXSIjFIWrdoj2wyFcG7Lf1q6Z4pnYe8cGW3h0lVL4vCAV23Kl0R26M30SyOqmiy62JcxHqfcSVs58kVdTXggXZnpy5dg1cdiutFb2QACphXSbK6Y970bHwYSVaSgKxcjlDrAatAxYX8BkTxYqsBL8VEytIoZehDGiPLtFHEW2Rg0wWA4meefDBUSRUoW2GAPm1qRY2gPV1iMdxMr0";
        
        int _flagFim = enviarFim;

        Serial.print("Status");
        Serial.println(status);
        Serial.print("Fim");
        Serial.println(_flagFim);

        if(_flagFim == 1){
          iniciar = 0;
          doc["status"] = 3;
          
          JsonObject proc = doc.createNestedObject("process");
          
          proc["weight"] = endWeight;
          proc["hectoliter"] = endHectoliter;
          proc["status"] = procStatus;
          
          if(procStatus == 2){
            int i;
            JsonArray errs = proc.createNestedArray("errors");
            for(i = 0; i < errorsPVectorSize; i++){
              errs.add(errorsPVector[i]);
            }
          }
        }
        else if(status == 2){
          int i;
          doc["status"] = 2;
          
          JsonArray errs = doc.createNestedArray("errors");
          for(i = 0; i < errorsMVectorSize; i++){
            errs.add(errorsMVector[i]);
          }
        }
        else{
          doc["status"] = status;
        }
        
        String _Body;
        serializeJson(doc, _Body);
        
        Serial.println(_Body);
        int httpResponseCode = http.POST(_Body);
      
        Serial.println(httpResponseCode);
        if(httpResponseCode > 0){
          if(httpResponseCode >= 200 && httpResponseCode < 300){
            if(_flagFim){
              enviarFim = 0;
              status = 0;
            }
            else{
              StaticJsonDocument<2000> resp;
              deserializeJson(resp, http.getStream());

              String _Teste;
              serializeJson(resp, _Teste);
              Serial.println(_Teste);
            
              if(resp["newProcess"]){
                iniciar = 1;
              }
              else{
                iniciar = 0;
              }
            }
          }
          else{
            WiFiStatus = 2;

            StaticJsonDocument<2000> resp;
              deserializeJson(resp, http.getStream());

              String _Teste;
              serializeJson(resp, _Teste);
              Serial.println(_Teste);
          }
        }
        else{
          WiFiStatus = 2;
          Serial.println(http.errorToString(httpResponseCode).c_str());
        }

        http.end();

        task_monitor();
        
        delay(5000);
    }
}
