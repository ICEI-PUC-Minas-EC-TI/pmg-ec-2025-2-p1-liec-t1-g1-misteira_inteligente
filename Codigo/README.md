# Código do Arduino/ESP

# Projeto de Controle de Misteira Inteligente - ESP32

Este repositório contém o código utilizado no ESP32 para controlar a Misteira Inteligente, projeto acadêmico desenvolvido na disciplina de LIEC. O sistema permite ligar/desligar a misteira, controlar o tempo de cozimento e enviar alertas de fumaça via Bluetooth.

## Funções principais

  ## Comunicação Bluetooth
  _Utiliza a biblioteca BluetoothSerial.h_
  
  _Recebe comandos e envia mensagens para o aplicativo App Inventor_

  ## Leitura do Sensor de Fumaça
  _Lê valores analógicos do sensor (A0)_
  
  _Envia alerta via Bluetooth quando o valor ultrapassa o limiar configurado (LIMIAR_FUMACA = 350)_

  ## Controle do Relé
  _Liga ou desliga a resistência da misteira_
  
  _Define estados: RELE_LIGA = LOW / RELE_DESLIGA = HIGH_
  
  _Evita acionamentos incorretos e garante desligamento seguro_

  ## Temporizador de Cozimento

## Código
Arquivo principal: `codigoEsp32.ino`


Mantenha neste diretório todo o código do Arduino ou ESP. Para isso, salve aqui o arquivo .ino.
