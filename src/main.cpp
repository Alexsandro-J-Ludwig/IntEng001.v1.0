  /*
  ===================================================================
  DOCUMENTAÇÃO DE USO DO COMPONENTE PONTE H L298N
  ===================================================================

  Definição dos pinos físicos conectados ao ESP32 para controle dos motores e do PWM:
  - Driver 0 (Motor Esquerdo): Pino 2 (motorA[0]) e Pino 3 (motorA[1])
  - Driver 1 (Motor Direito) : Pino 4 (motorA[2]) e Pino 5 (motorB[0])
  - PWM (Velocidade) : Pino 25 (ENA) e Pino 16 (ENB)
  - Sensor IR        : Pinos 13, 33, 14, 27, 26

  -------------------------------------------------------------------
  TABELA DE MOVIMENTAÇÃO (Exemplos Práticos para o loop):
  -------------------------------------------------------------------

  1. MOVER PARA FRENTE:
    digitalWrite(driver[0].motorA, HIGH);
    digitalWrite(driver[0].motorB, LOW);
    digitalWrite(driver[1].motorA, HIGH);
    digitalWrite(driver[1].motorB, LOW);
    analogWrite(pwm.enA, 200); // Velocidade motor esquerdo
    analogWrite(pwm.enB, 200); // Velocidade motor direito

  2. MOVER PARA TRÁS:
    digitalWrite(driver[0].motorA, LOW);
    digitalWrite(driver[0].motorB, HIGH);
    digitalWrite(driver[1].motorA, LOW);
    digitalWrite(driver[1].motorB, HIGH);

  3. GIRAR PARA DIREITA:
    digitalWrite(driver[0].motorA, HIGH);
    digitalWrite(driver[0].motorB, LOW);
    digitalWrite(driver[1].motorA, LOW);
    digitalWrite(driver[1].motorB, HIGH);

  4. GIRAR PARA ESQUERDA:
    digitalWrite(driver[0].motorA, LOW);
    digitalWrite(driver[0].motorB, HIGH);
    digitalWrite(driver[1].motorA, HIGH);
    digitalWrite(driver[1].motorB, LOW);
  */

  #include <Arduino.h>
  #include "Structs.h"
  #include "Mover.h"

  // INSTÂNCIAS GLOBAIS
  L298N_Motor driver[] = {
      {21, 19},
      {18, 17}};
    
  L298N_PWM pwm = {25, 16};

  IR_Sensor ir = {{13, 33, 14, 27, 26}};

  void setup()
  {
    // Configuração automática de todos os pinos de sentido como SAÍDA
    for (int i = 0; i < 2; i++)
    {
      pinMode(driver[i].motorA, OUTPUT);
      pinMode(driver[i].motorB, OUTPUT);
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
    int irValues[5] = {
        digitalRead(ir.channels[0]),
        digitalRead(ir.channels[1]),
        digitalRead(ir.channels[2]),
        digitalRead(ir.channels[3]),
        digitalRead(ir.channels[4])};

  if (irValues[0] == HIGH && irValues[1] == HIGH) 
    { mover(50,  200, ESQUERDA); 
  }
  else if (irValues[3] == HIGH && irValues[4] == HIGH) 
    { mover(200, 50,  DIREITA);  
  }
  else if (irValues[0] == HIGH) 
    { mover(50,  200, ESQUERDA); 
  }  
  else if (irValues[4] == HIGH) 
    { mover(200, 50,  DIREITA);  
  } 
  else if (irValues[1] == HIGH) 
    { mover(100, 200, ESQUERDA); 
    }
  else if (irValues[3] == HIGH) 
    { mover(200, 100, DIREITA);  
  }
  else if (irValues[2] == HIGH) 
    { mover(200, 200, FRENTE);   
  }
  else { 
    mover(0,   0,   PARAR);    
  }
  }