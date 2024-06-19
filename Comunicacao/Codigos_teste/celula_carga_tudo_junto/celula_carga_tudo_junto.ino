/*
     Q0481-Sketch-Calibrar
     AUTOR:   BrincandoComIdeias
     LINK:    https://www.youtube.com/brincandocomideias ; https://cursodearduino.net/
     COMPRE:  https://www.arducore.com.br/
     SKETCH:  Calibrar Balanca com módulo HX711
     DATA:    04/07/2019
*/

// INCLUSÃO DE BIBLIOTECAS
#include <HX711.h>

// DEFINIÇÕES DE PINOS
#define LOADCELL_DOUT_PIN 19
#define LOADCELL_SCK_PIN 18
// med / 208.5
// INSTANCIANDO OBJETOS
HX711 scale;

// DECLARAÇÃO DE VARIÁVEIS
float medida = 0;
float escala = 0.0;
int cont = 0;

void setup() {
  Serial.begin(57600);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN); // CONFIGURANDO OS PINOS DA BALANÇA

  scale.set_scale(); // LIMPANDO O VALOR DA ESCALA
  scale.tare(); // ZERANDO A BALANÇA PARA DESCONSIDERAR A MASSA DA ESTRUTURA
}

void calibra(void) {
  

  delay(2000);

  Serial.println("Balança Zerada");

  medida = scale.get_units(50); // SALVANDO NA VARIAVEL O VALOR DA MÉDIA DE 5 MEDIDAS

  scale.power_down(); // DESLIGANDO O SENSOR
  delay(1000); // AGUARDA 5 SEGUNDOS
  scale.power_up(); // LIGANDO O SENSOR

  escala = medida / 149.4;

  scale.set_scale(escala); // ENVIANDO O VALOR DA ESCALA CALIBRADO

  delay(2000);
  Serial.println("Setup Finalizado!");

  medida = scale.get_units(50); // SALVANDO NA VARIAVEL O VALOR DA MÉDIA DE 5 MEDIDAS

  Serial.println(medida,3);
}

void loop() {
  if (!cont)
    calibra();
  cont = 1;
}
