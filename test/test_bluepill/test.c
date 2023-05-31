#include <unity.h>
#include "BluePillHal.h"
#include <stm32f100xb.h>


#define OUT2Mhz     0x2
#define IN_          0x4
#define IN_PULLUP   0x8

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

static void PRUEBA_PIN_MODE ()  {
    apagaGpioB();
    TEST_ASSERT_BITS_LOW(RCC_APB2ENR_IOPBEN, RCC->APB2ENR);
    enciendeGpioB();
    BP_Pin_mode(B12,IN);
    TEST_ASSERT_BITS_HIGH(RCC_APB2ENR_IOPBEN, RCC->APB2ENR);
    TEST_ASSERT_BITS(GPIO_CRH_CNF12_Msk | GPIO_CRH_MODE12_Msk, IN_ << GPIO_CRH_MODE12_Pos     ,GPIOB->CRH); 
    BP_Pin_mode(B12, OUT_2MHz);
    TEST_ASSERT_BITS(GPIO_CRH_CNF12_Msk | GPIO_CRH_MODE12_Msk , OUT2Mhz << GPIO_CRH_MODE12_Pos,GPIOB->CRH);
}

void main(){
    BP_inicio();
    BP_delay(500);
    UNITY_BEGIN();
    RUN_TEST(PRUEBA_PIN_MODE);

    UNITY_END();


}