#include <OneWire.h>
#include <DallasTemperature.h>

// ======================= CONFIGURAÇÕES DE PINOS ==========================
#define PINO_TEMPERATURA  2    // DS18B20 -> GPIO 2 (pode mudar conforme ligação)
#define PINO_PROXIMIDADE  5    // Sensor indutivo -> GPIO 5
#define PINO_RELE         13   // Relé -> GPIO 13

// ======================= OBJETOS DOS SENSORES ===========================
OneWire oneWire(PINO_TEMPERATURA);
DallasTemperature sensorTemperatura(&oneWire);

// ======================= VARIÁVEIS GLOBAIS ==============================
float temperaturaAtual = 0;
int estadoProximidade = 0; // 1 = tampa fechada, 0 = aberta
int modoPreparo = 2;       // 1=mal passado, 2=ao ponto, 3=bem passado
bool reléLigado = false;

// Temperaturas-alvo (exemplo)
float tempMalPassado = 80.0;
float tempAoPonto = 100.0;
float tempBemPassado = 120.0;
float tempSeguranca = 130.0; // desligar se ultrapassar isso

unsigned long tempoInicioAquecimento = 0;
unsigned long tempoManterAquecimento = 0;
bool atingiuPico = false;

// ======================= CONFIGURAÇÃO INICIAL ===========================
void setup() {
  Serial.begin(115200);
  sensorTemperatura.begin();

  pinMode(PINO_PROXIMIDADE, INPUT);
  pinMode(PINO_RELE, OUTPUT);
  digitalWrite(PINO_RELE, LOW);

  Serial.println("Sistema de controle de misteira iniciado!");
  Serial.println("Modo: AO PONTO (padrão)");
}

// ======================= LOOP PRINCIPAL ================================
void loop() {
  // Ler sensores
  sensorTemperatura.requestTemperatures();
  temperaturaAtual = sensorTemperatura.getTempCByIndex(0);
  estadoProximidade = digitalRead(PINO_PROXIMIDADE);

  // Exibir leituras
  Serial.print("Temperatura: ");
  Serial.print(temperaturaAtual);
  Serial.print(" °C | Tampa: ");
  Serial.print(estadoProximidade ? "FECHADA" : "ABERTA");
  Serial.print(" | Rele: ");
  Serial.println(reléLigado ? "LIGADO" : "DESLIGADO");

  // Verificar tampa aberta
  if (estadoProximidade == LOW) {
    digitalWrite(PINO_RELE, LOW);
    reléLigado = false;
    Serial.println("⚠️ Tampa aberta! Desligando aquecimento...");
  } else {
    // Controle por modo
    float tempAlvo = (modoPreparo == 1) ? tempMalPassado :
                     (modoPreparo == 2) ? tempAoPonto : tempBemPassado;

    if (!atingiuPico && temperaturaAtual < tempAlvo) {
      digitalWrite(PINO_RELE, HIGH);
      reléLigado = true;
    } else if (temperaturaAtual >= tempAlvo) {
      atingiuPico = true;
      if (tempoInicioAquecimento == 0) tempoInicioAquecimento = millis();
      digitalWrite(PINO_RELE, LOW);
      reléLigado = false;
    }

    // Manter desligado após 10 segundos do pico
    if (atingiuPico && millis() - tempoInicioAquecimento > 10000) {
      Serial.println("✅ Pão pronto!");
      atingiuPico = false;
      tempoInicioAquecimento = 0;
    }
  }

  // Segurança
  if (temperaturaAtual > tempSeguranca) {
    digitalWrite(PINO_RELE, LOW);
    reléLigado = false;
    Serial.println("🔥 Alerta! Temperatura acima do limite de segurança!");
  }

  delay(1000);
}
