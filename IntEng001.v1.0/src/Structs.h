/*
==================================================================
IntEng001 - Controle de Motores com L298N e ESP32
==================================================================

Definição da estrutura de dados para controle dos motores utilizando o driver L298N, e a configuração dos pinos físicos conectados ao ESP32.
- motores - Estrutura para armazenar os pinos de controle dos motores (IN1, IN2, IN3, IN4).
    - motorA - Pinos de controle do Motor A (Motor Esquerdo).
    - motorB - Pinos de controle do Motor B (Motor Direito).

- pwm - Estrutura para armazenar os pinos de controle de velocidade (ENA, ENB).

- sensor IR - Estrutura para armazenar os pinos dos sensores infravermelhos (IR1).

Exemplo de declaração e uso das estruturas:
L298N_Motor driver[] = {
    {2, 3}, // exemplo Motor Esquerdo: IN1 = 2, IN2 = 3
    {4, 5}  // exemplo Motor Direito: IN3 = 4, IN4 = 5
};

L298N_PWM pwm = {6, 7}; // ENA = 6, ENB = 7

IR_Sensor ir[] = {
  {8, 9, 10, 11, 12}
};

*/

#ifndef STRUCTS_H
#define STRUCTS_H

struct L298N_Motor
{
    int motorA; // Pino de controle do Motor A (IN1 e IN2)
    int motorB; // Pino de controle do Motor B (IN3 e IN4)
};

struct L298N_PWM
{
    int enA;
    int enB;
};

struct IR_Sensor
{
    int in1; // Receberá o 8
    int in2; // Receberá o 9
    int in3; // Receberá o 10
    int in4; // Receberá o 11
    int in5; // Receberá o 12
};

#endif // STRUCTS_H