#include <stm32f1xx.h>



int main(void){
    //Enciende GPIO B y C
    //RCC->APB2ENR | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOCEN;
// conf C13 como salida
    //GPIOC->CRH = (GPIOC->CHRH & not(GPIO_CRH_CNF13 | GPIO_CRH_MODE13))
    //               | GPIO_CRH_MODE13_1;

    //GPIOB->CRH = (GPIOB -> CRH & not(GPIO_CRH_CNF9 | GPIO_CRH_CNF9))
    //              | GPIO_CRH_CNF9_1;
    //GPIO->ODR |= GPIO_ODR_ODR9;
    unsigned volatileA = 0x80000000U, B=1U<<2, C=1U<<4, D=1U<<7;
    // bit set
    A |= B;
    A |= C;
    A |= D;

    //bit clear
    A &= not(B | C);  //ver teclado
    A &= notC;

    //bit flip
    A &= D | B;

    A = 0;
    //Inicia con luz
    GPIOC->ODR|= GPIO_ODR_ODR13//ver donde colocar este
    while(1){                             //Lazo infinito con while            
        //Esperar que se preesione el pulsador
        while((GPIOB->IDR & GPIO_IDR_IDR9));
        GPIOC->ODR ^=GPIO_ODR_ODR13;
    }
    
   // for(;;){                              //Lazo infinito con for
    //
  //  }
    return 0;       // x/wx A usar en terminal para ver variable A
}