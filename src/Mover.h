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

unsigned long lastMoveTime = 0; // Variável para armazenar o tempo da última movimentação
long erroTA = 0; // Variável para armazenar o erro de posição
long erroTB = 0;
unsigned long msTP;


void mover(int enA, int enB, Chassi_Direcao sentido, unsigned long msT) {
    static float pwm_atual[2] = {0.0f, 0.0f};
    static float k = 0.25;
    static float kT = 0.01;

    // switch (sentido) {
    //     case FRENTE:
    //         digitalWrite(driver[0].motorA, HIGH);
    //         digitalWrite(driver[0].motorB, LOW);
    //         digitalWrite(driver[1].motorA, HIGH);
    //         digitalWrite(driver[1].motorB, LOW);
    //         break;
    //     case PARAR:
    //         digitalWrite(driver[0].motorA, LOW);
    //         digitalWrite(driver[0].motorB, LOW);
    //         digitalWrite(driver[1].motorA, LOW);
    //         digitalWrite(driver[1].motorB, LOW);
    //         break;
    //     case ESQUERDA:
    //         digitalWrite(driver[0].motorA, LOW);
    //         digitalWrite(driver[0].motorB, HIGH);
    //         digitalWrite(driver[1].motorA, HIGH);
    //         digitalWrite(driver[1].motorB, LOW);
    //         break;
    //     case DIREITA:
    //         digitalWrite(driver[0].motorA, HIGH);
    //         digitalWrite(driver[0].motorB, LOW);
    //         digitalWrite(driver[1].motorA, LOW);
    //         digitalWrite(driver[1].motorB, HIGH);
    //         break;
    //}

    unsigned long tempoAnterior = (msT - msTP);
    if(tempoAnterior == 0) tempoAnterior = 1; 
    msTP = msT;

    if (pwm_atual[0] != enA || pwm_atual[1] != enB){
        // Aplicar a derivada da diferença entre PWMs usando P como derivação de PWM
        // E aplicar para realizar o "freio preditivo"
        // pwm = f(x + h) - f(x) / h

        pwm_atual[0] = pwm_atual[0] + (enA - pwm_atual[0]) * k;
        pwm_atual[1] = pwm_atual[1] + (enB - pwm_atual[1]) * k;

        int pwmA_value = pwm_atual[0] + (erroTA * kT);
        int pwmB_value = pwm_atual[1] + (erroTB * kT);

        pwmA_value = constrain(abs(pwmA_value), 0, 255);
        pwmB_value = constrain(abs(pwmB_value), 0, 255);

        analogWrite(pwm.enA, abs(pwmA_value));
        analogWrite(pwm.enB, abs(pwmB_value));
        
        
        erroTA += (enA - pwm_atual[0]) * (tempoAnterior / 1000.0);
        erroTA = constrain(erroTA, -500, 500);
        erroTB += (enB - pwm_atual[1]) * (tempoAnterior / 1000.0);
        erroTB = constrain(erroTB, -500, 500);
    } else {
        analogWrite(pwm.enA, abs(enA));
        analogWrite(pwm.enB, abs(enB));
        erroTA = 0;
        erroTB = 0;
    }
}   

#endif