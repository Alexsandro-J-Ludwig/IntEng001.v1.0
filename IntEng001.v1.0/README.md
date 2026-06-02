# 🤖 **ROBÔ SEGUIDOR DE LINHA**
Esse código é dedicado ao funcionamento de um robô seguidor de linha como parte do projeto de 
Introdução a Engenharia do curso de Engenharia Mecâtronica da Instituição federal de Santa Catarina.

## 📝 componentes necessários
- ESP32 (modelo usado no projeto: ESP32-S2)
- 2x motores DC 3-6v com caixa de redução de 39:1
- Ponte H L298N
- Placa de Extensão do ESP32-S2
- Modulo de sensor de Infravermelho de 5 canais 74hc14d

## 📌 Mapeamento de Pinos (Hardware vs Software)
Para que o código funcione perfeitamente com a Placa de Extensão do ESP32-S2, as conexões físicas devem seguir o esquema abaixo:

| Componente Ponte H | Função | Pino no ESP32-S2
| :--- | :--- | :--- |
| **ENA** | Controle de Velocidade - Motor Esquerdo | GPIO 6 |
