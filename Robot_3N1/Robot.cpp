// 1 seguidor de linea
// 2 evasor de obstaculos
// s swsdxc Amodo bluetooth
#include "Robot.h"
#include <Arduino.h> //Permite utilizar los comandos de Arduino
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
void Sensor::Inicializar()
{
  pinMode(Pin_sensor, OUTPUT);
}
int Sensor::Leer_sensor()
{
  int Estado;
  Estado = digitalRead(Pin_sensor);
  return Estado;
}
void Motor::Inicializar_Motor()
{
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(Pwm, OUTPUT);
}
void Motor::Atras(int Velocidad)
{
  analogWrite(Pwm, Velocidad);
  digitalWrite(A, HIGH);
  digitalWrite(B, LOW);
}
void Motor::Adelante(int Velocidad)
{
  analogWrite(Pwm, Velocidad);
  digitalWrite(A, LOW);
  digitalWrite(B, HIGH);
}
void Motor::Stop()
{
  analogWrite(0, Pwm);
  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
}
int parseD1Value(String json)
{
  if (json == 's')
  {
    return 'w';
  }
  int d1Value = 0;
  int pos = json.indexOf("\"D1\":"); // Encontrar la posición donde comienza la clave "D1"
  if (pos != -1)
  {                                     // Si se encuentra la clave "D1"
    pos += 6;                           // Avanzar 6 posiciones para omitir la clave "D1": "
    String d1Str = json.substring(pos); // Obtener la subcadena que contiene el valor de D1
    d1Value = d1Str.toInt();            // Convertir la subcadena a un entero
  }
  return d1Value;
}
char traslateD1(int d1Value)
{
  char Estado;
  if (d1Value == 1)
  {
    Estado = '1';
  }
  if (d1Value == 2)
  {
    Estado = '2';
  }
  if (d1Value == 3)
  {
    Estado = '3';
  }
  if (d1Value == 4)
  {
    Estado = '4';
  }
  if (d1Value == 5)
  {
    Estado = '5';
  }
  if (d1Value == 6)
  {
    Estado = '6';
  }
  if (d1Value == 7)
  {
    Estado = '7';
  }
  if (d1Value == 8)
  {
    Estado = '8';
  }
  if (d1Value == 9)
  {
    Estado = '9';
  }

  return Estado;
}

char Robot::Leer_BT()
{
  if (Bluetooth.available())
  {
    char _command = Bluetooth.read();

    if (_command == '1')
    {
      return 'a';
    }
    if (_command == 's')
    {
      return 'w';
    }
    if (_command == '2')
    {
      return 'c';
    }
    if (_command == '{')
    {
      // Si hay datos disponibles para leer desde el módulo Bluetooth
      String data = Bluetooth.readStringUntil('}'); // Leer los datos hasta que se reciba un salto de línea (\n)
      int d1Value = parseD1Value(data);
      return traslateD1(d1Value);
    }
  }
}
void Robot::Inicializar_Robot()
{
  Motor_1.Inicializar_Motor();
  Motor_2.Inicializar_Motor();
  sonar.Inicializar_ultrasonico();
  Bluetooth.begin(9600); // Iniciar la comunicacion a 9600
}

void Robot::Atras(int Velocidad_1, int Velocidad_2)
{
  Motor_1.Atras(Velocidad_1);
  Motor_2.Atras(Velocidad_2);
}
void Robot::Adelante(int Velocidad_1, int Velocidad_2)
{
  Motor_1.Adelante(Velocidad_1);
  Motor_2.Adelante(Velocidad_2);
}
void Robot::Stop()
{
  Motor_1.Stop();
  Motor_2.Stop();
}

char Robot::Modo_Bluetooth()
{
  int Estado;
  int Velocidad_Max = 255;
  int Velocidad_Med = 180;
  Estado = Leer_BT();

  if (Estado == '6')
  {
    // Arriba_Izquierda
    Adelante(Velocidad_Med, Velocidad_Max);
  }
  else if (Estado == '3')
  {
    // Derecho
    Adelante(Velocidad_Max, Velocidad_Max);
  }
  else if (Estado == '8')
  {
    // Arriba_Derecha
    Adelante(Velocidad_Max, Velocidad_Med);
  }
  else if (Estado == '1')
  {
    // Girar a la izquierda
    Motor_2.Adelante(Velocidad_Max);
    Motor_1.Atras(Velocidad_Max);
  }
  else if (Estado == '2')
  {
    // Girar a la derecha
    Motor_2.Atras(Velocidad_Max);
    Motor_1.Adelante(Velocidad_Max);
  }
  else if (Estado == '7')
  {
    // Abajo Izquierda
    Atras(Velocidad_Med, Velocidad_Max);
  }
  else if (Estado == '4')
  {
    // Reversa
    Atras(Velocidad_Max, Velocidad_Max);
  }
  else if (Estado == '9')
  {
    // Abajo Derecha
    Atras(Velocidad_Max, Velocidad_Med);
  }
  else if (Estado == 'w')
  {
    Stop();
  }
  return Estado;
}

void Robot::Modo_Evasor(int Dis_giro)
{
  Distancia = sonar.Obtener_Distancia();

  if (Distancia <= Dis_giro)
  {
    // Girar a la derecha
    Motor_1.Atras(255);
    Motor_2.Adelante(255);
    delay(500);
  }
  else
  {
    Adelante(255, 255);
    delay(100);
  }
}
void Robot::Modo_Seguidor()
{
  int val_sensor_Derecha = Sensor_1.Leer_sensor();
  int val_sensor_Izquierda = Sensor_2.Leer_sensor();

  if (val_sensor_Derecha == 0 && val_sensor_Izquierda == 0)
  {
    Adelante(255, 255);
  }
  else if (val_sensor_Derecha == 0 && val_sensor_Izquierda == 1)
  {
    Adelante(255, 0);
  }
  else if (val_sensor_Derecha == 1 && val_sensor_Izquierda == 0)
  {
    Adelante(0, 255);
  }
}
