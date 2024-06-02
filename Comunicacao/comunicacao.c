#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

int status = 0;
int iniciar = 0;
int erros[8] = {10, 20, 30, 40, 50, 60, 70, 80};
char dados_finais[4][20] = {""};

QueueHandle_t fila_status;
QueueHandle_t fila_iniciar;
QueueHandle_t fila_erros;
QueueHandle_t fila_dados_finais;

void task_processo(void *pvParameters)
{
    while (1)
    {
        // Recebe iniciar
        xQueueReceive(fila_iniciar, &iniciar, portMAX_DELAY);

        // Envia status
        xQueueSend(fila_status, &status, portMAX_DELAY);

        // Envia erros
        xQueueSend(fila_erros, &erros, portMAX_DELAY);

        if (status == 3)
        {
            fim_processo();
            break;
        }
    }
}

void fim_processo()
{
    // Envia dados finais
    xQueueSend(fila_dados_finais, dados_finais, portMAX_DELAY);

    // Envia status
    xQueueSend(fila_status, &status, portMAX_DELAY);
}

void task_comunicacao(void *pvParameters)
{
    int status_local;
    int erros_recebidos[8];
    char dados_recebidos[4][20];

    while (1)
    {
        // Envia iniciar
        xQueueSend(fila_iniciar, &iniciar, portMAX_DELAY);

        // Recebe status
        xQueueReceive(fila_status, &status_local, portMAX_DELAY);

        // Recebe erros
        xQueueReceive(fila_erros, &erros_recebidos, portMAX_DELAY);

        // Recebe dados finais
        xQueueReceive(fila_dados_finais, &dados_recebidos, portMAX_DELAY);
    }
}

void app_main()
{
    // Inicializa filas
    fila_status = xQueueCreate(1, sizeof(int));
    fila_iniciar = xQueueCreate(1, sizeof(int));
    fila_erros = xQueueCreate(1, sizeof(int) * 8);
    fila_dados_finais = xQueueCreate(1, sizeof(dados_finais));

    // Cria threads
    xTaskCreate(task_comunicacao, "Comunicacao", 4096, NULL, 5, NULL);
    xTaskCreate(task_processo, "Processo", 4096, NULL, 5, NULL);
}