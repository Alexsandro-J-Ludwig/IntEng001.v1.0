#ifndef STRUCTS_H
#define STRUCTS_H

enum Chassi_Direcao {
    FRENTE,
    PARAR,
    ESQUERDA,
    DIREITA,
    EXTRA_ESQUERDA,
    EXTRA_DIREITA,
    CURVE_ESQUERDA,
    CURVE_DIREITA,
    FORA_LINHA
};

union L298N_Motor
{
    struct {
        int motorA_in1; // Pino de controle do Motor A (IN1 e IN2)
        int motorA_in2; // Pino de controle do Motor A (IN1 e IN2)
        int motorB_in1; // Pino de controle do Motor B (IN3 e IN4)
        int motorB_in2; // Pino de controle do Motor B (IN3 e IN4)
    } p;
    int pins[4];
};

union L298N_PWM
{
    struct {
        int enA; // Pino de controle de velocidade do Motor A (ENA)
        int enB; // Pino de controle de velocidade do Motor B (ENB)
    } p;
    int pins[2];
};

struct Motor {
    L298N_Motor driver; // Estrutura para armazenar os pinos de controle dos motores
    L298N_PWM pwm;      // Estrutura para armazenar os pinos de controle
    int pwm_atual;
    long erroT;
};

union IR_Sensor {
    struct {
        int ir1; // Pino do sensor IR 1
        int ir2; // Pino do sensor IR 2
        int ir3; // Pino do sensor IR 3
        int ir4; // Pino do sensor IR 4
        int ir5; // Pino do sensor IR 5
    } s;
    int channels[5];
};

#endif // STRUCTS_H