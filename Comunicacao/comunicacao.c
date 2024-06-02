#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define NUM_ERROS 8

typedef struct
{
    int erros[NUM_ERROS];
} Erros;

int status = 0;
int iniciar = 0;
Erros lista_erros = {{10, 20, 30, 40, 50, 60, 70, 80}};

// Definição das filas para comunicação entre as threads
QueueHandle_t fila_status;
QueueHandle_t fila_iniciar;
QueueHandle_t fila_erros;

void task_comunicacao(void *pvParameters)
{
    int status_local;

    while (1)
    {
        // Recebe 'status'
        xQueueReceive(fila_status, &status_local, portMAX_DELAY);

        // Envia 'iniciar'
        xQueueSend(fila_iniciar, &iniciar, portMAX_DELAY);
    }
}

void app_main()
{
    // Inicializa filas
    fila_status = xQueueCreate(1, sizeof(int));
    fila_iniciar = xQueueCreate(1, sizeof(int));
    fila_erros = xQueueCreate(1, sizeof(Erros));

    // Cria threads
    xTaskCreate(task_comunicacao, "Comunicacao", 4096, NULL, 5, NULL);
    xTaskCreate(task_processo, "Processo", 4096, NULL, 5, NULL);
}
