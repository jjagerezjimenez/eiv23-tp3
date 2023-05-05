#include <stm32f1xx.h>

#define Ciclo_pasada_lazo 13
#define clkkHz 8000 // Frecuencia de reloj en Mhz, 8Mhz 
#define delay_s 60 // Tiempo de espera en segundos, 1 minuto
//Notacion de pines: PuertoPin, ej Puerto C Pin 13 C13

int main(void){

    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN; //Activa los relojes puertos B y C
    GPIOC->CRH = (GPIOC -> CRH & ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13))  | GPIO_CRH_MODE13_1; //Configura C13 como salida
    /*
    Puerto C CRH MODE13 10, salida a 2 Mhz
    Pûerto C CRH CNF13  00, salida general Push-Pull
    */
    GPIOB->CRH = (GPIOB -> CRH & ~(GPIO_CRH_CNF9  | GPIO_CRH_MODE9 ))  | GPIO_CRH_CNF9_1;   //Configurta B9 como entrada 
    /*
    Puerto B CRH MODE9 00, entrada
    Pûerto B CRH CNF9  10, Entrada con Pull DOWN/UP (Ver ODR)
    */
    GPIOB->ODR |= GPIO_ODR_ODR9; // Pone la entrada del pin 9 en modo Pull UP
    GPIOC->ODR|= GPIO_ODR_ODR13;// ODR13 en 1 ¿Cambia algo?, pone el pin 13 en 1
    //La luz se enciende al principio, luego al apretar el botòn, permaneces enecendida 1 minuto, para entrar en el modo normal de funcionamiento
    while(1){                                    
        //Lazo infinito 
        while((GPIOB->IDR & GPIO_IDR_IDR9)); // Entramos al comprobar que que el pin 9 està en 1
        GPIOC->ODR |= GPIO_ODR_ODR13; //Enciende la Luz
        //esperar 1 minuto
        for (unsigned i=0;i< delay_s ;i++){
            for (unsigned volatile j;j<((clkkHz)/Ciclo_pasada_lazo);++j);              
        }
        GPIOC->ODR &= ~(GPIO_ODR_ODR13);
    }   
    return 0;       // x/wx A usar en terminal para ver variable A
}

