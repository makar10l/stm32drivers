#include <stm32f1xx.h>
#include "PinInitializer_0.1v.h"
#include "uart.h"
//there is no comment btw;

void DMAInit(UARTMessage* msg){
    //clocking
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    //setting for dma(output & input)
    DMA1_Channel4->CPAR = (uint32_t)&(USART1->DR);
    DMA1_Channel4->CMAR = (uint32_t)msg->buffer;
    //CCR
    DMA1_Channel4->CCR = DMA_CCR_DIR | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC;
    DMA1_Channel4->CNDTR = msg->size;
    DMA1_Channel4->CCR |= DMA_CCR_EN;
}
void UARTInit(uint32_t clocking, uint32_t baudrate){
    //Enabling Peripheral Clocking
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    PinInit(GPIOA, 9, 0b1011);
    PinInit(GPIOA, 10, 0b0100);
    //settings for uart
    USART1->BRR = clocking / baudrate;
    USART1->CR1 |= USART_CR1_UE | USART_CR1_RE | USART_CR1_TE;
    USART1->CR3 |= USART_CR3_DMAT;
}
void UartSend(UARTMessage* msg){
    DMA1_Channel4->CCR &= ~DMA_CCR_EN;
    DMA1_Channel4->CMAR = (uint32_t)msg->buffer;
    DMA1_Channel4->CNDTR = (uint32_t)msg->size;
    DMA1_Channel4->CCR |= DMA_CCR_EN;
}