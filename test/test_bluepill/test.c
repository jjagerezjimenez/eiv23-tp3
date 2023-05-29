#include <unity.h>
#include "BluePillHal.h"
#include <stm32f100xb.h>

void setUp(){

}

void tearDown(){

}

static void enciendeGpioB(void){
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
}
static void apagaGpioB(void){
    RCC->APB2ENR &= ~RCC_APB2ENR_IOPBEN;
}
static  void configurarPinB8Salida(void){
    GPIOB->CRH &= ~(GPIO_CRH_CNF8_Msk | GPIO_CRH_MODE8_Msk);
    GPIOB->CRH |= (~GPIO_CRH_CNF8_Msk | GPIO_CRH_MODE8_Msk);
}

static void Configurar_Entrada ()  {
    enciendeGpioB();
    TEST_ASSERT_BITS_HIGH(RCC_APB2ENR_IOPBEN, RCC->APB2ENR);

    apagaGpioB();
    TEST_ASSERT_BITS_LOW(RCC_APB2ENR_IOPBEN, RCC->APB2ENR);
    BP_inicio();
    TEST_ASSERT_BITS_HIGH(RCC_APB2ENR_IOPBEN, RCC->APB2ENR);
}

void main(){
    BP_inicio();
    BP_delay(500);
    UNITY_BEGIN();
    RUN_TEST(Configurar_Entrada);
    UNITY_END();


}