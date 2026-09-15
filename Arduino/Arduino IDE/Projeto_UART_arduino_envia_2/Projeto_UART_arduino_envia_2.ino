unsigned long time;
unsigned long ultima_vez;
int qtd=0;

void setup() {
  Serial.begin(9600);
  pinMode(7,INPUT_PULLUP);
  pinMode(3,OUTPUT);
  
}
void loop() {
  Serial.print('A');
  ultima_vez=millis();
  qtd++;
  while(time-ultima_vez<1001){
    time=millis();
    if(Serial.available()>0){
      qtd=0;
      char c=Serial.read();
      if(c=='B')
        digitalWrite(3,1);
      if(c=='b')
        digitalWrite(3,0);
    }
  }
  Serial.print('a');
  ultima_vez=millis();
  qtd++;
  while(time-ultima_vez<1001){
    time=millis();
    if(Serial.available()>0){
      qtd=0;
      char c=Serial.read();
      if(c=='B')
        digitalWrite(3,1);
      if(c=='b')
        digitalWrite(3,0);
    }
  }

  /*if(qtd==4){
    Serial.print(" Cade voce? ");
    qtd=0;
  }

  /*if(digitalRead(7)==0){
    Serial.println("Ola!");
    ultima_vez=time;
  }
  else if(time-ultima_vez==3000){
    Serial.println("Cade voce?");
    ultima_vez=time;
  }*/
}
