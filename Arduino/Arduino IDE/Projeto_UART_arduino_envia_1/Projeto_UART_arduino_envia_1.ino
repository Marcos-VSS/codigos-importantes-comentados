//Código pra enviar dados do potenciômetro pra ESP32

/*void setup()
{
  Serial.begin(115200);
  pinMode(A1, INPUT);
}

void loop()
{
  int entrada=map(analogRead(A1),0,1023,0,255);
  //char pot[4];
  //sprintf(pot,"%d",entrada);
  Serial.print(entrada);
  Serial.print(" ");
}*/

//Código pra receber informações do push buttom da ESP32

const int BUFFER_SIZE = 5;
char buffer[BUFFER_SIZE];

void setup() {
  Serial.begin(115200);
  pinMode(3,OUTPUT);
}

void loop() {
  if(Serial.available()>0){
    int bytesLidos = Serial.readBytes(buffer, BUFFER_SIZE - 1);
    buffer[bytesLidos] = '\0';

    int power=atoi(buffer);
    Serial.println(power);

    analogWrite(3,power);
  }
}

//Exemplo de código de entrada no pino RX2 do Arduino

/*void setup() {
  Serial.begin(115200);
  pinMode(2,OUTPUT);
}

void loop() {
  if(Serial.available()>0){
    char c=Serial.read();
    if(c=='A')
      digitalWrite(2,1);
    if(c=='a')
      digitalWrite(2,0);
  }
}*/

//Exemplo de código de saída no pino TX2 do Arduino

/*void setup() {
  // A velocidade DEVE ser a mesma do ESP32 (115200)
  Serial.begin(115200); 
}

void loop() {
  Serial.print('A'); // Envia A para ligar o LED no ESP32
  delay(1000);
  Serial.print('a'); // Envia a para desligar o LED no ESP32
  delay(1000);
}*/
