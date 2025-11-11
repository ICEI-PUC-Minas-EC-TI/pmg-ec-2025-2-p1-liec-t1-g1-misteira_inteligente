#include <OneWire.h>
#include <DallasTemperature.h>

// ===== PINOS =====
#define ONE_WIRE_BUS 4    // Pino do DS18B20
#define RELAY_PIN 5       // Pino do relé
#define PROX_PIN 18       // Pino do sensor de proximidade

// ===== OBJETOS =====
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ===== ESTRUTURA DE MODOS =====
enum Modo {MAL_PASSADO, AO_PONTO, BEM_PASSADO};
Modo modoSelecionado = AO_PONTO;

// ===== VARIÁVEIS GLOBAIS =====
float temperaturaAtual = 0;
bool tampaAberta = false;
bool releLigado = false;
unsigned long tempoPico = 0;
bool atingiuPico = false;

void setup() {
  Serial.begin(115200);
  sensors.begin();

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(PROX_PIN, INPUT_PULLUP); // saída NPN -> LOW = ativo

  digitalWrite(RELAY_PIN, LOW);
  Serial.println("=== Sistema iniciado ===");
}

void loop() {
  // --- 1. Ler temperatura ---
  sensors.requestTemperatures();
  temperaturaAtual = sensors.getTempCByIndex(0);

  // --- 2. Ler sensor de tampa ---
  int estadoProx = digitalRead(PROX_PIN);
  tampaAberta = (estadoProx == LOW); // depende do sensor: NPN = LOW quando ativo

  Serial.print("Temperatura: ");
  Serial.print(temperaturaAtual);
  Serial.print("°C | Tampa: ");
  Serial.println(tampaAberta ? "ABERTA" : "FECHADA");

  // --- 3. Segurança ---
  if (temperaturaAtual > 120) {
    desligarRele();
    Serial.println("⚠️ Temperatura perigosa! Desligando relé!");
    delay(1000);
    return;
  }

  if (tampaAberta) {
    desligarRele();
    Serial.println("⚠️ Tampa aberta! Relé desligado.");
    delay(500);
    return;
  }

  // --- 4. Controle principal ---
  controlarModo(modoSelecionado);

  delay(1000); // leitura a cada 1s
}

void controlarModo(Modo modo) {
  float pico = 0;
  unsigned long tempoPosPico = 0;

  switch (modo) {
    case MAL_PASSADO:
      pico = 70; tempoPosPico = 5000; break;
    case AO_PONTO:
      pico = 80; tempoPosPico = 10000; break;
    case BEM_PASSADO:
      pico = 90; tempoPosPico = 15000; break;
  }

  if (!atingiuPico) {
    if (temperaturaAtual < pico) {
      ligarRele();
    } else {
      desligarRele();
      atingiuPico = true;
      tempoPico = millis();
      Serial.println("🔥 Pico atingido!");
    }
  } else {
    // Espera o tempo pós-pico
    if (millis() - tempoPico >= tempoPosPico) {
      desligarRele();
      atingiuPico = false;
      Serial.println("✅ Ponto atingido! Desligando relé.");
    }
  }
}

void ligarRele() {
  digitalWrite(RELAY_PIN, HIGH);
  releLigado = true;
}

void desligarRele() {
  digitalWrite(RELAY_PIN, LOW);
  releLigado = false;
}
