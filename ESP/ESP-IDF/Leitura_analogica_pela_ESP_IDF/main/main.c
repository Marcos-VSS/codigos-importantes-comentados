//Código para leitura das GPIOS ADC1
//Avisos: recomendado (funciona junto com Wi-Fi)

/*#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"

void app_main(void)
{
    //Configura a resolução do ADC1: 12 bits (0–4095)
    adc1_config_width(ADC_WIDTH_BIT_12);

    //Configura a atenuação do canal (que você pode trocar entre outros canais da ADC1)
    //Atenuação é a tensão máxima que o sensor consegue medir, se ultrapassar essa tensão a leitura fica
    // sempre saturada (no valor máximo), se ultrapassar muito essa tensão, a pode queimar a porta
    //Valores de atenuação:
    //- ADC_ATTEN_DB_0 = Tensão máxima ~1.1 V
    //- ADC_ATTEN_DB_2_5 = Tensão máxima ~1.5 V
    //- ADC_ATTEN_DB_6 = Tensão máxima ~2.2 V
    //- ADC_ATTEN_DB_11 = Tensão máxima ~3.9 V (mais comum)
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);

    while (1) {
        //Leitura analógica do canal para a variável (equivalente ao analogRead)
        int valor_adc = adc1_get_raw(ADC1_CHANNEL_6);

        printf("GPIO 34 (ADC1): %d\n", valor_adc);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}*/

//Código para leitura das GPIOS ADC2
//Avisos: conflita com Wi-Fi (evite se possível)

/*#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"

void app_main(void)
{
    int valor_adc = 0;

    // Configura atenuação do ADC2 canal 0 (GPIO4)
    adc2_config_channel_atten(ADC2_CHANNEL_0, ADC_ATTEN_DB_11);

    while (1) {
        //O ADC2 não retorna propriamente a leitura do canal, mas sim um código de erro do tipo esp_err_t,
        // sendo que, se a leitura for bem sucedida (wi-fi não interromper), retorna "ESP_OK"
        esp_err_t ret = adc2_get_raw(
            //O canal onde ler
            ADC2_CHANNEL_0,
            //Configura a resolução do ADC1: 12 bits (0–4095)
            ADC_WIDTH_BIT_12,
            //Ponteiro para a um espaço de memória de uma variável onde a função vai colocar o valor lido
            &valor_adc
        );

        if (ret == ESP_OK) {
            printf("GPIO 4 (ADC): %d\n", valor_adc);
        } else {
            printf("Erro ao ler ADC2 (Wi-Fi ativo?)\n");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}*/

//Código para leitura das GPIOS ADC1 e ADC2
//Avisos: ADC2 continua conflitando com Wi-Fi: O driver tenta acessar; Se o Wi-Fi estiver ativo → erro
//Estudar melhor esse código se for usar, mas eu acho melhor usar os outros por conta das fuynções mais 
// simples

/*#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_adc/adc_oneshot.h"

void app_main(void)
{
    adc_oneshot_unit_handle_t adc_handle;

    adc_oneshot_unit_init_cfg_t init_cfg = {
        .unit_id = ADC_UNIT_1, // ou ADC_UNIT_2
    };
    adc_oneshot_new_unit(&init_cfg, &adc_handle);

    adc_oneshot_chan_cfg_t chan_cfg = {
        .atten = ADC_ATTEN_DB_11,
        .bitwidth = ADC_BITWIDTH_12,
    };

    // GPIO34 → ADC1_CHANNEL_6
    adc_oneshot_config_channel(adc_handle, ADC_CHANNEL_6, &chan_cfg);

    while (1) {
        int valor;
        adc_oneshot_read(adc_handle, ADC_CHANNEL_6, &valor);

        printf("ADC: %d\n", valor);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}*/


/* 
Portas do ADC1:
- 36 = ADC1_CHANNEL_0
- 37 = ADC1_CHANNEL_1
- 38 = ADC1_CHANNEL_2
- 39 = ADC1_CHANNEL_3
- 32 = ADC1_CHANNEL_4
- 33 = ADC1_CHANNEL_5
- 34 = ADC1_CHANNEL_6
- 35 = ADC1_CHANNEL_7

Portas do ADC2:
- 4 = ADC2_CHANNEL_0
- 0 = ADC2_CHANNEL_1
- 2 = ADC2_CHANNEL_2
- 15 = ADC2_CHANNEL_3
- 13 = ADC2_CHANNEL_4
- 12 = ADC2_CHANNEL_5
- 14 = ADC2_CHANNEL_6
- 27 = ADC2_CHANNEL_7
- 25 = ADC2_CHANNEL_8
- 26 = ADC2_CHANNEL_9

Avisos importantes:
- GPIs (Input Only): Os pinos GPIO 34, 35, 36 e 39 pertencem ao bloco ADC1, mas são fisicamente conectados 
apenas como entradas. Eles não possuem transistores de pull-up/pull-down internos nem drivers de saída. 
Portanto, eles nunca funcionarão como saída, independentemente do software.
- Strapping Pins: Pinos como GPIO 0, 2, 5, 12 e 15 são usados para determinar o modo de boot do chip. Se 
você forçá-los como saída com uma carga pesada durante a inicialização, o ESP32 pode não ligar ou falhar 
ao carregar o código.

Código que procura o valor de tensão de referência e, por meio de funções matemáticas, calibra bem a tensão
recebida e já transforma o valor lido em mV,lembrando que ele funciona apenas com ADC1:

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_adc_cal.h" // Biblioteca necessária para calibração

void app_main(void)
{
    // 1. Estrutura para armazenar as características de calibração
    esp_adc_cal_characteristics_t adc_chars;

    // 2. Configura a resolução e atenuação
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);

    // 3. Caracteriza o ADC (Gera a "tabela" de correção baseada no hardware real)
    // O valor 1100 é a referência de tensão padrão (VRef) em mV.
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);

    while (1) {
        // 4. Leitura do valor bruto (0 a 4095)
        int raw = adc1_get_raw(ADC1_CHANNEL_6);

        // 5. Converte o valor bruto para Voltagem real usando a calibração
        uint32_t voltage = esp_adc_cal_raw_to_voltage(raw, &adc_chars);

        printf("Valor Raw: %d | Tensão Real: %u mV\n", raw, voltage);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
*/