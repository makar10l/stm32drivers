#include <dma.h>
#include <uart.h>
#include <stm32f1xx.h>
int TX_SIZE;

void DMAInit(RingBuffer* buffer){
    //clocking
    NVIC_EnableIRQ(DMA1_Channel4_IRQn);
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    //setting for dma(output & input)
    DMA1_Channel4->CPAR = (uint32_t)&(USART1->DR);
    DMA1_Channel4->CMAR = (uint32_t)&buffer->buffer[buffer->READ & BUFFER_SIZE_MASK];
    //CCR
    DMA1_Channel4->CCR = DMA_CCR_DIR | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC;
}
void DMA1_Channel4_IRQHandler(){
    if(DMA1->ISR & DMA_ISR_TCIF4){
        //clearing TC(transer complete) flag
        DMA1->IFCR = DMA_IFCR_CTCIF4; 
        UART_BUFFER.READ += TX_SIZE;
        int aviable = UART_BUFFER.WRITE - UART_BUFFER.READ;
        if(aviable > 0){
        DMA1_Channel4->CNDTR = aviable;
        TX_SIZE = aviable;
        DMA1_Channel4->CMAR = (uint32_t)&UART_BUFFER.buffer[UART_BUFFER.READ & BUFFER_SIZE_MASK];
        DMA1_Channel4->CCR |= DMA_CCR_EN;
        }
        else{
            DMA1_Channel4->CCR &= ~DMA_CCR_EN;
        }
    }
}