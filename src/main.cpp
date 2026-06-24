  #include <Arduino.h>
  #include "Structs.h"
  #include "Mover.h"

  // INSTÂNCIAS GLOBAIS
  L298N_Motor driver[] = {
      {21, 19, 18, 17}};
    
  L298N_PWM pwm = {25, 16};

  IR_Sensor ir = {{26, 27, 14, 33, 13}};

  Chassi_Direcao sentido = Chassi_Direcao::PARAR;

  unsigned long lastMoveTime = 0; // Variável para armazenar o tempo da última movimentação
  const unsigned long moveInterval = 10; // Intervalo de movimentação em milissegundos

  void setup()
  {
    // Configuração automática de todos os pinos de sentido como SAÍDA
    for (int i = 0; i < 4; i++)
    {
      pinMode(driver[i].pins[i], OUTPUT);
    }

    // Configuração dos pinos de PWM como SAÍDA
    pinMode(pwm.enA, OUTPUT);
    pinMode(pwm.enB, OUTPUT);

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

      int irValues[5] = {
          digitalRead(ir.channels[0]),
          digitalRead(ir.channels[1]),
          digitalRead(ir.channels[2]),
          digitalRead(ir.channels[3]),
          digitalRead(ir.channels[4])};

      switch (irValues[0], irValues[1], irValues[2], irValues[3], irValues[4]) {
        case (LOW, LOW, HIGH, LOW, LOW):
          sentido = Chassi_Direcao::FRENTE;
            mover(200, 200, sentido);
            break;
          case (LOW, HIGH, LOW, LOW, LOW):
            sentido = Chassi_Direcao::ESQUERDA;
            mover(100, 200, sentido);
            break;
          case (LOW, LOW, LOW, HIGH, LOW):
            sentido = Chassi_Direcao::DIREITA;
            mover(200, 100, sentido);
            break;
          case (HIGH, LOW, LOW, LOW, LOW):
            sentido = Chassi_Direcao::EXTRA_ESQUERDA;
            mover(50, 200, sentido);
            break;
          case (LOW, LOW, LOW, LOW, HIGH):
            sentido = Chassi_Direcao::EXTRA_DIREITA;
            mover(200, 50, sentido);
            break;
          default:
            sentido = Chassi_Direcao::PARAR;
            mover(0, 0, sentido);
            break;
        }

  }