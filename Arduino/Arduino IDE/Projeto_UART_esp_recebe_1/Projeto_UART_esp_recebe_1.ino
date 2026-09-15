//Código pra receber dados do potenciômetro do Arduino

/*const int BUFFER_SIZE = 4;
char buffer[BUFFER_SIZE];

#define RXD2 16
#define TXD2 17

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  pinMode(4,OUTPUT);
}

void loop() {
  if (Serial2.available()>0 && Serial2.read()==' '){
    int bytesLidos = Serial2.readBytes(buffer, BUFFER_SIZE - 1);
    buffer[bytesLidos] = '\0';

    Serial.print("Dados do Potenciometro: ");
    Serial.println(buffer);

    int num = atoi(buffer);
    analogWrite(4,num);
  }
}*/

//Código pra enviar informações do push buttom pro arduino 

#define RXD2 16
#define TXD2 17

void setup() {
  Serial.begin(115200); // Monitor Serial (USB)
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  pinMode(27,INPUT_PULLUP);
}

void loop() {
  int i=0;
  if(digitalRead(27)==0){
    delay(300);
    while(i<1023 && digitalRead(27)==0){
      i++;
      delay(10);
      //Serial.println(i);
    }
    Serial2.print(i);
    Serial.println(i);
    delay(2000);
  }
}

//Exemplo de código de saída no pino TX2 da ESP32

/*#define RXD2 16
#define TXD2 17

void setup() {
  Serial.begin(115200); // Monitor Serial (USB)
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
}

void loop() {
  Serial2.print('A');
  Serial.println("Enviado: A");
  delay(1000);
  Serial2.print('a');
  Serial.println("Enviado: a");
  delay(1000);
}*/

//Exemplo de código de entrada no pino RX2 da ESP32

/*#define RXD2 16
#define TXD2 17

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  pinMode(2, OUTPUT);
}

void loop() {
  if(Serial2.available()>0){
    char c = Serial2.read(); // Lê o caractere recebido
    if(c=='A')
      digitalWrite(2,1); // Liga o LED
    else if(c=='a')
      digitalWrite(2,0);
  }
}*/
