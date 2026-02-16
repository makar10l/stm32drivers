#include <stm32f1xx.h>
char* buffer = "yohooo its working\n";
//there is no comment btw;
void DMAInit(char* buffer, uint32_t size){
    NVIC_EnableIRQ(DMA1_Channel4_IRQn);
    //clocking
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    //setting for dma(output & input)
    DMA1_Channel4->CPAR = (uint32_t)&(USART1->DR);
    DMA1_Channel4->CMAR = (uint32_t)buffer;
    //CCR
    DMA1_Channel4->CCR = DMA_CCR_DIR | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC;
    DMA1_Channel4->CNDTR = size;
    DMA1_Channel4-> CCR |= DMA_CCR_EN;
}
void UARTInit(uint32_t clocking, uint32_t baudrate){
    //Enabling Peripheral Clocking
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN;
    //PA9
    GPIOA->CRH &= ~(0xF << 4);
    GPIOA->CRH |= (0b1011 << 4);
    //PA10
    GPIOA->CRH &= ~(0xF << 8);
    GPIOA->CRH |= (0b0100 << 8);
    //settings for uart
    USART1->BRR = clocking / baudrate;
    USART1->CR1 |= USART_CR1_UE | USART_CR1_RE | USART_CR1_TE;
    USART1->CR3 |= USART_CR3_DMAT;
} 
void DMA1_Channel4_IRQHandler(){
    if(DMA1->ISR & DMA_ISR_TCIF4){
        DMA1->IFCR |= DMA_IFCR_CTCIF4;
        buffer = "what the fuck?";
        DMA1_Channel4->CCR &= ~DMA_CCR_EN;
        DMA1_Channel4->CMAR = (uint32_t)buffer;
        DMA1_Channel4->CNDTR = 14;
        DMA1_Channel4->CCR |= DMA_CCR_EN;
    }
}

