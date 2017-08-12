#include "Arduino.h" //declaração da biblioteca que reune as funções padrão do Arduino
#include "l298n.h"  //declaração da biblioteca que reune a listagem das funções presentes na biblioteca


l298n :: l298n(int IN1, int IN2, int EN //método construtor da classe l298n
{
//declaração dos pinos necessários para implementação da biblioteca
pinMode(IN1, OUTPUT);
pinMode(IN2, OUTPUT);
pinMode(EN, OUTPUT);

//artifício utilizado para usar sempre a variável _IN e _EN sem ter conflito de nomes durante a execução
_IN1 = IN1;
_IN2 = IN2;
_EN = EN;
}

void l298n :: mover_sentido1() //método que move o motor em um sentido 
{
digitalWrite(_IN1, HIGH); //manda nível lógico alto para o pino IN1
digitalWrite(_IN2, LOW);  //manda nível lógico baixo para o pino IN2
}

void l298n :: mover_sentido2() //método que move o motor em outro sentido
{
digitalWrite(_IN1, LOW); //manda nivel lógico baixo para o pino IN1
digitalWrite(_IN2, HIGH); //manda nível lógico alto para o pino IN2
}

void l298n :: parar()  //método que para um motor
{
	digitalWrite(_IN1, LOW); //manda nivel lógico baixo para o pino IN1
	digitalWrite(_IN2, LOW); //manda nível lógico alto para o pino IN2
}
void setSpeed(int pwm)  //método que seta a velocidade de um motor
{
	analogWrite(_EN, pwm); //enviar um valor analógico para o pino EN 
}
