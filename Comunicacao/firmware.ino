#include <WiFi.h>
#include <HTTPClient.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <ArduinoJson.h>

int status = 0;
int iniciar = 0;
int enviarFim;
int errorsMVector[20];
int errorsMVectorSize;

int errorsPVector[20];
int errorsPVectorSize;

double endHectoliter;
double endWeight;
int procStatus;

int erros[8] = {10, 20, 30, 40, 50, 60, 70, 80};
char dados_finais[4][20] = {};

const char* ssid = "NEOSILOS-2_4G";
const char* password = "ueWKFEXx-NEOSILOS";
const char* apiUrl = "https://joaopedrogalera.pythonanywhere.com:80/machineSync";
int WiFiStatus = 0;


void setup()
{
    /* Adicionat setup do código do Gui aqui */
  
    Serial.begin(9600);
    //Inicializa WiFi
    WiFi.begin(ssid, password);

    // Cria threads
    xTaskCreate(task_comunicacao, "Comunicacao", 4096, NULL, 5, NULL);
    xTaskCreate(task_processo, "Processo", 4096, NULL, 5, NULL);
}

void loop(){}

void task_processo(void *pvParameters)
{
    while (1)
    {
      if(Serial.available()){
        int err[] = {10, 20};
        switch(Serial.read()){
          case '0':
            setIdle();
            break;
          case '1':
            setRunning();
            break;
          case '2':
            setError(err, 2);
            break;
          case '3':
            fim_processo(1, 10.0, 10.0, err, 0);
            break;
        }
      }
      delay(1000);
    }
}

void setIdle(){
  status = 0;
}

void setRunning(){
  status = 1;
}

void setError(int* list, int size){
  int i = 0;
  status = 2;
  
  errorsMVectorSize = size;
  
  for(i = 0; i < size; i++){
    errorsMVector[i] = list[i];
  }
}

void fim_processo(int prcstatus, double weight, double hectoliter, int* errors, int nerros)
{
    int i = 0;
    enviarFim = 1;
    status = 3;
    
    endHectoliter = hectoliter;
    endWeight = weight;
    procStatus = prcstatus;
    
    errorsPVectorSize = nerros;
  
    for(i = 0; i < nerros; i++){
      errorsPVector[i] = errors[i];
    }
    
}

void task_comunicacao(void *pvParameters)
{
    while (1)
    {
        Serial.println("comunica");
        Serial.println(iniciar);
        //Espera WiFi subir
        while(WiFi.status() != WL_CONNECTED){
          WiFiStatus = 0; 
          delay(500);
        }
        WiFiStatus = 1;
      
        WiFiClient client;
        HTTPClient http;
      
        //Inicia HTTPClient
        http.begin(client, apiUrl);

        http.addHeader("Content-Type", "application/json");
        
        StaticJsonDocument<2000> doc;
        
        doc["uuid"] = "4688356cecb043a78b15e4e101b7fdfd";
        doc["secret"] = "6KMayg5Vw5PTUb52iMz8OUT7JvSFH21i5PSpdvRFPosfjFoAXSIjFIWrdoj2wyFcG7Lf1q6Z4pnYe8cGW3h0lVL4vCAV23Kl0R26M30SyOqmiy62JcxHqfcSVs58kVdTXggXZnpy5dg1cdiutFb2QACphXSbK6Y970bHwYSVaSgKxcjlDrAatAxYX8BkTxYqsBL8VEytIoZehDGiPLtFHEW2Rg0wWA4meefDBUSRUoW2GAPm1qRY2gPV1iMdxMr0";
        
        if(enviarFim == 1){
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
      
        if(httpResponseCode > 0){
          if(httpResponseCode >= 200 && httpResponseCode < 300){
            if(status == 3 && enviarFim){
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
        }

        http.end();
        
        delay(10000);
    }
}