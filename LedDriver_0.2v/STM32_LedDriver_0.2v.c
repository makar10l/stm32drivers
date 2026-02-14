#include <stm32f1xx.h>

void LedInit(GPIO_TypeDef* GPIO, uint8_t GPIO_offset,uint8_t pin){
    RCC->APB2ENR |= (1 << GPIO_offset);
    if(pin > 7){
        uint8_t offset = (pin - 8) * 4;
        GPIO->CRH &= ~(0xF << offset);
        GPIO->CRH |= (0x2 << offset);
    }
    else{
        GPIO->CRL &= ~(0xF << pin * 4);
        GPIO->CRL |= (0x2 <<  pin * 4);
    }
}
void LedPullToPlus(GPIO_TypeDef* GPIO, uint8_t pin){
    GPIO->BSRR = (1 << pin);
}
void LedPullToGround(GPIO_TypeDef* GPIO, uint8_t pin){
   GPIO->BSRR = (1 << (pin + 16));
}