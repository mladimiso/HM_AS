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

int circular2array(CircularBuffer_t * , 
									uint8_t * );
uint8_t array2circular(CircularBuffer_t *c_buff, 
									 uint8_t *uc_array, 
									 uint8_t size);

#endif //__CIRCULAR_BUFFER__
