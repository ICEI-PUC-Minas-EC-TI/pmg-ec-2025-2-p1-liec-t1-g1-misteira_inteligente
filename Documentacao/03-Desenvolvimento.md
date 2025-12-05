
# Materiais

Os materiais utilizados no projeto foram:

- Esp32.
- Protoboard.
- Sensor de fumaça, sensor de distância, Sensor de temperatura da Misteira.
- Relé.
- 3 Resistores.
- Diodo.
- Transitor.

# Desenvolvimento

O projeto foi desenvolvido ao longo da disciplina de LIEC. Inicialmente, o grupo discutiu ideias que integrassem os conteúdos estudados e, ao mesmo tempo, apresentassem utilidade prática no cotidiano. A partir dessas discussões, definiu-se a criação da Misteira Inteligente.
Em seguida, foram organizadas as principais etapas do desenvolvimento:

- Desenvolvimento do Frontend no App Inventor

- Desenvolvimento do código do ESP32 em C++

- Montagem dos componentes eletrônicos na protoboard

- Realização de testes funcionais e ajustes finais

## Desenvolvimento do Aplicativo

### Interface

A interface foi criada no App Inventor com foco em simplicidade, usabilidade e intuitividade.
A proposta é manter o usuário informado sobre o estado da misteira — temperatura, presença do pão e nível de segurança — permitindo também controlar o preparo. Além disso, o usuário recebe alertas caso se esqueça de desligar o aparelho ou programe um tempo inadequado.
A interface busca otimizar o tempo do usuário durante tarefas cotidianas simples.

### Código

O código do aplicativo foi desenvolvido no App Inventor utilizando blocos visuais. A lógica foi estruturada principalmente para permitir a troca de informações com o ESP32 por Bluetooth. Para isso, configuramos blocos responsáveis por enviar comandos como ON, OFF, 90, 120 e 150, que acionam ou temporizam a misteira.

Também foram criados blocos para receber mensagens enviadas pelo ESP32, como alertas de fumaça ou aviso de que o tempo de cozimento terminou. Cada mensagem recebida atualiza elementos da interface, permitindo ao usuário acompanhar o funcionamento em tempo real.

Durante o desenvolvimento, tivemos alguns ajustes necessários, especialmente na interpretação das strings recebidas, mas o aplicativo conseguiu cumprir as funções básicas de controle e monitoramento.

## Desenvolvimento do Hardware

### Montagem

### Desenvolvimento do Código

O desenvolvimento do código do ESP32 foi realizado em C++ utilizando a IDE Arduino. A lógica principal envolveu três partes: leitura do sensor de fumaça, controle do relé e comunicação Bluetooth com o aplicativo.

Primeiro, configuramos os pinos do relé e do sensor, definindo limites de leitura para detecção de fumaça. Em seguida, implementamos a estrutura para receber comandos via Bluetooth, como ON, OFF, 90, 120 e 150, que representam modos de cozimento com temporizações diferentes.

Foi criada uma função para monitorar continuamente o tempo de cozimento, desligando a misteira automaticamente quando o temporizador chegava ao fim. Também programamos alertas de fumaça, enviados tanto pela serial quanto via Bluetooth.

Durante o desenvolvimento, alguns problemas surgiram, como leituras instáveis do sensor e comportamentos inesperados do relé. Com testes sucessivos e ajustes nos limites e temporizações, o sistema conseguiu atuar de forma consistente, ainda que não perfeita. No final, o código permitiu que o usuário controlasse o aparelho pelo celular, monitorando seu funcionamento em tempo real.

## Comunicação entre App e Hardware

A comunicação entre o aplicativo e o ESP32 foi feita por Bluetooth, utilizando o módulo interno do próprio microcontrolador. Essa etapa se mostrou uma das mais difíceis do projeto, principalmente pela limitação de componentes do App Inventor e pela necessidade de sincronizar envio e leitura de dados. Tivemos dificuldades significativas para fazer o aplicativo reconhecer e comunicar adequadamente com o hardware.
Depois de vários testes, ajustes no código e reconstrução de partes do app, conseguimos estabelecer a comunicação de forma funcional. Apesar de não ser o sistema mais robusto, a conexão permitiu enviar comandos (como ON/OFF e tempos pré-definidos) e receber alertas, como detecção de fumaça e conclusão do temporizador.
