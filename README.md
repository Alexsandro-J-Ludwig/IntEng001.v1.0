# Robô Seguidor de Linha
Esse código é dedicado ao funcionamento de um robô seguidor de linha como parte do projeto de Introdução à Engenharia do curso de Engenharia Mecatrônica da Instituição Federal de Santa Catarina.

---

## Componentes necessários
- ESP32 DevKit V1 (WROOM-32, 30 pinos)
- 2x motores TT DC com caixa de redução
- 2x Baterias 18650
- Roda boba
- Ponte H L298N
- Módulo sensor IR de 5 canais com TCRT5000
- Buzzer passivo
- 2x LEDs indicadores de direção

---

## Mapeamento de pinos
### Ponte H L298N → ESP32
| Pino L298N | Função | GPIO ESP32 |
| :--- | :--- | :--- |
| **ENA** | Velocidade — Motor A (PWM) | GPIO 4 |
| **IN1** | Direção — Motor A | GPIO 25 |
| **IN2** | Direção — Motor A | GPIO 32 |
| **ENB** | Velocidade — Motor B (PWM) | GPIO 16 |
| **IN3** | Direção — Motor B | GPIO 18 |
| **IN4** | Direção — Motor B | GPIO 17 |
| **GND** | Terra | GND |

### Sensor IR 5 canais → ESP32
| Canal | Função | GPIO ESP32 |
| :--- | :--- | :--- |
| **Channel 1** | Sensor extremo esquerdo | GPIO 26 |
| **Channel 2** | Sensor esquerdo | GPIO 27 |
| **Channel 3** | Sensor central | GPIO 14 |
| **Channel 4** | Sensor direito | GPIO 33 |
| **Channel 5** | Sensor extremo direito | GPIO 13 |
| **GND** | Terra | GND |
| **VCC** | Alimentação | 5V |

> ⚠️ O TCRT5000 opera em 5V. A saída do sensor entrega até 5V — use um divisor de tensão antes de conectar ao ESP32, que suporta no máximo 3.3V nas entradas GPIO.

### Periféricos
| Componente | GPIO ESP32 |
| :--- | :--- |
| **Buzzer passivo** | GPIO 21 |
| **LED esquerdo** | GPIO 23 |
| **LED direito** | GPIO 22 |
| **Botão de start** | GPIO 0 (BOOT) |

---

## Lógica de seguimento de linha
O robô segue uma linha **preta** sobre fundo **branco**. O sensor retorna `HIGH` quando detecta a linha.

A posição do robô é calculada pela distância média entre os canais ativos do TCRT5000, gerando um valor de desvio contínuo:

| Desvio | Significado |
| :--- | :--- |
| `-3.0` | Extremo esquerdo |
| `0.0` | Centralizado |
| `+3.0` | Extremo direito |

| `irValues` | Desvio |
| :--- | :--- |
| `0b11011` | 0.0 — centralizado |
| `0b10011` | -0.75 |
| `0b11001` | +0.75 |
| `0b10111` | -1.5 |
| `0b11101` | +1.5 |
| `0b00111` | -2.25 |
| `0b11100` | +2.25 |
| `0b01111` | -3.0 |
| `0b11110` | +3.0 |
| `0b00000` | Linha de chegada — freio dinâmico |

---

## Controle PID
O ajuste de velocidade dos motores é feito por um controlador PID com base no desvio de posição:

| Termo | Ganho | Função |
| :--- | :--- | :--- |
| **kP** | 25.0 | Corrige proporcionalmente ao desvio atual |
| **kI** | 0.3 | Corrige offset persistente acumulado |
| **kD** | 10.0 | Antecipa variação do desvio |
| **Base** | 150 | Velocidade base dos motores (0–255) |

---

## Inicialização
1. Ao ligar, o robô emite um **bipe longo** indicando fim do setup
2. Aguarda o pressionamento do **botão BOOT** (GPIO 0)
3. Três **bipes curtos** confirmam o início — o robô começa a seguir a linha

---

## Estrutura do projeto
```
/
├── main.cpp       — loop principal, leitura dos sensores e máquina de estados
├── Mover.h        — funções mover() e setSpeed() com controle PID
└── Structs.h      — estruturas Motor, IR_Sensor, Buzzer e Led
```
