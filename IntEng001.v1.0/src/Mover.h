/* 
==============================================
Mover - Função para controlar os motores do robô utilizando o driver L298N.
==============================================

Definição da função mover() para controlar a direção e velocidade dos motores do robô com base nos sinais dos sensores IR.
- mover() - Função que recebe os valores de velocidade para os motores esquerdo e direito, e a direção do movimento (frente, trás, esquerda, direita).
    - enA - Velocidade do motor esquerdo (ENA).
    - enB - Velocidade do motor direito (ENB).
    - sentido - Direção do movimento (FRENTE, TRAS, ESQUERDA, DIREITA).

Exemplo de uso da função mover() no loop principal:
if (irValues[2] == HIGH) // Sensor principal detecta linha mantendo o robô na linha
{
    mover(200, 200, FRENTE);
} else if (irValues[1] == HIGH && irValues[2] == LOW) // Sensor esquerdo detecta linha, girar para a esquerda
{
    mover(100, 200, ESQUERDA);
} 
else if (irValues[3] == HIGH && irValues[2] == LOW) // Sensor direito detecta linha, girar para a direita
{
    mover(200, 100, DIREITA);
} 
else if (irValues[0] == HIGH) // Sensor mais à esquerda detecta linha, girar mais para a esquerda
{
    mover(50, 200, ESQUERDA);
} 
else if (irValues[4] == HIGH) // Sensor mais à direita detecta linha, girar mais para a direita
{
    mover(200, 50, DIREITA);
}
*/

#ifndef MOVIMENTACAO_H
#define MOVIMENTACAO_H

#include <Arduino.h>
#include "Structs.h"

enum Direcao {
    FRENTE,
    PARAR,
    ESQUERDA,
    DIREITA
};

extern L298N_Motor driver[];
extern L298N_PWM pwm;

void mover(int enA, int enB, Direcao sentido) {
    switch (sentido) {
        case FRENTE:
            digitalWrite(driver[0].motorA, HIGH);
            digitalWrite(driver[0].motorB, LOW);
            digitalWrite(driver[1].motorA, HIGH);
            digitalWrite(driver[1].motorB, LOW);
            break;
        case PARAR:
            digitalWrite(driver[0].motorA, LOW);
            digitalWrite(driver[0].motorB, LOW);
            digitalWrite(driver[1].motorA, LOW);
            digitalWrite(driver[1].motorB, LOW);
            break;
        case ESQUERDA:
            digitalWrite(driver[0].motorA, LOW);
            digitalWrite(driver[0].motorB, HIGH);
            digitalWrite(driver[1].motorA, HIGH);
            digitalWrite(driver[1].motorB, LOW);
            break;
        case DIREITA:
            digitalWrite(driver[0].motorA, HIGH);
            digitalWrite(driver[0].motorB, LOW);
            digitalWrite(driver[1].motorA, LOW);
            digitalWrite(driver[1].motorB, HIGH);
            break;
    }

    analogWrite(pwm.enA, enA); // Velocidade motor esquerdo
    analogWrite(pwm.enB, enB); // Velocidade motor direito
}

#endif