#include <BluePillHal.h>
#include <stm32f1xx.h>
#include <control_luz.h>


void SysTick_Handler(void);

void BP_inicio(){
    SystemCoreClockUpdate();
    uint32_t const CUENTAS_POR_MILISEG = SystemCoreClock / 1000;
    SysTick_Config(CUENTAS_POR_MILISEG);}

typedef struct Pin {
GPIO_TypeDef * puerto;
int npin;
}Pin;

static Pin const pines[PINLIMITE] = {
    [B12]  = {.puerto = GPIOB, .npin=12},
    [C13] = {.puerto = GPIOC, .npin=13},

};


bool BP_Pin_read(BP_PuertoPin pin){
    return pines[pin].puerto->IDR & (0x1UL << pines[pin].npin);
}

void BP_Pin_mode(BP_PuertoPin pin, BP_ModoPin modo){
    uint32_t bitsModo;
    int const offset_habilitacion = (((uint32_t)(pines[pin].puerto) >> 10) & 0xF);
    uint32_t const mascara_habilitacion = 1UL<<offset_habilitacion;
    RCC->APB2ENR |= mascara_habilitacion;
    switch (modo) {
        case IN_PULL_UP:
            bitsModo = 0b1000;
            pines[pin].puerto->ODR |= (0x1UL << pines[pin].npin);
        break;
        case OUT_2MHz:
            bitsModo = 0b0010;
        break;
        case IN:
            bitsModo = 0b0100;
        break;
        default:
            bitsModo = 0;
        break;
    }

    int const OFFSET_CONFIG =  ((pines[pin].npin % 8) * 4);
    int const MASCARA_CONFIG = 0b1111;
    uint32_t volatile * const CR = (pines[pin].npin < 7) ? &pines[pin].puerto->CRL : &pines[pin].puerto->CRH;

    *CR = (*CR & ~(MASCARA_CONFIG << OFFSET_CONFIG)) | (bitsModo << OFFSET_CONFIG);

}
void BP_Pin_set(BP_PuertoPin pin, bool valor)
{   if (valor)
        pines[pin].puerto->BSRR = (0x1UL << pines[pin].npin);
    else
        pines[pin].puerto->BRR  =  (0x1UL << pines[pin].npin);

}

static uint32_t volatile ticks;
void BP_delay(uint32_t tiempo){
    uint32_t const ticks_inicial = ticks;
    uint32_t tiempo_transcurrido = ticks - ticks_inicial;
    while(tiempo_transcurrido < tiempo){
        __WFI();
        tiempo_transcurrido = ticks - ticks_inicial;
    }
}

static ControladorLuz * controlador;
void setControlador (ControladorLuz * controladorLuz){
    controlador = controladorLuz;
}

void SysTick_Handler(void){
    ++ticks;
    checkTIMEOUT(controlador);}

void checkTIMEOUT(ControladorLuz * controlador){
    if (getTicks() == controlador->TO_Boton) Maquina_despacha(controlador,EV_TIMEOUT_BOTON);
    if (getTicks() == controlador->TO_Luz)   Maquina_despacha(controlador,EV_TIMEOUT_LUZ);   
    }   
 

int getTicks(void){ return ticks;}


