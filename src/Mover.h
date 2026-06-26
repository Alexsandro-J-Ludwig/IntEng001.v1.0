#ifndef MOVIMENTACAO_H
#define MOVIMENTACAO_H

#include <Arduino.h>
#include "Structs.h"

/*
enum Direcao
{
    FRENTE,
    PARAR,
    ESQUERDA,
    DIREITA
};
*/

extern Motor motor_a;
extern Motor motor_b;

long msTP = 0;

/* 
// Controle de velocidade baseado em suavização + integral de erro
// - Suavização: aproximação gradual do PWM em direção ao alvo (filtro de 1ª ordem)
// - Integral: acumulação do erro ao longo do tempo para correção de offset
// - Anti-windup: limitação e reset da integral quando próximo do alvo
*/
void setSpeed(
    unsigned long msT,
    Motor &motor_a,
    Motor &motor_b,
    int pwm_alvo_a,
    int pwm_alvo_b)
{   
    // COEFICIENTE DE SUAVIZAÇÃO E CORREÇÃO DE ERRO ACUMULADO
    static float k = 0.40;
    static float kT = 0.001;

    // CALCULO DO TEMPO ANTERIOR
    unsigned long tempoAnterior = (msT - msTP);
    if (tempoAnterior == 0)
        tempoAnterior = 1;
    msTP = msT;

    if (motor_a.pwm.p.enA != pwm_alvo_a)
    {
        // CONTROLE DE VELOCIDADE E FRENAGEM BASEADO EM SUAVIZAÇÃO
        motor_a.pwm_atual = motor_a.pwm_atual + (pwm_alvo_a - motor_a.pwm_atual) * k;

        // CORREÇÃO POR ERRO ACUMULADO
        int pwmA_value = motor_a.pwm_atual+ (motor_a.erroT * kT);

        // LIMITE E ANTI-WINDUP
        pwmA_value = constrain(pwmA_value, 0, 255);

        // ESCREVE NO PWM
        analogWrite(motor_a.pwm.p.enA, pwmA_value);

        // ATUALIZA CORREÇÃO POR ERRO ACUMULADO
        motor_a.erroT += (pwm_alvo_a - motor_a.pwm_atual) * (tempoAnterior / 1000.0);
        motor_a.erroT = constrain(motor_a.erroT, -500, 500);
    }
    else
    {
        // ESCREVE NO PWM
        analogWrite(motor_a.pwm.p.enA, abs((int)motor_a.pwm_atual));
        // LIMPA O ERRO ACUMULADO
        motor_a.erroT = 0;
    }

    if (motor_b.pwm.p.enB != pwm_alvo_b)
    {
        // CONTROLE DE VELOCIDADE E FRENAGEM BASEADO EM SUAVIZAÇÃO
        motor_b.pwm_atual = motor_b.pwm_atual + (pwm_alvo_b - motor_b.pwm_atual) * k;

        // CORREÇÃO POR ERRO ACUMULADO
        int pwmB_value = motor_b.pwm_atual + (motor_b.erroT * kT);

        // LIMITE E ANTI-WINDUP
        pwmB_value = constrain(pwmB_value, 0, 255);

        // ESCREVE NO PWM
        analogWrite(motor_b.pwm.p.enB, pwmB_value);

        // ATUALIZA CORREÇÃO POR ERRO ACUMULADO
        motor_b.erroT += (pwm_alvo_b - motor_b.pwm_atual) * (tempoAnterior / 1000.0);
        motor_b.erroT = constrain(motor_b.erroT, -500, 500);
    }
    else
    {
        // ESCREVE NO PWM
        analogWrite(motor_b.pwm.p.enB, abs((int)motor_b.pwm_atual));
        motor_b.erroT = 0;
    }
}

extern Chassi_Direcao ultimaDirecao;

