#include <Arduino.h>
#include "Structs.h"
#include "Mover.h"

// INSTÂNCIAS GLOBAIS
// Definindo motor A
Motor motor_a = {
    { { 25, 32, 0, 0 } },
    { { 4, 0 } },
    0,
    0,
};

// Definindo motor B
Motor motor_b = {
    { { 0, 0, 18, 17 } },
    { { 0, 16 } },
    0,
    0,
};

// Definindo sensores IR
IR_Sensor ir = {{26, 27, 14, 33, 13}};

Chassi_Direcao sentido = Chassi_Direcao::PARAR;

unsigned long lastMoveTime = 0;        // Variável para armazenar o tempo da última movimentação
const unsigned long moveInterval = 10; // Intervalo de movimentação em milissegundos

void setup()
{
  // Configuração automática de todos os pinos de sentido como SAÍDA
  pinMode(motor_a.driver.p.motorA_in1, OUTPUT);
  pinMode(motor_a.driver.p.motorA_in2, OUTPUT);
  pinMode(motor_b.driver.p.motorB_in1, OUTPUT);
  pinMode(motor_b.driver.p.motorB_in2, OUTPUT);

  // Configuração dos pinos de PWM como SAÍDA
  pinMode(motor_a.pwm.p.enA, OUTPUT);
  pinMode(motor_b.pwm.p.enB, OUTPUT);

  for (int i = 0; i < 5; i++)
  {
    pinMode(ir.channels[i], INPUT);
  }

  // Inicia a comunicação serial para monitoramento
  Serial.begin(9600);
}

void loop()
{
  unsigned long currentTime = millis();
  if (currentTime - lastMoveTime >= moveInterval)
  {
    lastMoveTime = currentTime;

    int irValues = {
        (digitalRead(ir.channels[0]) << 4) |
        (digitalRead(ir.channels[1]) << 3) |
        (digitalRead(ir.channels[2]) << 2) |
        (digitalRead(ir.channels[3]) << 1) |
        (digitalRead(ir.channels[4]) << 0)};

    switch (irValues)
    {
    case (0b00100):
    case (0b01100):
    case (0b00110):
      sentido = Chassi_Direcao::FRENTE;
      mover(200, 200, sentido, lastMoveTime);
      break;
    case (0b01000):
      sentido = Chassi_Direcao::ESQUERDA;
      mover(100, 200, sentido, lastMoveTime);
      break;
    case (0b00010):
      sentido = Chassi_Direcao::DIREITA;
      mover(200, 100, sentido, lastMoveTime);
      break;
    case (0b10000):
    case (0b11000): 
    case (0b11100):
      sentido = Chassi_Direcao::EXTRA_ESQUERDA;
      mover(50, 200, sentido, lastMoveTime);
      break;
    case (0b00001):
    case (0b00011):
    case (0b00111):
      sentido = Chassi_Direcao::EXTRA_DIREITA;
      mover(200, 50, sentido, lastMoveTime);
      break;
    default:
      sentido = Chassi_Direcao::PARAR;
      mover(0, 0, sentido, lastMoveTime);
      break;
    }
  }
}