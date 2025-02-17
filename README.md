# Controle de Servomotor por PWM com Raspberry Pi Pico W

## Descrição do Projeto
Este projeto utiliza o microcontrolador **Raspberry Pi Pico W** para controlar um **servo motor** através de um sinal **PWM (Pulse Width Modulation)**. O código foi desenvolvido utilizando a ferramenta **Pico SDK** e pode ser simulado na plataforma online **Wokwi**.

## Vídeo de demonstração
https://youtu.be/t99qElfitgs

## Objetivo
O objetivo deste projeto é demonstrar como controlar o **ângulo do servo motor** ajustando o ciclo de trabalho (duty cycle) do sinal PWM gerado pelo **RP2040**, microcontrolador presente no Raspberry Pi Pico W.

## Componentes Utilizados
1. **Microcontrolador Raspberry Pi Pico W**
2. **Servomotor (Motor Micro Servo Padrão) - Simulador Wokwi**

## Configuração do PWM
- **Frequência:** 50 Hz (Período de 20 ms)
- **Pino de controle:** GPIO **22** (para simulação) ou GPIO **12** (para uso na placa real)
- **Valores de Pulso:**
  - **2400 µs** → **180 graus**
  - **1470 µs** → **90 graus**
  - **500 µs** → **0 graus**
  
## Funcionamento do Código
O programa realiza os seguintes passos:
1. Configura a **GPIO 22** para gerar um sinal PWM de **50 Hz**.
2. Define o servo em três posições iniciais, aguardando **5 segundos** em cada uma:
   - **180 graus (2400 µs)**
   - **90 graus (1470 µs)**
   - **0 graus (500 µs)**
3. Cria um **movimento suave e contínuo** entre **0 e 180 graus**, ajustando o pulso em **incrementos de 5 µs** a cada **10 ms**.

## Estrutura do Código
O código principal é dividido em:
1. **`config_servo_pwm()`** → Configura o PWM na GPIO definida.
2. **`set_servo_us(uint us)`** → Define o pulso PWM baseado no tempo em microsegundos.
3. **`smooth_movement(uint start_us, uint end_us)`** → Realiza a movimentação suave do servo entre dois ângulos.
4. **`main()`** → Executa a lógica do projeto, movimentando o servo entre 0 e 180 graus repetidamente.

## Requisitos e Critérios de Avaliação
| Item | Requisito | Nota |
|------|-----------|------|
| 1 | Configuração do PWM na GPIO 22 com 50Hz | **20%** |
| 2 | Ajuste do servo para 180 graus (2400µs) e espera de 5s | **10%** |
| 3 | Ajuste do servo para 90 graus (1470µs) e espera de 5s | **10%** |
| 4 | Ajuste do servo para 0 graus (500µs) e espera de 5s | **10%** |
| 5 | Implementação do movimento suave entre 0 e 180 graus | **35%** |
| 6 | Teste prático com LED RGB na GPIO 12 utilizando a BitDogLab | **15%** |

## Experimento com o LED RGB (GPIO 12)
Após rodar o código na **BitDogLab**, observe o comportamento do **LED RGB** conectado na **GPIO 12**. Pergunta para reflexão:
- Como o LED reage enquanto o servo está se movimentando?

## Como Rodar o Projeto
1. Instale o **Pico SDK** e configure o ambiente para compilar código para o **RP2040**.
2. Compile e carregue o programa no **Raspberry Pi Pico W**.
3. Para simulação, use a plataforma **Wokwi**.

## Considerações Finais
Este projeto demonstra o uso do PWM para **controle preciso** de um servo motor, garantindo movimentação suave e periódica. Além disso, o experimento com o **LED RGB** permite avaliar como o **PWM** afeta outros componentes no mesmo circuito.