void mover(Chassi_Direcao sentido, unsigned long msT)
{
    switch (sentido)
    {
    case Chassi_Direcao::FRENTE:
        digitalWrite(motor_a.driver.p.motorA_in1, HIGH);
        digitalWrite(motor_a.driver.p.motorA_in2, LOW);
        digitalWrite(motor_b.driver.p.motorB_in1, HIGH);
        digitalWrite(motor_b.driver.p.motorB_in2, LOW);
        setSpeed(msT, motor_a, motor_b, 255, 255);
        break;
    case Chassi_Direcao::DIREITA:
        // Smooth turn right: both wheels forward, left wheel (B) runs faster
        digitalWrite(motor_a.driver.p.motorA_in1, HIGH);
        digitalWrite(motor_a.driver.p.motorA_in2, LOW);
        digitalWrite(motor_b.driver.p.motorB_in1, HIGH);
        digitalWrite(motor_b.driver.p.motorB_in2, LOW);
        setSpeed(msT, motor_a, motor_b, 100, 200);
        break;
    case Chassi_Direcao::ESQUERDA:
        // Smooth turn left: both wheels forward, right wheel (A) runs faster
        digitalWrite(motor_a.driver.p.motorA_in1, HIGH);
        digitalWrite(motor_a.driver.p.motorA_in2, LOW);
        digitalWrite(motor_b.driver.p.motorB_in1, HIGH);
        digitalWrite(motor_b.driver.p.motorB_in2, LOW);
        setSpeed(msT, motor_a, motor_b, 200, 100);
        break;
    case Chassi_Direcao::EXTRA_ESQUERDA:
        digitalWrite(motor_a.driver.p.motorA_in1, HIGH);
        digitalWrite(motor_a.driver.p.motorA_in2, LOW);
        digitalWrite(motor_b.driver.p.motorB_in1, LOW);
        digitalWrite(motor_b.driver.p.motorB_in2, HIGH);
        setSpeed(msT, motor_a, motor_b, 200, 100);
        break;
    case Chassi_Direcao::EXTRA_DIREITA:
        digitalWrite(motor_a.driver.p.motorA_in1, LOW);
        digitalWrite(motor_a.driver.p.motorA_in2, HIGH);
        digitalWrite(motor_b.driver.p.motorB_in1, HIGH);
        digitalWrite(motor_b.driver.p.motorB_in2, LOW);
        setSpeed(msT, motor_a, motor_b, 100, 200);
        break;
    case Chassi_Direcao::CURVE_ESQUERDA:
        digitalWrite(motor_a.driver.p.motorA_in1, HIGH);
        digitalWrite(motor_a.driver.p.motorA_in2, LOW);
        digitalWrite(motor_b.driver.p.motorB_in1, LOW);
        digitalWrite(motor_b.driver.p.motorB_in2, HIGH);
        setSpeed(msT, motor_a, motor_b, 150, 150);
        break;
    case Chassi_Direcao::CURVE_DIREITA:
        digitalWrite(motor_a.driver.p.motorA_in1, LOW);
        digitalWrite(motor_a.driver.p.motorA_in2, HIGH);
        digitalWrite(motor_b.driver.p.motorB_in1, HIGH);
        digitalWrite(motor_b.driver.p.motorB_in2, LOW);
        setSpeed(msT, motor_a, motor_b, 150, 150);
        break;
    case Chassi_Direcao::FORA_LINHA:
        if (ultimaDirecao == Chassi_Direcao::EXTRA_ESQUERDA || 
            ultimaDirecao == Chassi_Direcao::CURVE_ESQUERDA || 
            ultimaDirecao == Chassi_Direcao::ESQUERDA) {
            // Last turn was left -> Continue turning left (A forward, B backward)
            digitalWrite(motor_a.driver.p.motorA_in1, HIGH);
            digitalWrite(motor_a.driver.p.motorA_in2, LOW);
            digitalWrite(motor_b.driver.p.motorB_in1, LOW);
            digitalWrite(motor_b.driver.p.motorB_in2, HIGH);
            setSpeed(msT, motor_a, motor_b, 150, 100);
        } else if (ultimaDirecao == Chassi_Direcao::EXTRA_DIREITA || 
                   ultimaDirecao == Chassi_Direcao::CURVE_DIREITA || 
                   ultimaDirecao == Chassi_Direcao::DIREITA) {
            // Last turn was right -> Continue turning right (A backward, B forward)
            digitalWrite(motor_a.driver.p.motorA_in1, LOW);
            digitalWrite(motor_a.driver.p.motorA_in2, HIGH);
            digitalWrite(motor_b.driver.p.motorB_in1, HIGH);
            digitalWrite(motor_b.driver.p.motorB_in2, LOW);
            setSpeed(msT, motor_a, motor_b, 100, 150);
        }
        break;
    default:
        digitalWrite(motor_a.driver.p.motorA_in1, LOW);
        digitalWrite(motor_a.driver.p.motorA_in2, LOW);
        digitalWrite(motor_b.driver.p.motorB_in1, LOW);
        digitalWrite(motor_b.driver.p.motorB_in2, LOW);
        setSpeed(msT, motor_a, motor_b, 0, 0);
        break;
    }
}

#endif