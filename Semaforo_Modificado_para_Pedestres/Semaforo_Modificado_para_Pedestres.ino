// Semáforo de 2 fases - Modificado para pedestrtes
// Autor: Gustavo G. Pires

// As variáveis de saída do sistema (luzes dos semáforos e timer):
unsigned long timer = 0;  // contagem de tempo desde a conclusão da última transição de estado;
int RA = 2;               // luz vermelha (Red) do semáforo da via A;
int YA = 3;               // luz amarela (Yellow) do semáforo da via A;
int GA = 4;               // luz verde (Green) do semáforo da via A;
int RB = 5;               // luz vermelha (Red) do semáforo da via B;
int YB = 6;               // luz amarela (Yellow) do semáforo da via B;
int GB = 7;               // luz verde (Green) do semáforo da via B;
int PR = 8;               // Luz vermelha (Red) de pedestre;
int PG = 9;               // Luz verde (Green) de pedestre.

// Variáveis de entrada (botões de pedestre):
int BP = 10;              // Botão (Pedestrian)

// Os tempos serão dados pelas variáveis:
int tY = 1000;            // tempo durante o qual a luz amarela (Yellow) permanecerá acesa;
int tS = 500;             // tempo de segurança durante o qual ambas as luzes vermelhas permanecerão simultaneamente acesas antes de uma delas mudar para o verde;
int tGA = 5000;           // tempo durante o qual a luz verde (Green) da via A permanecerá acesa;
int tGB = 3000;           // tempo durante o qual a luz verde (Green) da via B permanecerá acesa.
int tP = 5000;            // Tempo de travessia (Pedestrian)

// Cria a variável de estado:
char estado = 'A';
bool pedestrian = false;        // Variável de controle de solicitação de travessia

void setup() {
  // declara os pinos das luzes dos semáforos como sendo de saída:
  pinMode(RA, OUTPUT);
  pinMode(YA, OUTPUT);
  pinMode(GA, OUTPUT);
  pinMode(RB, OUTPUT);
  pinMode(YB, OUTPUT);
  pinMode(GB, OUTPUT);
  pinMode(PR, OUTPUT);
  pinMode(PG, OUTPUT);

  // Botões do pedestre
  pinMode(BP, INPUT);
  
  // acende as luzes amarelas:
  digitalWrite(YA, HIGH);
  digitalWrite(YB, HIGH);

  // Acende luzes vermelhas para pedestre:
  digitalWrite(PR, HIGH);

  Serial.begin(9600);
  
  // gera um atraso durante o qual as lâmpadas amarelas ficam acesas antes de iniciar a máquina de estados:
  delay(tY);
}

void loop() {
  // Lê estado do botão de pedestres
  bool cross = digitalRead(BP);

  if(cross) pedestrian = true;
  
  Serial.print("Pedestre: ");
  Serial.println(pedestrian);
  
  switch (estado) {
    case 'A':
      // chama a função do Estado_A:
      Estado_A();
      // Conta o tempo transcorrido desde o último valor atribuído ao timer e compara com o intervalo de tempo desejado:
      if ((millis() - timer) > tGB) {
        // próximo estado:
        estado = 'B';
        // Timer recebe o valor da contagem de tempo atual do Arduino para a contagem de tempo relativa:
        timer = millis();
      }
      break;

    case 'B':
      // chama a função do Estado_B:
      Estado_B();
      // Conta o tempo transcorrido desde o último valor atribuído ao timer e compara com o intervalo de tempo desejado:
      if ((millis() - timer) > tY) {
        // próximo estado:
        estado = 'C';
        // Timer recebe o valor da contagem de tempo atual do Arduino para a contagem de tempo relativa:
        timer = millis();
      }
      break;
    
    case 'C':
      // chama a função do Estado_C:
      Estado_C();
      // Conta o tempo transcorrido desde o último valor atribuído ao timer e compara com o intervalo de tempo desejado:
      if ((millis() - timer) > tS) {
        // próximo estado:
        estado = 'D';
        // Timer recebe o valor da contagem de tempo atual do Arduino para a contagem de tempo relativa:
        timer = millis();
      }
      break;

    case 'D':
      // chama a função do Estado_D:
      Estado_D();
      // Conta o tempo transcorrido desde o último valor atribuído ao timer e compara com o intervalo de tempo desejado:
      if ((millis() - timer) > tGA) {
        // próximo estado:
        estado = 'E';
        // Timer recebe o valor da contagem de tempo atual do Arduino para a contagem de tempo relativa:
        timer = millis();
      }
      break;

    case 'E':
      // chama a função do Estado_E:
      Estado_E();
      // Conta o tempo transcorrido desde o último valor atribuído ao timer e compara com o intervalo de tempo desejado:
      if ((millis() - timer) > tY) {
        // próximo estado:
        estado = 'F';
        // Timer recebe o valor da contagem de tempo atual do Arduino para a contagem de tempo relativa:
        timer = millis();
      }
      break;

    case 'F':
      // chama a função do Estado_F:
      Estado_F();
      // Conta o tempo transcorrido desde o último valor atribuído ao timer e compara com o intervalo de tempo desejado:
      if ((millis() - timer) > tS) {
        // próximo estado:
        estado = 'A';
        // Timer recebe o valor da contagem de tempo atual do Arduino para a contagem de tempo relativa:
        timer = millis();
      }
      break;
  }
}

// Funções de cada Estado:
void Estado_A() {
  // Semáforo A:
  digitalWrite(RA, HIGH);
  digitalWrite(YA, LOW);
  digitalWrite(GA, LOW);
  // Semáforo B:
  digitalWrite(RB, LOW);
  digitalWrite(YB, LOW);
  digitalWrite(GB, HIGH);
}

void Estado_B() {
  // Semáforo B:
  digitalWrite(YB, HIGH);
  digitalWrite(GB, LOW);
}

void Estado_C() {
  // Semáforo B:
  digitalWrite(RB, HIGH);
  digitalWrite(YB, LOW);

  // Nesta função se encontra o final de um ciclo.  
  
  // Verifica se o botão de pedestre foi pressionado  
  if(pedestrian) {
    crossStreet();
  }
}

void Estado_D() {
  // Semáforo A:
  digitalWrite(RA, LOW);
  digitalWrite(GA, HIGH);
}

void Estado_E() {
  // Semáforo A:
  digitalWrite(YA, HIGH);
  digitalWrite(GA, LOW);
}

void Estado_F() {
  // Semáforo A:
  digitalWrite(RA, HIGH);
  digitalWrite(YA, LOW);

  // Nesta função se encontra o final de um ciclo.  
  
  // Verifica se o botão de pedestre foi pressionado  
  if(pedestrian) {
    crossStreet();
  }
}

void crossStreet() {
  delay(tS);
  digitalWrite(PR, LOW);
  digitalWrite(PG, HIGH);
  delay(tP);
  digitalWrite(PG, LOW);
  for(int i = 0; i < 4; i++){
    digitalWrite(PR, HIGH);
    delay(500);
    digitalWrite(PR, LOW);
    delay(500);
  }
  digitalWrite(PR, HIGH);
  delay(tS);

  pedestrian = false;
}
