//Importa a biblioteca nativa do Arduino para controle PWM 
// de Servo Motores
#include <Servo.h>

//Cria um objeto da classe "Servo"
Servo servo;

void setup()
{
  pinMode(A0,INPUT);
  Serial.begin(9600);
  //Relaciona o objeto criado à uma gpio do Arduino, gerando 
  // o PWM nesse pino
  servo.attach(3);
}

void loop()
{
  float sensor=analogRead(A0);
  int angulo=map(sensor,710,301,0,180);
  Serial.println(angulo);
  
  //Uso o método write, que pega o ângulo fornecido, converte 
  // para duty cycle e gera a onda PWM no gpio ligado ao objeto
  servo.write(angulo);
  delay(15);
}
