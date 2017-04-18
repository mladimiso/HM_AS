#include "common/includes/circular_buffer.h"

//*****************************************************************************
//
// Initialization of circular buffer
//
//*****************************************************************************
uint8_t circularInit(CircularBuffer_t *buff)
{
	if (buff != NULL)
	{
		buff->head = buff->buffer;
		buff->tail = buff->buffer;
		return 1;
	}
	return 0;
}

//*****************************************************************************
//
// Function checks if a circular buffer is full
// returns 1 if CB is full or 0 if it is not
//
//*****************************************************************************
uint8_t circularIsFull(CircularBuffer_t *buff)
{
	if(buff->head == buff->buffer && buff->tail == (buff->buffer + CAPACITY - 1))
		return 1;
	else if ((buff->head - buff->tail) == 1)
		return 1;
	else return 0;
}

//*****************************************************************************
//
// Function checks if a CB  is empty
// returns 1 if CB is empty or 0 if it is not
//
//*****************************************************************************
uint8_t circularIsEmpty(CircularBuffer_t *buff)
{
	if (buff->head == buff->tail)
		return 1;
	return 0;
}

//*****************************************************************************
//
// Function put a new element in CB (at end)
//
//*****************************************************************************
void circularPut(CircularBuffer_t *buff, 
				 uint8_t el)
{
	if ( ((buff->buffer + CAPACITY - 1) - buff->tail) == 0 )
	{
		*(buff->tail) = el;
		buff->tail = buff->buffer;
	}
	*(buff->tail) = el;
	buff->tail++;
}

//*****************************************************************************
//
// Function gets a element from CB 
//
//*****************************************************************************
uint8_t circularGet(CircularBuffer_t *buff, 
					uint8_t *el)
{
	if (circularIsEmpty(buff))
	{
		return 0;
	}
	else if ( ((buff->buffer + CAPACITY - 1) - buff->head) == 0 )
	{
		*el = *(buff->head);
		buff->head = buff->buffer;
		return 1;
	}
	else 
	{
		*el = *(buff->head);
		buff->head++;
		return 1;
	}
}

//*****************************************************************************
//
// Function that clears a CB
//
//*****************************************************************************
void circularEmptyBuffer(CircularBuffer_t *buff)
{
	circularInit(buff);
}

uint8_t circularSize(CircularBuffer_t *buff)
{
	uint8_t *tmp = buff->head;
	uint8_t cnt = 0;
	while(tmp != buff->tail)
	{
		if ( ((buff->buffer + CAPACITY - 1) - tmp) == 0 )
		{
			cnt++;
			tmp = buff->buffer;
		}
		cnt++;
		tmp++;
	}
	return cnt;	
}

int circular2array(CircularBuffer_t *c_buff, 
									 uint8_t *uc_array)
{	
	unsigned char *tmp = c_buff->head;
	int i = 0;
	while(tmp != c_buff->tail)
	{
		*(uc_array + i) = *(tmp);
		tmp++;
		i++;
	}
	return circularSize(c_buff);
}

uint8_t array2circular(CircularBuffer_t *c_buff, 
									 uint8_t *uc_array, 
									 uint8_t size)
{
	int i;
	if (circularIsEmpty(c_buff) && size < CAPACITY)
	{
		for (i = 0; i < size; i++)
		{
			circularPut(c_buff, *(uc_array + i));
		}
		return 1;
	}
	else
	{
		return 0;
	}
}
	
