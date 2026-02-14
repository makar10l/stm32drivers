#include <stm32f1xx.h>

void LedInit(GPIO_TypeDef* GPIO, uint8_t GPIO_offset,uint8_t pin);
void LedOn(GPIO_TypeDef* GPIO, uint8_t pin);
void LedOff(GPIO_TypeDef* GPIO, uint8_t pin);