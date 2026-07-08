#include <Arduino.h>
#include "Structs.h"
#include "Mover.h"
extern float distAntiga;

// INSTÂNCIAS GLOBAIS
Motor motor_a = {
    {{25, 32, 0, 0}},
    {{4, 0}},
    0};

Motor motor_b = {
    {{0, 0, 18, 17}},
    {{0, 16}},
    0};

IR_Sensor ir = {{26, 27, 14, 33, 13}};

Buzzer buzzer = {{21, 0}};

Led led = {{23, 22}};

const int BOTAO_START = 0;

unsigned long lastMoveTime = 0;
const unsigned long moveInterval = 5;

const int BUZZER_BOOT    = 1000;
const int BUZZER_CHEGADA = 2000;
const int BUZZER_PERDIDO =  500;

bool bipouPerdido = false;
bool bipouChegada = false;

void setup()
{
  pinMode(BOTAO_START, INPUT_PULLUP);

  pinMode(motor_a.driver.p.motorA_in1, OUTPUT);
  pinMode(motor_a.driver.p.motorA_in2, OUTPUT);
  pinMode(motor_b.driver.p.motorB_in1, OUTPUT);
  pinMode(motor_b.driver.p.motorB_in2, OUTPUT);

  pinMode(motor_a.pwm.p.enA, OUTPUT);
  pinMode(motor_b.pwm.p.enB, OUTPUT);

  for (int i = 0; i < 5; i++)
  {
    pinMode(ir.channels[i], INPUT);
  }

  pinMode(buzzer.b.pin, OUTPUT);
  pinMode(led.l.l_direita, OUTPUT);
  pinMode(led.l.l_esquerda, OUTPUT);

  Serial.begin(9600);

  // Bipe de boot
  tone(buzzer.out[0], BUZZER_BOOT, 2000);
  delay(2000);

  // Aguarda botão
  while (digitalRead(BOTAO_START) == HIGH) {}

  // Três bipes de largada
  for (int i = 0; i < 3; i++) {
    tone(buzzer.out[0], BUZZER_BOOT, 200);
    delay(400);
  }
}

void loop()
{
  unsigned long currentTime = millis();
  if (currentTime - lastMoveTime >= moveInterval)
  {
    lastMoveTime = currentTime;

    // LEDs proporcionais ao desvio
    int brilho = (int)(abs(distAntiga) / 3.0 * 255);

    if (distAntiga < -0.1)
    {
      analogWrite(led.l.l_esquerda, brilho);
      analogWrite(led.l.l_direita, 0);
    }
    else if (distAntiga > 0.1)
    {
      analogWrite(led.l.l_esquerda, 0);
      analogWrite(led.l.l_direita, brilho);
    }
    else
    {
      analogWrite(led.l.l_esquerda, 0);
      analogWrite(led.l.l_direita, 0);
    }

    int irValues =
        (digitalRead(ir.channels[0]) << 4) |
        (digitalRead(ir.channels[1]) << 3) |
        (digitalRead(ir.channels[2]) << 2) |
        (digitalRead(ir.channels[3]) << 1) |
        (digitalRead(ir.channels[4]) << 0);

    switch (irValues)
    {
    case (0b11011):
      mover(0.0, lastMoveTime);
      bipouPerdido = false;
      break;
    case (0b10011):
      mover(-0.50, lastMoveTime);
      bipouPerdido = false;
      break;
    case (0b11001):
      mover(0.50, lastMoveTime);
      bipouPerdido = false;
      break;
    case (0b10111):
      mover(-1.5, lastMoveTime);
      bipouPerdido = false;
      break;
    case (0b11101):
      mover(1.5, lastMoveTime);
      bipouPerdido = false;
      break;
    case (0b00111):
      mover(-2.25, lastMoveTime);
      bipouPerdido = false;
      break;
    case (0b11100):
      mover(2.25, lastMoveTime);
      bipouPerdido = false;
      break;
    case (0b01111):
      mover(-3.00, lastMoveTime);
      bipouPerdido = false;
      break;
    case (0b11110):
      mover(3.0, lastMoveTime);
      bipouPerdido = false;
      break;
    case (0b00000): // Linha de chegada: Freio Dinâmico Ativo
      digitalWrite(motor_a.driver.p.motorA_in1, LOW);
      digitalWrite(motor_a.driver.p.motorA_in2, LOW);
      digitalWrite(motor_b.driver.p.motorB_in1, LOW);
      digitalWrite(motor_b.driver.p.motorB_in2, LOW);
      analogWrite(motor_a.pwm.p.enA, 0);
      analogWrite(motor_b.pwm.p.enB, 0);

      if (!bipouChegada)
      {
        tone(buzzer.out[0], BUZZER_CHEGADA, 2000);
        bipouChegada = true;
      }
      break;
    default:
      // Perdeu a linha — repete última correção conhecida
      mover(distAntiga, lastMoveTime);
      if (!bipouPerdido)
      {
        tone(buzzer.out[0], BUZZER_PERDIDO, 200);
        bipouPerdido = true;
      }
      break;
    }
  }
}