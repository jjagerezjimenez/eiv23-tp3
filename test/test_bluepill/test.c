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
static  void configurarPinB12Salida(void){
    GPIOB->CRH &= ~(GPIO_CRH_CNF12_Msk | GPIO_CRH_MODE12_Msk);
    GPIOB->CRH |= (~GPIO_CRH_CNF12_Msk | GPIO_CRH_MODE12_Msk);
}

static void PRUEBA_CONFIG_ENTRADA ()  {
    //enciendeGpioB();
    //configurarPinB12Salida();
    apagaGpioB();
    TEST_ASSERT_BITS_LOW(RCC_APB2ENR_IOPBEN, RCC->APB2ENR);
    BP_inicio();
    BP_Pin_mode(B12,IN);
    TEST_ASSERT_BITS_HIGH(RCC_APB2ENR_IOPBEN, RCC->APB2ENR);
    TEST_ASSERT_BITS( GPIO_CRH_CNF12_Msk | GPIO_CRH_MODE12_Msk ,0x00040001,GPIOB->CRH); //como ponerlo mas claro?    
}

void main(){
    BP_inicio();
    BP_delay(500);
    UNITY_BEGIN();
    RUN_TEST(PRUEBA_CONFIG_ENTRADA);
    RUN_TEST()
    UNITY_END();


}