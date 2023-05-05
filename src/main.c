#define USA_BLUEPILL_HALL

#ifdef USA_BLUEPILL_HALL

#include <BluePillHal.h>

int main(void) {
    BP_inicio();
    BP_Pin_mode(B9,IN_PULL_UP);
    BP_Pin_mode(C13,OUT_2MHz);
    BP_Pin_set(C13,1);
    while(1){
        while (!(BP_Pin_read(B9))){
               BP_Pin_set(C13,0);
               BP_delay(60*1000);
               BP_Pin_set(C13,1);
        };
    };
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