# Robô Seguidor de Linha

Esse código é dedicado ao funcionamento de um robô seguidor de linha como parte do projeto de Introdução à Engenharia do curso de Engenharia Mecatrônica da Instituição Federal de Santa Catarina.

---

## Componentes necessários

- ESP32 DevKit V1 (WROOM-32, 30 pinos)
- 2x motores DC 3-6V com caixa de redução 39:1
- Ponte H L298N
- Módulo sensor IR de 5 canais com TCRT5000

---

## Mapeamento de pinos

### Ponte H L298N → ESP32

| Pino L298N | Função | GPIO ESP32 |
| :--- | :--- | :--- |
| **ENA** | Velocidade — Motor Esquerdo (PWM) | GPIO 25 |
| **IN1** | Direção — Motor Esquerdo A | GPIO 21 |
| **IN2** | Direção — Motor Esquerdo B | GPIO 19 |
| **ENB** | Velocidade — Motor Direito (PWM) | GPIO 16 |
| **IN3** | Direção — Motor Direito A | GPIO 18 |
| **IN4** | Direção — Motor Direito B | GPIO 17 |
| **GND** | Terra | GND |

### Sensor IR 5 canais → ESP32

| Canal | Função | GPIO ESP32 |
| :--- | :--- | :--- |
| **Channel 1** | Sensor extremo esquerdo | GPIO 13 |
| **Channel 2** | Sensor esquerdo | GPIO 33 |
| **Channel 3** | Sensor central | GPIO 14 |
| **Channel 4** | Sensor direito | GPIO 27 |
| **Channel 5** | Sensor extremo direito | GPIO 26 |
| **GND** | Terra | GND |
| **VCC** | Alimentação | 5V |

> ⚠️ O TCRT5000 opera em 5V. A saída do sensor entrega até 5V — use um divisor de tensão antes de conectar ao ESP32, que suporta no máximo 3.3V nas entradas GPIO.

---

## Lógica de seguimento de linha

O robô segue uma linha **preta** sobre fundo **branco**. O sensor retorna `HIGH` quando detecta a linha.

A prioridade de decisão é:

| Prioridade | Condição | Ação |
| :--- | :--- | :--- |
| 1ª | CH1 + CH2 ativos | Curva forte para a esquerda |
| 2ª | CH4 + CH5 ativos | Curva forte para a direita |
| 3ª | CH1 ativo sozinho | Curva forte para a esquerda |
| 4ª | CH5 ativo sozinho | Curva forte para a direita |
| 5ª | CH2 ativo | Correção suave para a esquerda |
| 6ª | CH4 ativo | Correção suave para a direita |
| 7ª | CH3 ativo | Linha centralizada — seguir em frente |
| 8ª | Nenhum ativo | Parar |

---

## Estrutura do projeto

```
/
├── main.cpp       — loop principal e leitura dos sensores
├── Mover.h        — função mover() e enum Direcao
└── Structs.h      — estruturas L298N_Motor, L298N_PWM e IR_Sensor
```
