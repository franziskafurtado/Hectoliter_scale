/*
 *   Q0000
 *   AUTOR:   BrincandoComIdeias
 *   LINK:    https://www.youtube.com/brincandocomideias ; https://cursodearduino.net/
 *   COMPRE:  https://www.arducore.com.br/
 *   SKETCH:  Módulo para Arduino HX711
 *   DATA:    04/07/2019
*/

// INCLUSÃO DE BIBLIOTECAS
#include <HX711.h>

// DEFINIÇÕES DE PINOS
#define LOADCELL_DOUT_PIN 19
#define LOADCELL_SCK_PIN 18

// DEFINIÇÕES
#define pesoMin 0.010
#define pesoMax 30.0

#define escala 146.43f

// INSTANCIANDO OBJETOS
HX711 scale;

// DECLARAÇÃO DE VARIÁVEIS  
float medida=0;

void setup() {
  Serial.begin(57600);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN); // CONFIGURANDO OS PINOS DA BALANÇA
  scale.set_scale(escala); // ENVIANDO O VALOR DA ESCALA CALIBRADO

  delay(2000);
  scale.tare(); // ZERANDO A BALANÇA PARA DESCONSIDERAR A MASSA DA ESTRUTURA
  Serial.println("Setup Finalizado!");
}

void loop() {
    
    
  medida = scale.get_units(20); // SALVANDO NA VARIAVEL O VALOR DA MÉDIA DE 5 MEDIDAS
    
  // if (medida <= pesoMin ){ // CONFERE SE A MASSA ESTÁ NA FAIXA VÁLIDA
  //   scale.tare(); // ZERA A BALANÇA CASO A MASSA SEJA MENOR QUE O VALOR MIN
  //   medida = 0;
  //   Serial.println("Tara Configurada!");
  // } else if ( medida >= pesoMax ){
  //   scale.tare(); // ZERA A BALANÇA CASO A MASSA SEJA MAIOR QUE O VALOR MÁX
  //   medida = 0;
  //   Serial.println("Tara Configurada!");
  // } else {
    Serial.println(medida,3);
  // }
  
}

// IMPLEMENTO DE FUNÇÕES
