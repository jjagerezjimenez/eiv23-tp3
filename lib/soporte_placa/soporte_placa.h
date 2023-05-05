#ifndef SOPORTE_PLACA_H
#define SOPORTE_PLACA_H

#include <stdbool.h>

// declaraciones
// Espacion de nombres: SP_

enum SP_Pines{
	SP_PB9,
	SP_PC13,
	SP_PLED = SP_PC13,
	SP_HPIN_LIMITE
};

typedef int SP_HPin;

typedef enum SP_Pin_Modo{
	SP_PIN_ENTRADA,	//Entrada flotante
	SP_PIN_ENTRADA_PULLUP, //entrada con resistencia pull up interna
	SP_PIN_ENTRADA_PULLDN,	//pull down
	SP_PIN_SALIDA,		//salida pushpull
	SPPIN_SALIDA_OPEN_DRAIN	//salida con drenador abierto
}SP_Pin_Modo;

void SP_Pin_setModo(SP_HPin pin, SP_Pin_Mode modo);

bool SP_Pin_read(SP_HPin pin);

void SP_Pin_write(SP_HPin pin, bool valor);

#endif