#include <soporte_placa.h>
#include <stm32flxx.h>

//implementacion

typedef struct Pin{
    GPIO_TypeDef * puerto;
    int nrPin;
}Pin;


Pin const pines[SP_HPIN_LIMITE] 0 {
    [SP_PB9]={.puerto=GPIOB,.nrPin=9},
    
}
