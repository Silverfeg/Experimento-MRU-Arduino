#ifndef l298n_h
#define l298n_h
#include <Arduino.h>

class l298n
{
public:
	l298n(int IN1, int IN2, int EN);
	void mover_sentido1();
	void mover_sentido2();
	void parar();
	void setSpeed();
	int _IN1;
	int _IN2;
	int _EN;
	};
#endif
