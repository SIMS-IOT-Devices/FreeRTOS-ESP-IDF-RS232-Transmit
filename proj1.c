// Transmitting a string via RS232

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "soc/uart_struct.h"
#include "string.h"

#define UART_0_TX 1
#define UART_0_RX 3

void init_RS232()
{
    const uart_port_t uart_num = UART_NUM_0;
    const int uart_buffer_size = 1024;
    QueueHandle_t uart_queue;

    // 1 - Setting Communication Parameters
    const uart_config_t uart_config = {             
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
    uart_param_config(uart_num, &uart_config);
    
    // 2 - Setting Communication Pins
    uart_set_pin(uart_num, UART_0_TX, UART_0_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    // 3 - Driver Installation
    uart_driver_install(uart_num, uart_buffer_size, uart_buffer_size, 10, &uart_queue, 0);
}

static void tx_task()
{
    char *data = "Hello world";
    
    // 4 - Running UART Communication
    while (1)
    {
        int len = strlen(data);
        int txBytes = uart_write_bytes(UART_NUM_0, data, len);
        printf("%d", txBytes);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    printf("Send data:\n");
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    init_RS232();
    xTaskCreate(tx_task, "uart_tx_task", 1024 * 2, NULL, configMAX_PRIORITIES - 1, NULL);
}