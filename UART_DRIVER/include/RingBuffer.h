#ifndef RINGBUFFER_H
#define RINGBUFFER_H
#define BUFFER_SIZE (256)
#define BUFFER_SIZE_MASK (BUFFER_SIZE - 1)
#include <stdint.h>
typedef struct{
    char buffer[BUFFER_SIZE];
    volatile uint16_t WRITE;
    volatile uint16_t READ;
} RingBuffer;
void BufferInit();
static inline void push_to_buffer(RingBuffer* buff, char ch){
    if(BUFFER_SIZE - (buff->WRITE - buff->READ) > 0)
    {
        buff->buffer[buff->WRITE & BUFFER_SIZE_MASK] = ch; 
        buff->WRITE++;
    }
}
#endif