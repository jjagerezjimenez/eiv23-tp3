#define USA_BLUEPILL_HALL

#ifdef USA_BLUEPILL_HALL
#define BOTON B12
#define PIN_LED C13
#define TIEMPO_LUZ_ENCENDIDA 60000
#define TIEMPO_PARA_PRESIONAR 1000
#include <BluePillHal.h>
#include "control_luz.h"


void luzOn(void){BP_Pin_set(PIN_LED,1);}
void luzOff(void){BP_Pin_set(PIN_LED,0);}

void setTimeoutLuz(ControladorLuz * controlador){
    controlador->TO_Luz = getTicks() +  controlador->tiempoLuz;
    }
void setTimeoutBoton(ControladorLuz *  controlador){
   controlador->TO_Boton = getTicks() +  controlador->tiempoBoton;
    }

bool botonPresionado(){
static  TiempoMilisegundos ultimoCambio = 0;
static  bool valorAnterior;    
        bool valorActual = BP_Pin_read(BOTON);
        bool Presionado = false;

        if (valorActual != valorAnterior && (getTicks()-ultimoCambio) >= 15){
            Presionado = true;
            ultimoCambio = getTicks();
            valorAnterior=valorActual;
        }
        return Presionado;     
}


int main(void) {
    BP_inicio();
    BP_Pin_mode(BOTON,IN);
    BP_Pin_mode(PIN_LED, OUT_2MHz);
    static ControladorLuz implControlador;
    Maquina * controlador = ControladorLuz_init(&implControlador,TIEMPO_LUZ_ENCENDIDA,TIEMPO_PARA_PRESIONAR);
    setControlador( (ControladorLuz*) controlador );
    while(1){
        Maquina_procesa(controlador);
        if (botonPresionado()){
            Maquina_despacha(controlador,EV_BOTON);
        }
    }
}

#else

#include <stm32f1xx.h>

#define Ciclo_pasada_lazo 11
#define clkkHz 8000 // Frecuencia de reloj en Mhz, 8Mhz
#define delay_s 60000  // Tiempo de espera en segundos, 1 minuto
// Notacion de pines: PuertoPin, ej Puerto C Pin 13 C13

int main(void)
{

    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN;                             // Activa los relojes puertos B y C

    GPIOB->CRH = (GPIOB->CRH & ~(GPIO_CRH_CNF9 | GPIO_CRH_MODE9)) | GPIO_CRH_CNF9_1; // Configurta B9 como entrada
    /*
    Puerto B CRH MODE9 00, entrada
    Pûerto B CRH CNF9  10, Entrada con Pull DOWN/UP (Ver ODR)
    */
    GPIOB->ODR |= GPIO_ODR_ODR9;  // Pone la entrada del pin 9 en modo Pull UP

    /*
    Puerto C CRH MODE13 10, salida a 2 Mhz
    Pûerto C CRH CNF13  00, salida general Push-Pull
    */
    GPIOC->CRH = (GPIOC->CRH & ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13)) | GPIO_CRH_MODE13_1; // Configura C13 como salida
    GPIOC->ODR |= GPIO_ODR_ODR13; // ODR13 en 1  pone el pin 13 en 1
    while (1)
    {
        // Lazo infinito
        while ((GPIOB->IDR & GPIO_IDR_IDR9)); // NO-Entramos al comprobar que que el pin 9 està en 1
        GPIOC->ODR &= ~(GPIO_ODR_ODR13);
        // esperar 1 minuto
        for (unsigned i = 0; i < delay_s; i++)
        {
            for (unsigned volatile j = 0; j < ((clkkHz) / Ciclo_pasada_lazo); ++j);
        }
        GPIOC->ODR |= GPIO_ODR_ODR13; // Enciende la Luz
    }
    return 0; // x/wx A usar en terminal para ver variable A
}


#endif