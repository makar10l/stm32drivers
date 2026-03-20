#include <uart.h>
#include <dma.h>
#include <RingBuffer.h>
#include <PinInitializer_0.1v.h>
UARTPackage msg = {
    .chars = "|1 2 3| ",
    .size = 8,
};
int main(){
    __enable_irq();
    BufferInit();
    UARTInit(8000000, 9600, 1);
    DMAInit();  
    while(1){
        UartSend(&msg);
        __WFI(); 
    }
}