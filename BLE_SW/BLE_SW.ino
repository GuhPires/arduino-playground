// Controle de LED através de Bluetooth BLE e App Inventor (Android)

#include <SoftwareSerial.h>
SoftwareSerial BTSerial(11, 10); //RX|TX

// Pinos (terminais) dos LEDs, um para cada cor:
const int PinoVermelho = 7;
const int PinoVerde = 6;
const int PinoAzul = 5;

void setup() {
  // Inicializa a porta serial com a velocidade (baud rate) padrão:
  Serial.begin(9600);
  // Inicializa a porta serial do Bluetooth com a velocidade (baud rate) padrão:
  BTSerial.begin(9600);
  // Comando necessário quando se utiliza o Bluettoth:
  while(!Serial);
  // Define os terminais como sendo de saída:
  pinMode(PinoVermelho, OUTPUT);
  pinMode(PinoVerde, OUTPUT);
  pinMode(PinoAzul, OUTPUT);
  // Apresenta mensagem inicial no Monitor Serial:
  Serial.println("Sistema pronto...");
}

/* A seguir começamos o nosso programa. 
 * Esta função monitora a porta serial onde o módulo Bluetooth está ligado
 * e aguarda que algo seja escrito neste canal. Assim que uma informação
 * surgir nela, o código a seguir é executado. */

void loop(){
  // Aguarda a chegada de algum dado na porta serial de entrada RX:
  while (BTSerial.available() > 0) {
    delay(200);
    
    // Aguarda o sinal de início da transmissão:
    
    // IMPORTANTE: Fazer o casting "(char)" SE o BTSerial.read() retornar um HEX,
    //             caso contrário remover o mesmo.
    if ((char)BTSerial.read() == '<') {

      /* A seguir, o Arduino fica a espera do nosso "Olá!", indicando o início da
       * nossa conversa com o Android. Uma vez que ele encontra (condição verdadeira),
       * ele executa o código a seguir. */
    
      // Procura o primeiro valor char válido no fluxo de dados do sinal serial, que será a identificação da cor:
      char cor = BTSerial.read();
      // Procura o primeiro valor inteiro válido no fluxo de dados do sinal serial, que será a intensidade da luz:
      int intensidade = BTSerial.parseInt();

      /* O código procura por uma letra e a armazena na variável "cor". Após procura por um número 
       *  inteiro (de 0 a 255) e o armazena na variável "intensidade". Quando surge o caractere 
       *  ">" ele entende que o valor inteiro terminou (parseInt).
       *  Terminados as duas informações que estávamos esperando, o Arduino procura pelo nosso "Tchau!". */
      
      // Procura o sinal de final da transmissão:

      // IMPORTANTE: Fazer o casting "(char)" SE o BTSerial.read() retornar um HEX,
      //             caso contrário remover o mesmo.
      if ((char)BTSerial.read() == '>') {

        /* Uma vez encontrado, ele executa a próxima parte do código. O Arduino pega os
         * dois valores armazenados e envia o valor da cor para a porta digital PWM onde
         * o respectivo LED está ligado. */
        
        // Verifica qual o LED (cor) selecionado e envia o sinal digital para o respectivo terminal:
        switch(cor) {
          case 'R': // R = Red/Vermelho
            // Acende ou apaga o LED na intensidade recebida:
            analogWrite(PinoVermelho, intensidade);
            // Apresenta o valor recebido do celular pelo Arduino através do módulo Bluetooth no Monitor Serial:
            Serial.print("Vermelho: ");
            Serial.println(intensidade);
            break;
          case 'G': // G = Green/Verde
            // Acende ou apaga o LED na intensidade recebida:
            analogWrite(PinoVerde, intensidade);
            // Apresenta o valor recebido do celular pelo Arduino através do módulo Bluetooth no Monitor Serial:
            Serial.print("Verde: ");
            Serial.println(intensidade);
            break;
          case 'B': // B = Blue/Azul
            // Acende ou apaga o LED na intensidade recebida:
            analogWrite(PinoAzul, intensidade);
            // Apresenta o valor recebido do celular pelo Arduino através do módulo Bluetooth no Monitor Serial:
            Serial.print("Azul: ");
            Serial.println(intensidade);
            break;
          default:
            break;
        }
      }
    }
  }
}
