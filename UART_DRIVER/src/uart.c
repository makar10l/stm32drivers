#include <stm32f1xx.h>
#include <PinInitializer_0.1v.h>
#include <uart.h>
#include <RingBuffer.h>

RingBuffer UART_BUFFER;

int UARTInit(uint32_t clocking, uint32_t baudrate, uint8_t remapping){
    //Enabling Peripheral Clocking
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

    switch (remapping)
    {
    case 0:
        PinInit(GPIOA, 9, 0b1011);
        PinInit(GPIOA, 10, 0b0100);
        break;
    case 1:
        PinInit(GPIOB, 6, 0b1011);
        PinInit(GPIOB, 7, 0b0100);
        AFIO->MAPR |= AFIO_MAPR_USART1_REMAP;
        break;
    default:
        return -1; // means remapping variable has wrong value
    }

    //settings for uart
    USART1->BRR = clocking / baudrate;
    USART1->CR1 |= USART_CR1_UE | USART_CR1_RE | USART_CR1_TE |  USART_CR1_RXNEIE;
    USART1->CR3 |= USART_CR3_DMAT;
    return 0;
}

int UartSend(UARTPackage* msg){
    int aviable = UART_BUFFER.WRITE - UART_BUFFER.READ;
    
    if(BUFFER_SIZE - aviable < msg->size) return -1; // no space left
    //copying data to uart buffer
    for(int i = 0; i < msg->size; i++){
        UART_BUFFER.buffer[(UART_BUFFER.WRITE + i) & (BUFFER_SIZE_MASK)] = msg->chars[i];
    }
    UART_BUFFER.WRITE += msg->size;

    aviable = UART_BUFFER.WRITE - UART_BUFFER.READ;

    if(!(DMA1_Channel4->CCR & DMA_CCR_EN)){
        /*
        setting INPUT dma register with buffer address, 
        index is calculated by bitmask(alternative for UART_BUFFER.READ % 256)
        */
        DMA1_Channel4->CMAR = (uint32_t)&UART_BUFFER.buffer[UART_BUFFER.READ & BUFFER_SIZE_MASK];
        DMA1_Channel4->CNDTR = (uint32_t)aviable;
        DMA1_Channel4->CCR |= DMA_CCR_EN;
        return 1; // means that function starts dma
    }
    return 0;
}

int UartRead(char* buffer){return 0;}