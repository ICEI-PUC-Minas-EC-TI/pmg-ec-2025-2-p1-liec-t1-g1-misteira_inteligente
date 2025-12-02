#include "BluetoothSerial.h"

// --- CONFIGURAÇÃO DO RELÉ ---
#define RELE_PINO 22
#define RELE_LOGICA_LIGA LOW
#define RELE_LOGICA_DESLIGA HIGH

// --- CONFIGURAÇÃO DO SENSOR MQ-2 ---
#define PINO_SENSOR A0
int valorSensor = 0;
#define LIMIAR_FUMACA 350 // ajuste conforme seu sensor

// --- VARIÁVEIS DE CONTROLE ---
bool misteiraLigada = false;
unsigned long tempoInicioMisteira = 0;
unsigned long duracaoCozimento_ms = 0;

String comandoRecebido = "";

// Controle de tempo do alerta de fumaça (evita enviar muitas mensagens)
unsigned long ultimoAlertaFumaca = 0;
const unsigned long intervaloAlertaFumaca = 1000; // 1 segundo

// --- INSTÂNCIA BLUETOOTH ---
BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_MISTEIRA"); // Nome do dispositivo Bluetooth
  Serial.println("Bluetooth 'ESP32_MISTEIRA' iniciado.");

  pinMode(RELE_PINO, OUTPUT);
  digitalWrite(RELE_PINO, RELE_LOGICA_DESLIGA);

  Serial.println("MISTEIRA pronta. Envie comandos via Bluetooth: ON, OFF, 90, 120, 150");
}

void loop() {

  unsigned long agora = millis();

  // --- LÊ SENSOR DE FUMAÇA ---
  valorSensor = analogRead(PINO_SENSOR);
  if (valorSensor > LIMIAR_FUMACA && agora - ultimoAlertaFumaca >= intervaloAlertaFumaca) {
    SerialBT.println("ALERTA: FUMACA DETECTADA!");
    Serial.println("ALERTA: FUMACA DETECTADA!");
    ultimoAlertaFumaca = agora; // registra o último alerta
  }

  // --- LÊ COMANDOS VIA BLUETOOTH ---
  while (SerialBT.available()) {
    char c = SerialBT.read();
    if (c != '\n' && c != '\r') {
      comandoRecebido += c;
    } else {
      comandoRecebido.trim();
      if (comandoRecebido.length() > 0) {

        Serial.print("Comando recebido: ");
        Serial.println(comandoRecebido);

        // --- LIGA/DESLIGA ---
        if (comandoRecebido == "ON") {
          digitalWrite(RELE_PINO, RELE_LOGICA_LIGA);
          misteiraLigada = true;
          duracaoCozimento_ms = 0;
          SerialBT.println("MISTEIRA LIGADA MANUALMENTE");
        } 
        else if (comandoRecebido == "OFF") {
          digitalWrite(RELE_PINO, RELE_LOGICA_DESLIGA);
          misteiraLigada = false;
          duracaoCozimento_ms = 0;
          SerialBT.println("MISTEIRA DESLIGADA");
        } 
        // --- TEMPO ---
        else if (comandoRecebido == "90" || comandoRecebido == "120" || comandoRecebido == "150") {
          int tempo = comandoRecebido.toInt();
          duracaoCozimento_ms = (unsigned long)tempo * 1000UL;
          tempoInicioMisteira = agora;
          digitalWrite(RELE_PINO, RELE_LOGICA_LIGA);
          misteiraLigada = true;
          SerialBT.print("MISTEIRA ligada com timer de ");
          SerialBT.print(tempo);
          SerialBT.println(" segundos");
        } 
        else {
          SerialBT.println("Comando inválido! Use ON, OFF, 90, 120 ou 150.");
        }

      }
      comandoRecebido = ""; // zera buffer
    }
  }

  // --- VERIFICA TEMPORIZADOR ---
  if (misteiraLigada && duracaoCozimento_ms > 0) {
    if (agora - tempoInicioMisteira >= duracaoCozimento_ms) {
      digitalWrite(RELE_PINO, RELE_LOGICA_DESLIGA);
      misteiraLigada = false;
      duracaoCozimento_ms = 0;
      SerialBT.println("Temporizador concluído. MISTEIRA DESLIGADA");
    }
  }

  delay(20); // pequena pausa
}