#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_log.h>
#include "driver/adc.h"
#include "driver/ledc.h"

#define PIN_POT GPIO_NUM_35

#define SERVO_GPIO        4
#define SERVO_FREQ        50
#define SERVO_RESOLUTION  LEDC_TIMER_16_BIT
#define SERVO_CHANNEL     LEDC_CHANNEL_0
#define SERVO_MODE        LEDC_LOW_SPEED_MODE
#define SERVO_TIMER       LEDC_TIMER_0

void potTask(void*);

//Funciona como o map() do arduino
//long = 4 bytes (32 bits), com sinal
float map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Converte ângulo (0–180) em duty
uint32_t angle_to_duty(int angle)
{
  // Pulso mínimo e máximo em microssegundos
  uint32_t min_us = 500;   // 0°
  uint32_t max_us = 2500;  // 180°

  //Mesmo princípio da map()
  uint32_t pulse_width = min_us + ((max_us - min_us) * angle) / 180;

  // Período do PWM em microssegundos (50Hz = 20ms)
  uint32_t period_us = 20000;

  // Cálculo do duty baseado na resolução
  uint32_t max_duty = (1 << 16) - 1; //max_duty = 2^16 - 1 = 65536 − 1 = 65535

  return (pulse_width * max_duty) / period_us;
}

void app_main() {
  adc1_config_width(ADC_WIDTH_BIT_10);
  adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);

  // Configuração do timer
  ledc_timer_config_t timer = {
    .speed_mode       = SERVO_MODE,
    .timer_num        = SERVO_TIMER,
    .duty_resolution  = SERVO_RESOLUTION,
    .freq_hz          = SERVO_FREQ,
    .clk_cfg          = LEDC_AUTO_CLK
  };
  ledc_timer_config(&timer);

  // Configuração do canal
  ledc_channel_config_t channel = {
    .gpio_num       = SERVO_GPIO,
    .speed_mode     = SERVO_MODE,
    .channel        = SERVO_CHANNEL,
    .timer_sel      = SERVO_TIMER,
    .duty           = 0,
    .hpoint         = 0
  };
  ledc_channel_config(&channel);

  //xTaskCreatePinnedToCore(potTask, "potTask", 2048, NULL, 1, NULL, 0);

  while (1) {
    int entrada = adc1_get_raw(ADC1_CHANNEL_7);
    int angulo = map(entrada, 0, 1023, 0, 179);
    printf("Ang: %d\n", angulo);
    ledc_set_duty(SERVO_MODE, SERVO_CHANNEL, angle_to_duty(angulo));
    ledc_update_duty(SERVO_MODE, SERVO_CHANNEL);
    vTaskDelay(pdMS_TO_TICKS(15));
  }
}

/*
Lógica de angle_to_duty:

- converter ângulo em duty

"min_us" é a largura mínima, em microssegundos, que o sinal alto do PWM pode ter para ser enviado ao Servo
"max_us" é a largura máxima, em microssegundos, que o sinal alto do PWM pode ter para ser enviado ao Servo

pulse_width é um valor, em microssegundos, no intervalo entre a largura mínima e máxima do pulso que está 
localizado de forma proporcional ao valor do ângulo obtido entre o intervalo 0 - 180 graus. Ele é obtido 
com a mesma lógica da função map():
- obtemos o tamanho do intervalo em "max_us - min_us", que é de 2000us
- fazemos uma regra de três entre intervalo máximo <-> ângulo máximo e intervalo esperado <-> angulo obtido
  (é o mesmo que multiplicar o intervalo máximo pela proporção entre o ângulo obtido e o ângulo máximo)
- somar o intervalo mínimo no final, para começarmos a função do valor de pulso no ponto mínimo possível

"period_us = 20000;" lembrando que a frequência é 50Hz. O valor obtido é o mesmo de "max_us - min_us" nesse
caso, mas não misturar, pois representam coisas diferentes na teoria

"max_duty" representa o valor máximo, em bits, que podemos atribuir ao tempo de sinal alto da onda PWM

"(pulse_width * max_duty) / period_us" conversão do tempo de pulso do sinal alto em número de bits. Temos
uma regra de três entre max_duty (que deixa a onda inteira em sinal alto) <-> period_us (que pega o tempo
entre o início oa final da onda inteira) e valor do duty <-> pulse width (uma fração do período)

*/