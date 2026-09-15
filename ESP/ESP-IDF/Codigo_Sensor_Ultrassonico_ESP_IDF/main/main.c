/* PARA UM SENSOR ULTRASSÔNICO */

#include <stdio.h>
#include "driver/gpio.h"

//Para usar o esp_timer_get_time(), que conta o tempo total partindo de quando a ESP foi ligada
#include "esp_timer.h"

//Para utilizar a memória ultra-rápida na função de interrupção
#include "esp_attr.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//Para usar o esp_rom_delay_us(10), que pausa TODAS as tarefas por microsegundos
#include "esp_rom_sys.h"

#define trig1 GPIO_NUM_16
#define echo1 GPIO_NUM_4

//Indica uma variável privada (que funciona de forma análoga à função privada) de tipo int64_t
// e declarada como volátil, ou seja, a partir daí, o sistema vai entender que não é para fazer
// otimizações nela durante o uso, pois ela pode mudar o conteúdo a qualquer momento
static volatile int64_t tempo_comeco = 0;
static volatile int64_t duracao = 0;

//É uma função privada (static), ou seja, só vista e chamada dentro deste arquivo .c e vive
// globalmente durante todo esse programa. Ela roda direto da memória ultra-rápida (IRAM_ATTR)
// para responder imediatamente ao sensor. Recebe um parâmetro genérico (void *arg), sendo exigido
// pelo colocá-lo por conta de padrões do sistema de interrupção, mesmo que não o utilizemos
static void IRAM_ATTR gpio_isr(void *arg)
{
  int nivel = gpio_get_level(echo1);

  //diz que é uma variável inteira de 64bits, ou seja, tem tipo definido ("_t")
  int64_t tempo_agora = esp_timer_get_time();

  if (nivel == 1) {
    //Marca o período em que o sensor mandou as ondas
    tempo_comeco = tempo_agora;
  } else {
    //A duração da "viagem" entre as ondas e o corpo é a diferença entre o período que o
    // sensor recebeu as ondas e o perído que o sensor mandou as ondas
    duracao = tempo_agora - tempo_comeco;
  }
}

