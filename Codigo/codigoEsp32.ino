#include "BluetoothSerial.h"

// --- CONFIGURAÇÕES ---
#define RELE_PINO 22
#define RELE_LIGA LOW
#define RELE_DESLIGA HIGH
#define PINO_SENSOR A0
#define LIMIAR_FUMACA 350
#define INTERVALO_ALERTA 10000

BluetoothSerial SerialBT;

bool misteiraLigada = false;
unsigned long inicioCozimento = 0, duracaoCozimento = 0, ultimoAlerta = 0;
String comando = "";

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_MISTEIRA");
  pinMode(RELE_PINO, OUTPUT);
  digitalWrite(RELE_PINO, RELE_DESLIGA);
  Serial.println("MISTEIRA pronta. Comandos: ON, OFF, 90, 120, 150");
}

void loop() {
  unsigned long agora = millis();
  lerSensorFumaca(agora);
  lerComandoBluetooth(agora);
  verificarTemporizador(agora);
  delay(20);
}

// --- FUNÇÕES ---

void lerSensorFumaca(unsigned long agora) {
  if (analogRead(PINO_SENSOR) > LIMIAR_FUMACA && agora - ultimoAlerta >= INTERVALO_ALERTA) {
    SerialBT.println("ALERTA: FUMACA DETECTADA!");
    Serial.println("ALERTA: FUMACA DETECTADA!");
    ultimoAlerta = agora;
  }
}

void lerComandoBluetooth(unsigned long agora) {
  while (SerialBT.available()) {
    char c = SerialBT.read();
    if (c != '\n' && c != '\r') comando += c;
    else {
      comando.trim();
      if (comando.length()) tratarComando(comando, agora);
      comando = "";
    }
  }
}

void tratarComando(String cmd, unsigned long agora) {
  Serial.print("Comando recebido: "); Serial.println(cmd);

  if (cmd == "ON") {
    ligarMisteira(10);
    SerialBT.println("MISTEIRA LIGADA MANUALMENTE");
  } else if (cmd == "OFF") {
    desligarMisteira();
    SerialBT.println("MISTEIRA DESLIGADA");
  } else if (cmd == "90" || cmd == "120" || cmd == "150") {
    int tempo = cmd.toInt();
    ligarMisteira(tempo * 1000UL);
    inicioCozimento = agora;
    SerialBT.printf("MISTEIRA ligada com timer de %d segundos\n", tempo);
  } else {
    SerialBT.println("Comando inválido! Use ON, OFF, 90, 120 ou 150.");
  }
}

void ligarMisteira(unsigned long duracao) {
  digitalWrite(RELE_PINO, RELE_LIGA);
  misteiraLigada = true;
  duracaoCozimento = duracao;
}

void desligarMisteira() {
  digitalWrite(RELE_PINO, RELE_DESLIGA);
  misteiraLigada = false;
  duracaoCozimento = 0;
}

void verificarTemporizador(unsigned long agora) {
  if (misteiraLigada && duracaoCozimento > 0 && agora - inicioCozimento >= duracaoCozimento) {
    desligarMisteira();
    SerialBT.println("Temporizador concluído. MISTEIRA DESLIGADA");
  }
}
