#include <stm32f1xx.h>

void PinInit(GPIO_TypeDef* GPIO, uint8_t pin, uint8_t bitmask){
    uint8_t GPIOoffset = 0;
    if(GPIO == GPIOA) GPIOoffset = 2;
    else if(GPIO == GPIOB) GPIOoffset = 3;
    else if(GPIO == GPIOC) GPIOoffset = 4;
    RCC->APB2ENR |= (1 << GPIOoffset);
    if(pin > 7){
        uint8_t offset = (pin - 8) * 4;
        GPIO->CRH &= ~(0xF << offset);
        GPIO->CRH |= (bitmask << offset);
    }
    else{
        GPIO->CRL &= ~(0xF << pin * 4);
        GPIO->CRL |= (bitmask <<  pin * 4);
    }
}