void app_main(void)
{
  //struct de configuração dos pinos utilizados para echo
  gpio_config_t echos = {
    //É importante utilizar ULL, que significa Unsigned Long Long, usado para dizer ao
    // compilador que aquele número deve ser tratado como um inteiro sem sinal, ou seja,
    // somente positivo, de 64 bits, não só 32 como geralmente é o padrão de inteiros,
    // permitindo configurar todas as portas com segurança
    .pin_bit_mask = (1ULL << echo1),
    .mode = GPIO_MODE_INPUT,
    //Diz que a sensibilidade à interrupção deve ser nos dois lados
    .intr_type = GPIO_INTR_ANYEDGE
  };

  //declaração dos pinos de echo conforme a struct
  gpio_config(&echos);

  //struct de configuração dos pinos utilizados para trig
  gpio_config_t trigs = {
    .pin_bit_mask = (1ULL << trig1),
    .mode = GPIO_MODE_OUTPUT,
  };

  //declaração dos pinos de trig conforme a struct
  gpio_config(&trigs);

  //Instalação de um gerenciador central de interrupções, permitindo que o ESP32 identifique
  // e execute funções de interrupção específicas para cada GPIO, economizando recursos do
  // processador. O parâmetro 0 indica a prioridade, que é padrão no caso do 0
  gpio_install_isr_service(0);

  //declaração de uma "atividade de interrupção", dizendo o pino onde terá a interrupção,
  // a função que ele deve fazer quando ocorrer a interrupção e o parâmetro a ser passado para
  // a função
  gpio_isr_handler_add(echo1, gpio_isr, NULL);

  while (1) {
    gpio_set_level(trig1, 1);
    esp_rom_delay_us(10);
    gpio_set_level(trig1, 0);

    //if (duracao > 0) {
    float distancia = duracao * 0.0173;
    printf("Distancia: %f cm\n", distancia);
    //duracao = 0;
    //}
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

/*

  Lógica do sensor ultrassônico:

  - Pulso de Gatilho (Trig): Você envia um pulso de 10µs.
  - Emissão Sonora: O sensor emite 8 pulsos de ultrassom a 40kHz.
  - Resposta do Echo: Assim que o sensor termina de enviar o som, ele coloca o pino Echo em nível
  alto (1).
  - O Retorno: O pino Echo permanece em 1 até que o som bata em um objeto e volte para o sensor.
  No momento em que o sensor detecta o ricochete (eco), ele coloca o Echo em nível baixo (0).

*/

/* PARA DOIS SENSORES ULTRASSÔNICOS */

/*
#include <stdio.h>
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_attr.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_rom_sys.h"

//Array que guarda o número dos pinos de echo, útil para reutilizar a função gpio_isr
int pinos_echo[2]={4,26};

//Array que guarda o número dos pinos de echo, útil para algum tipo de laço de repetição,
// se for preciso
int pinos_trig[2]={16,27};

//Array que guarda os tempos de começo da contagem da interrupção de cada sensor, útil para
// reutilizar a função gpio_isr
static volatile int64_t tempo_comeco[2] = {0,0};

//Array que guarda o a duração em que o sinal foi e chegou de cada sensor, útil para algum
// tipo de laço de repetição, se for preciso
static volatile int64_t duracao[2]={0,0};

static void IRAM_ATTR gpio_isr(void *arg)
{
  //Obtenção do argumento dado pelo serviço de interrupção, que indicará qual porta foi
  // interrompida, ou seja, o sensor que está trabalhando no momento
  int sensor=(int) arg;
  int nivel = gpio_get_level(pinos_echo[sensor]);
  int64_t tempo_agora = esp_timer_get_time();

  if (nivel == 1) {
    tempo_comeco[sensor] = tempo_agora;
  } else {
    duracao[sensor] = tempo_agora - tempo_comeco[sensor];
  }
}

void app_main(void)
{
  gpio_config_t echos = {
    //O | significa que o bit mask deve ser feito (vale) para ambos, semelhante ao OU lógico
    .pin_bit_mask = (1ULL << pinos_echo[0]) | (1ULL << pinos_echo[1]),
    .mode = GPIO_MODE_INPUT,
    .intr_type = GPIO_INTR_ANYEDGE
  };
  gpio_config(&echos);

  gpio_config_t trigs = {
    .pin_bit_mask = (1ULL << pinos_trig[0]) | (1ULL << pinos_trig[1]),
    .mode = GPIO_MODE_OUTPUT,
  };
  gpio_config(&trigs);

  gpio_install_isr_service(0);
  
  // O (void*) trata-se de um ponteiro genérico, ou seja, ele aponta para um lugar da memória
  // mas não especifica para o compilador que tipo de dado ele vai encontrar lá, sendo preciso
  // para o argumento da nossa função que é void, ou seja, não é declarado o tipo dele
  gpio_isr_handler_add(pinos_echo[0], gpio_isr, (void*) 0);
  gpio_isr_handler_add(pinos_echo[1], gpio_isr, (void*) 1);

  while (1) {
    gpio_set_level(pinos_trig[0], 1);
    gpio_set_level(pinos_trig[1], 1);
    esp_rom_delay_us(10);
    gpio_set_level(pinos_trig[0], 0);
    gpio_set_level(pinos_trig[1], 0);

    //Se quiser as casas decimais da distancia, troque o int para float
    int distancia1 = duracao[0] * 0.0171;
    printf("Distancia 1: %d cm\n", distancia1);
    int distancia2 = duracao[1] * 0.0171;
    printf("-------------------------------------\n");
    printf("Distancia 2: %d cm\n", distancia2);
    printf("-------------------------------------\n");
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}
*/