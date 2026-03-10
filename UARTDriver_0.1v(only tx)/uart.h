#include <stm32f1xx.h>
typedef struct{
    char* buffer;
    int size;
} UARTMessage;
void DMAInit(UARTMessage* msg);
void UARTInit(uint32_t clocking, uint32_t baudrate);
void UartSend(UARTMessage* msg);