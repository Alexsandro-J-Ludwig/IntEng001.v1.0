/*
==================================================================
IntEng001 - Controle de Motores com L298N e ESP32
==================================================================

Definição da estrutura de dados para controle dos motores utilizando o driver L298N, e a configuração dos pinos físicos conectados ao ESP32.
- motores - Estrutura para armazenar os pinos de controle dos motores (IN1, IN2, IN3, IN4).
    - motorA - Pinos de controle do Motor A (Motor Esquerdo).
    - motorB - Pinos de controle do Motor B (Motor Direito).

- pwm - Estrutura para armazenar os pinos de controlfeat: PWM controller with predictive brake and fix logic bugs and pinse de velocidade (ENA, ENB).

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

enum Chassi_Direcao {
    FRENTE,
    PARAR,
    ESQUERDA,
    DIREITA,
    EXTRA_ESQUERDA,
    EXTRA_DIREITA
};

union L298N_Motor
{
    struct {
        int motorA_in1; // Pino de controle do Motor A (IN1 e IN2)
        int motorA_in2; // Pino de controle do Motor A (IN1 e IN2)
        int motorB_in1; // Pino de controle do Motor B (IN3 e IN4)
        int motorB_in2; // Pino de controle do Motor B (IN3 e IN4)
    };
    int pins[4];
};

union L298N_PWM
{
    struct {
        int enA; // Pino de controle de velocidade do Motor A (ENA)
        int enB; // Pino de controle de velocidade do Motor B (ENB)
    };
    int pins[2];
};

union IR_Sensor {
    struct {
        int ir1; // Pino do sensor IR 1
        int ir2; // Pino do sensor IR 2
        int ir3; // Pino do sensor IR 3
        int ir4; // Pino do sensor IR 4
        int ir5; // Pino do sensor IR 5
    };
    int channels[5];
};

#endif // STRUCTS_H