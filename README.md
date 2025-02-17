# Controle de LEDs e Display com Joystick no Raspberry Pi Pico W

## Descrição do Projeto
Este projeto utiliza um **Raspberry Pi Pico W** e a placa **BitDogLab** para controlar um **LED RGB** e um **display OLED SSD1306** via joystick. O joystick fornece valores analógicos que são utilizados para ajustar a intensidade dos LEDs e movimentar um quadrado no display.

## Vídeo de demonstração


## Objetivos
- Compreender o funcionamento do conversor Analógico-Digital (ADC) no RP2040.
- Utilizar **PWM** para controlar a intensidade luminosa de dois LEDs RGB com base nos valores do joystick.
- Representar a posição do joystick no display **SSD1306** através de um quadrado móvel.
- Aplicar o protocolo de comunicação **I2C** para interação com o display.
- Implementar interrupções para lidar com eventos de botão.

## Funcionalidades
### Controle dos LEDs RGB
- **LED Azul (GPIO 12)**: Brilho ajustado com base no eixo **Y** do joystick.
  - Posição central (2048): LED apagado.
  - Movimento para cima ou para baixo aumenta a intensidade até o máximo (0 ou 4095).
- **LED Vermelho (GPIO 13)**: Brilho ajustado com base no eixo **X** do joystick.
  - Posição central (2048): LED apagado.
  - Movimento para esquerda ou direita aumenta a intensidade até o máximo (0 ou 4095).

### Movimentação no Display SSD1306 (128x64)
- Um **quadrado de 8x8 pixels** se move proporcionalmente aos valores do joystick.
- A posição do quadrado é atualizada conforme os eixos X e Y do joystick.

### Controle via Botões
- **Botão do Joystick (GPIO 22)**:
  - Alterna o estado do **LED Verde (GPIO 11)** a cada acionamento.
  - Altera o estilo da borda do display para indicar o acionamento.
- **Botão A (GPIO 5)**:
  - Ativa ou desativa os LEDs controlados por PWM.

## Componentes Utilizados
- **Raspberry Pi Pico W**;
- **Placa BitDogLab**;
- **Joystick (GPIO 26 e 27 para X/Y, GPIO 22 para botão)**;
- **Display OLED SSD1306 (I2C - GPIO 14 e 15)**;
- **LED RGB (GPIO 11, 12 e 13)**;
- **Botão A (GPIO 5)**.

## Requisitos do Projeto
1. **Uso de Interrupções:** Todas as funcionalidades dos botões devem ser implementadas com **IRQ**;
2. **Debouncing:** Tratamento de bouncing via software;
3. **Display 128x64:** Utilização de gráficos para exibição dos dados;
4. **Organização do Código:** Estrutura bem definida e comentada para melhor compreensão.

## Como Executar
1. Clone este repositório e compile o código para o Raspberry Pi Pico W;
2. Conecte os componentes conforme o esquema de conexão;
3. Faça o upload do código e execute o programa;
4. Utilize o joystick para controlar os LEDs e o quadrado no display.

## Como Simular
1. Clone este repositório e compile o código;
2. Abra o **diagram.json** e execute a simulação no Wokwi do VS Code.
