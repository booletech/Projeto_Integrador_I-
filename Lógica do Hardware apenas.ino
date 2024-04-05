#include <Arduino.h>

// Pinos ultrassônicos
const int triggerPin = 22;
const int echoPin = 23;

// Pinos dos LEDs
const int ledRed = 4;    // Vazio
const int ledYellow = 16; // Quase Vazio
const int ledWhite = 17;  // Metade
const int ledGreen = 19;  // Quase Cheio
const int ledBlue = 21;   // Cheio

unsigned long ultimaMedicao = 0; // Armazena a última vez que uma medição foi feita
const long intervalo = 5000; // Intervalo de 5 segundos
int alturaReservatorio = 100; // Defina a altura do reservatório aqui

void setup() {
  Serial.begin(115200);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledWhite, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);

  Serial.println("Sistema inicializado, aguardando altura do reservatorio...");
}

float medirDistancia() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  long duracao = pulseIn(echoPin, HIGH);
  float distancia = (duracao * 0.0343) / 2;
  return distancia;
}

void atualizarLEDs(float porcentagem) {
  digitalWrite(ledRed, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledWhite, LOW);
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledBlue, LOW);

  if (porcentagem > 80) digitalWrite(ledBlue, HIGH);
  if (porcentagem > 60) digitalWrite(ledGreen, HIGH);
  if (porcentagem > 40) digitalWrite(ledWhite, HIGH);
  if (porcentagem > 20) digitalWrite(ledYellow, HIGH);
  digitalWrite(ledRed, HIGH); // Acende para qualquer nível abaixo de 100%

  // Logica para exibir a mensagem correspondente
  if(porcentagem > 80) Serial.println("CHEIO");
  else if(porcentagem > 60) Serial.println("QUASE CHEIO");
  else if(porcentagem > 40) Serial.println("METADE");
  else if(porcentagem > 20) Serial.println("QUASE VAZIO");
  else Serial.println("VAZIO");
}

void loop() {
  unsigned long tempoAtual = millis();
  if (tempoAtual - ultimaMedicao >= intervalo) {
    ultimaMedicao = tempoAtual;

    float distanciaMedia = 0;
    for (int i = 0; i < 5; i++) {
      distanciaMedia += medirDistancia();
      delay(100); // Pequena pausa entre medições
    }
    distanciaMedia /= 5;
    
    // Calcula a porcentagem do nível da água
    float nivelAgua = alturaReservatorio - distanciaMedia; // Subtrai a distância medida da altura total
    float porcentagem = (nivelAgua / alturaReservatorio) * 100;

    Serial.print("Distancia media: ");
    Serial.println(distanciaMedia);
    Serial.print("Porcentagem do nivel da agua: ");
    Serial.println(porcentagem);

    atualizarLEDs(porcentagem);
  }
}
