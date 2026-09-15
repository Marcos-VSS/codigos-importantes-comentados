#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"

#define led1 2
#define led2 4

//Frequência da onda de PWM, deve estar de acordo com o dispositivo utilizado como saída:
//- LEDs (Dimmer): 1 kHz a 5 kHz
//- Servo Motores: 50 Hz
//- Motores DC: 4 kHz a 20 kHz
//- Buzzer Passivo: 100 Hz a 4 kHz
//- Áudio PWM: 44.1 kHz +
//É preciso seguir isso porque cada hardware usa as variações feitas pelo PWM para um funcionamento próprio, como por exeplo, os 
// servo motores usam-o para se posionar certamente, utilizando o sinal alto obtido a cada quantidade dde milissengundos
#define PWM_FREQ 5000

//Resolução em bits do duty cycle, precisão para controlar o duty
#define PWM_RESOLUTION LEDC_TIMER_8_BIT


void app_main(void)
{
    //Configuração do relógio, onde coloca-se as informações da estrutura básica da onda PWM (sem duty) que será construída depois no(s) canal(is)
    // configurando a precisão e ritmo do sinal
    ledc_timer_config_t timer = {
        //Existem dois modos: LEDC_LOW_SPEED_MODE e LEDC_HIGH_SPEED_MODE
        //No primeiro, as alterações no Duty Cycle são tratadas por uma lógica de hardware simplificada
        //No segundo, as mudanças no duty cycle são processadas diretamente pelo Hardware, operando com clocks de alta frequência, sendo mais 
        // rápido, responsivo, bom para precisão
        //Para modelos de ESPs mais novos, os dois foram integrados, usando-se apenas a nomenclatura LEDC_LOW_SPEED_MOD
        .speed_mode = LEDC_LOW_SPEED_MODE,

        //Timer que será utilizado, possibilidades de 0 a 4
        .timer_num = LEDC_TIMER_0,

        .duty_resolution = PWM_RESOLUTION,
        .freq_hz = PWM_FREQ,

        //Qual fonte de clock vai usar para esse timer, assim, ele configura a melhor fonte de clock para nossas especificações
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer);

    ledc_channel_config_t canal1 = {
        .speed_mode = LEDC_LOW_SPEED_MODE,

        //Canal onde configuramos a onda PWM e seu duty cycle de saída 
        .channel = LEDC_CHANNEL_0,

        .timer_sel = LEDC_TIMER_0,
        .intr_type = LEDC_INTR_DISABLE,

        //Liga o canal ao gpio
        .gpio_num = led1,

        //Quanto tempo na onda PWM o sinal fica alto
        .duty = 0,

        //Em qual momento da onda PWM o sinal começa a ficar alto
        .hpoint = 0
    };
    ledc_channel_config(&canal1);

    //É recomendado que utilize-se canais diferentes para cada dispositivo se ele precisam de um valor de duty diferente, mas pode manter o mesmo timer para os canais
    ledc_channel_config_t canal2 = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_1,
        .timer_sel = LEDC_TIMER_0,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = led2,
        .duty = 255,
        .hpoint = 0
    };
    ledc_channel_config(&canal2);

    while(1){
        for(int i=0; i<256; i++){
            //Somente configura o novo valor de duty cycle em um canal
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, i);

            //Atualiza a saída do canal para o novo valor do duty
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

            
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 255-i);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
            vTaskDelay(pdMS_TO_TICKS(10));
        }
        for(int i=0; i<256; i++){
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 255-i);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, i);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }

}
