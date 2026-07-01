#ifndef MOVIMENTACAO_H
#define MOVIMENTACAO_H

#include <Arduino.h>
#include "Structs.h"

extern Motor motor_a;
extern Motor motor_b;

float distAntiga;
unsigned long msTP;

void setSpeed(
    Motor &motor_a,
    Motor &motor_b,
    unsigned long msT,
    float distDetect)
{
    // PID - ajuste na pista
    const float kP = 25.0;
    const float kI = 0.3;
    const float kD = 10.0;
    const int VEL_BASE = 150;

    static bool primeiraExecucao = true;
    if (primeiraExecucao)
    {
        msTP = msT;
        primeiraExecucao = false;
    }

    unsigned long tempoAnterior = (msT - msTP);
    if (tempoAnterior == 0)
        tempoAnterior = 1;
    msTP = msT;

    float erroD = distDetect;
    float derivada = (distDetect - distAntiga) / (tempoAnterior / 1000.0);

    static float integralErro = 0;

    if (abs(erroD) > 0.1)
    {
        integralErro = constrain(integralErro + erroD * (tempoAnterior / 1000.0), -50, 50);

        float correcao = erroD * kP + derivada * kD + integralErro * kI;

        int pwm_a = constrain(VEL_BASE - correcao, 0, 255);
        int pwm_b = constrain(VEL_BASE + correcao, 0, 255);

        motor_a.pwm_atual = pwm_a;
        motor_b.pwm_atual = pwm_b;

        analogWrite(motor_a.pwm.p.enA, pwm_a);
        analogWrite(motor_b.pwm.p.enB, pwm_b);
    }
    else
    {
        integralErro = 0;
        analogWrite(motor_a.pwm.p.enA, VEL_BASE);
        analogWrite(motor_b.pwm.p.enB, VEL_BASE);
    }

    // Histórico para o termo derivativo
    distAntiga = distDetect;
}

void mover(float distCenter, unsigned long times)
{
    digitalWrite(motor_a.driver.p.motorA_in1, HIGH);
    digitalWrite(motor_a.driver.p.motorA_in2, LOW);
    digitalWrite(motor_b.driver.p.motorB_in1, HIGH);
    digitalWrite(motor_b.driver.p.motorB_in2, LOW);

    setSpeed(motor_a, motor_b, times, distCenter);
}

#endif