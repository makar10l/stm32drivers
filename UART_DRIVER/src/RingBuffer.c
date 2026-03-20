#include <uart.h>
#define NULL ((void*)0)
void BufferInit(){
    UART_BUFFER.WRITE = 0; 
    UART_BUFFER.READ = 0;
}
