#ifndef __CIRCULAR_BUFFER__
#define __CIRCULAR_BUFFER__

#include <stdint.h>
#include <stdlib.h>

#define CAPACITY 128

typedef struct circularBuffer 
{
		uint8_t buffer[CAPACITY];
		uint8_t *head;
		uint8_t *tail;
} CircularBuffer_t;

uint8_t circularInit(CircularBuffer_t * );

uint8_t circularSize(CircularBuffer_t * );
uint8_t circularIsFull(CircularBuffer_t * );
uint8_t circularIsEmpty(CircularBuffer_t * );

void circularPut(CircularBuffer_t * , 
					uint8_t );
uint8_t circularGet(CircularBuffer_t * , 
					uint8_t * );

void circularEmptyBuffer(CircularBuffer_t * );
void circularDump(CircularBuffer_t * );

#endif //__CIRCULAR_BUFFER__
