#define trig 7
#define echo 4

float duracao;
float distancia;

void setup()
{
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  Serial.begin(9600);
}

void loop()
{
  //Dá um pulso, liberando ondas do trig por 10 microssegundos e logo após o próprio sensor liga o echo, deixa em 1
  digitalWrite(trig,1);
  delayMicroseconds(10);
  digitalWrite(trig,0);

  //Verifica o tempo em que as ondas levaram até chegar ao echo (quando chegam, o sensor desliga o echo, deixa em 0)
  duracao=pulseIn(echo,1);
  distancia=duracao*0.0173; 
  //0.0173 vem da conversão de 343m/s da velocidade do som para cm/microsegundos/ depois divide-se por 2, pois queremos multiplicar a duração somente de ida do sinal
  //deltax=v*deltat
  //Lembrando que a velocidade do som depende do meio
  
  Serial.println(distancia);
  delay(250);
}
