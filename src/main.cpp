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
Chassi_Direcao ultimaDirecao = Chassi_Direcao::PARAR;

bool curva_direita = false;
bool curva_esquerda = false;
bool recuperacao = false;

unsigned long lastMoveTime = 0;        // Variável para armazenar o tempo da última movimentação
const unsigned long moveInterval = 10; // Intervalo de movimentação em milissegundos
unsigned long fora_linha_tempo = 0;

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

    if (recuperacao) {
      if (irValues != 0b11111) {
        recuperacao = false;
      } else {
        irValues = 0b11111;
      }
    }

    if (irValues == 0b11011) {
      if (curva_esquerda)
        irValues = 0b00011;
      else if (curva_direita)
        irValues = 0b11000;
    } else {
      curva_esquerda = false;
      curva_direita = false;
    }

    switch (irValues)
    {
    case (0b11011):
    case (0b10011):
    case (0b11001):
      sentido = Chassi_Direcao::FRENTE;
      mover(sentido, lastMoveTime);
      break;
    case (0b10111):
      sentido = Chassi_Direcao::ESQUERDA;
      mover(sentido, lastMoveTime);
      break;
    case (0b11101):
      sentido = Chassi_Direcao::DIREITA;
      mover(sentido, lastMoveTime);
      break;
    case (0b01111):
    case (0b00111): 
    case (0b00011):
    case (0b00001):
      sentido = Chassi_Direcao::EXTRA_ESQUERDA;
      curva_esquerda = true;
      curva_direita = false;
      mover(sentido, lastMoveTime);
      break;
    case (0b11110):
    case (0b11100):
    case (0b11000):
    case (0b10000):
      sentido = Chassi_Direcao::EXTRA_DIREITA;
      curva_direita = true;
      curva_esquerda = false;
      mover(sentido, lastMoveTime);
      break;
    case (0b00000):
      sentido = Chassi_Direcao::PARAR;
      mover(sentido, lastMoveTime);
      break;
    case (0b11111):
      ultimaDirecao = sentido;
      sentido = Chassi_Direcao::FORA_LINHA;
      recuperacao = true;

      mover(sentido, lastMoveTime);

      break;
    }
  }
}