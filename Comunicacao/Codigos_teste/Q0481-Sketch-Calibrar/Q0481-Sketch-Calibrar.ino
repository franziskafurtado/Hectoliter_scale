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

void setup() {
  Serial.begin(57600);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN); // CONFIGURANDO OS PINOS DA BALANÇA
  scale.set_scale(); // LIMPANDO O VALOR DA ESCALA

  delay(2000);
  scale.tare(); // ZERANDO A BALANÇA PARA DESCONSIDERAR A MASSA DA ESTRUTURA

  Serial.println("Balança Zerada");
}

void loop() {

  medida = scale.get_units(50); // SALVANDO NA VARIAVEL O VALOR DA MÉDIA DE 5 MEDIDAS
  Serial.println(medida, 3); // ENVIANDO PARA MONITOR SERIAL A MEDIDA COM 3 CASAS DECIMAIS

  scale.power_down(); // DESLIGANDO O SENSOR
  delay(1000); // AGUARDA 5 SEGUNDOS
  scale.power_up(); // LIGANDO O SENSOR
}
