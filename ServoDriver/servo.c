#include "stm32f1xx.h"
#include "../include/PinInitializer_0.1v.h"
#include <stddef.h>
//TODO: clocking for timers/function for set angle
typedef struct
{
    uint8_t id;
    volatile uint32_t *CCR;
    uint8_t angle;
}Servo ;
void ServoUpdate(Servo* serv){
    if(serv->angle > 180) return;
    *(serv->CCR) = 500 + ((float)(serv->angle) * 11.1);
}
void ServoInit(Servo* serv, uint8_t pin, GPIO_TypeDef* GPIO){
    uint8_t CHANNEL = 5;
    TIM_TypeDef* TIM = NULL;
    volatile uint32_t *CCR;
    PinInit(GPIO, pin, 0b1011);
    if(GPIO == GPIOA && pin > 7 && pin < 12){
        if(pin == 8){CHANNEL = 1; CCR = &(TIM1->CCR1);}
        else if(pin == 9){CHANNEL = 2; CCR = &(TIM1->CCR2);}
        else if(pin == 10){CHANNEL = 3; CCR = &(TIM1->CCR3);}
        else if(pin == 11){CHANNEL = 4; CCR = &(TIM1->CCR4);}
        TIM = TIM1;
        RCC->APB2ENR |= (1 << 11);
        TIM1->BDTR |= TIM_BDTR_MOE;
    }
    else if(GPIO == GPIOA && pin < 4){
        if(pin == 0){CHANNEL = 1; CCR = &(TIM2->CCR1);}
        if(pin == 1){CHANNEL = 2; CCR = &(TIM2->CCR2);}
        if(pin == 2){CHANNEL = 3; CCR = &(TIM2->CCR3);}
        if(pin == 3){CHANNEL = 4; CCR = &(TIM2->CCR4);}
        TIM = TIM2;
        RCC->APB1ENR |= (1 << 0);
    }
    else if(GPIO == GPIOA && pin < 8 && pin > 5){
        if(pin == 6){CHANNEL = 1; CCR = &(TIM3->CCR1);}
        if(pin == 7){CHANNEL = 2; CCR = &(TIM3->CCR2);}
        TIM = TIM3;
        //CCR = &(TIM3->CCR3);
        RCC->APB1ENR |= (1 << 1);
    }
    else if(GPIO == GPIOB && pin < 2){
        if(pin == 0){CHANNEL = 3; CCR = &(TIM3->CCR3);}
        if(pin == 1){CHANNEL = 4; CCR = &(TIM3->CCR4);}
        TIM = TIM3;
        RCC->APB1ENR |= (1 << 1);
    }
    else if(GPIO == GPIOB && pin < 10 && pin > 5){
        if(pin == 6){CHANNEL = 1; CCR = &(TIM4->CCR1);}
        if(pin == 7){CHANNEL = 2; CCR = &(TIM4->CCR2);}
        if(pin == 8){CHANNEL = 3; CCR = &(TIM4->CCR3);}
        if(pin == 9){CHANNEL = 4; CCR = &(TIM4->CCR4);}
        TIM = TIM4;
        RCC->APB1ENR |= (1 << 2);
    }
    if(TIM == NULL) return;
    if(CHANNEL == 5) return;
    __DSB();
    TIM->PSC = 7;
    TIM->ARR = 19999;
    if(CHANNEL == 1){TIM->CCMR1 |= (0b110 << 4) | TIM_CCMR1_OC1PE; TIM->CCER |= TIM_CCER_CC1E;}
    else if(CHANNEL == 2){TIM->CCMR1 |= (0b110 << 8) | TIM_CCMR1_OC2PE; TIM->CCER |= TIM_CCER_CC2E;}
    else if(CHANNEL == 3){TIM->CCMR2 |= (0b110 << 4) | TIM_CCMR2_OC3PE; TIM->CCER |= TIM_CCER_CC3E;}
    else if(CHANNEL == 4){TIM->CCMR2 |= (0b110 << 8) | TIM_CCMR2_OC4PE; TIM->CCER |= TIM_CCER_CC4E;}
    TIM->CR1 |= TIM_CR1_CEN;
    serv->id = 1;
    serv->angle = 0;
    serv->CCR = CCR;
    
}

