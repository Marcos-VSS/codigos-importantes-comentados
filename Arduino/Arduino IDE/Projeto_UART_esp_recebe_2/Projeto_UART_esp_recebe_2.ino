#define RXD2 16
#define TXD2 17

//Sem sinal e com 64 bits (seriam 32 positivos e 32 negativos se tivesse sinal), necessário para armazenar o tempo da função millis()
unsigned long ultima_vez=0;
int alterna=0;

void setup() {
  Serial.begin(9600); // Monitor Serial (USB)
  //SERIAL_8N1 é o padrão de recebimento de dados como pacote de 8 bits (1 byte)
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  pinMode(4,OUTPUT);
}

void loop() {
  //Verifica se algum dado chegou no Serial2
  if(Serial2.available()>0){
    char c=Serial2.read();
    if(c=='A'){
      digitalWrite(4,1);
      alterna=0;
    }
    if(c=='a'){
      digitalWrite(4,0);
      alterna=1;
    }

    if(millis()-ultima_vez>=1000){
      if(alterna==0){
        Serial2.print('b');
        Serial.print('b');
        alterna=1;
      }
      else if(alterna==1){
        Serial2.print('B');
        Serial.print('B');
        alterna=0;
      }
      ultima_vez=millis();
    }
  }
}
