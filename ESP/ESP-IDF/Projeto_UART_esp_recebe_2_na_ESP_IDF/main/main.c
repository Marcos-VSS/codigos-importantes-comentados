#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include <esp_log.h>

#define LED_GPIO GPIO_NUM_4

#define UART_COM UART_NUM_2
#define RXD2 GPIO_NUM_16
#define TXD2 GPIO_NUM_17

#define BUF_SIZE 1024

void app_main(void)
{
    //Tag utilizada para identificar a origem dos LOGs
    static const char *TAG = "App_Main";

    // =====================
    // Configuração do LED
    // =====================
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << LED_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);

    // =====================
    // Configuração da UART2
    // =====================
    uart_config_t uart2_config = {
        .baud_rate = 9600,

        //O equivalente a "SERIAL_8N1" no arduino
        .data_bits = UART_DATA_8_BITS,

        //Bit extra de verificação de erro (a pesquisar melhor)
        .parity    = UART_PARITY_DISABLE,

        //Quantidade de bits que indicam parada de envio de dados. O padrão mais moderno é 1 bit
        .stop_bits = UART_STOP_BITS_1,

        //Controla o fluxo de dados. Se desativado, a UART envia dados sem perguntar se o outro lado 
        // está pronto (a pesquisar melhor)
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    //Utilizada para enfim configurar a UART que iremos utilizar, pegando a struct anterior e escrevendo
    // seus valores dentro dos registradores do periférico UART2
    uart_param_config(UART_COM, &uart2_config);

    //Liga a UART selecionada com os pinos físicos RX2 TX2 escolhidos
    //UART_PIN_NO_CHANGE tem haver com o controle de fluxo que desativamos (RTS e CTS)
    uart_set_pin(UART_COM, TXD2, RXD2, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    //Instala o driver de alto nível da UART no FreeRTOS
    //Sem essa linha uart_read_bytes() e uart_write_bytes() não funcionam e não há buffer
    //O BUF_SIZE indica a quantidade de dados (bytes) pode ficar armazenado temporariamente esperando
    // serem lidos com uart_read_bytes()
    //O primeiro 0 significa "Sem buffer de transmissão adicional", sendo menos eficiente para envio grande
    //O segundo 0 diz que não estou usando fila de eventos
    //Outros parâmetros, a pesquisar melhor
    uart_driver_install(UART_COM, BUF_SIZE, 0, 0, NULL, 0);

    uint8_t data[1];
    int alterna = 0;
    int64_t ultima_vez = 0;

    while (1)
    {
        //"UART_COM" é de onde (de qual UART) queremos ler informações
        //"data" é para onde vai o que será lido pela função. Configuramos ela como um array de tamanho
        // 1 porque essa função sempre espera um bloco de memória configurado como buffer, uma lista de 
        // ponteiros que levam a espaços para ela preencher com as informações recebidas
        //"1" é a quantidade máxima de bytes que queremos ler do que foi acumulado no buffer
        //"pdMS_TO_TICKS(10)" é o tempo máximo que ele vai esperar para achar uma informação, se achar,
        // retorna a quantidade de bytes lidos na variável len, se não achar, retorna 0
        int len = uart_read_bytes(UART_COM, data, 1, pdMS_TO_TICKS(10));

        if (len > 0)
        {
            if (data[0] == 'A')
            {
                gpio_set_level(LED_GPIO, 1);
                alterna = 0;
            }

            if (data[0] == 'a')
            {
                gpio_set_level(LED_GPIO, 0);
                alterna = 1;
            }
            
            //lembrando que esp_timer_get_time() retorna o tempo em microssegundos, sendo preciso dividir 
            // por 1000000 para obter o tempo em segundos
            int64_t agora = esp_timer_get_time() / 1000000;

            if (agora - ultima_vez >= 1)
            {
                if (alterna == 0){
                    uart_write_bytes(UART_COM, "b", 1); //O 1 indica que vai enviar só 1 caractere
                    ESP_LOGI(TAG,"b");
                    alterna = 1;
                }
                else{
                    uart_write_bytes(UART_COM, "B", 1);
                    ESP_LOGI(TAG,"B");
                    alterna = 0;
                }
                ultima_vez = agora;
            }
        }
    }
}