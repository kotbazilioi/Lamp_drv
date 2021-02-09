/*
    FreeRTOS V8.2.3 - Copyright (C) 2015 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/


/*
 * Implementation of pvPortMalloc() and vPortFree() that relies on the
 * compilers own malloc() and free() implementations.
 *
 * This file can only be used if the linker is configured to to generate
 * a heap memory area.
 *
 * See heap_1.c, heap_2.c and heap_4.c for alternative implementations, and the
 * memory management pages of http://www.FreeRTOS.org for more information.
 */

#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "heap_3.h"
#include "fsl_debug_console.h"

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
all the API functions to use the MPU wrappers.  That should only be done when
task.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#include "FreeRTOS.h"
#include "task.h"

#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE



#define HEAP_ALIGN	portBYTE_ALIGNMENT

#pragma segment="HEAP"
heap_t system_heap;

// Добавляет к 'heap' еще один отдельно находящийся блок.
// *heap - Указатель на основной уже проинициализированный heap
//----------------------------------------------------------------------------
void heapadd( heap_t *heap, heap_mcb *xptr, int size ){
    heap_mcb *tptr = heap->freem;
    // Формирование нового MCB в блоке
    xptr->next = tptr;
    xptr->prev = tptr;
    xptr->ts.bits.size = size-sizeof(heap_mcb);
    xptr->ts.bits.type = MARK_FREE;
    tptr->next = xptr;
}

//---------------------------------------------------------------------------
// Инициализация Heap - обязательный вызов перед использованием
// malloc_z() и free_z()
//---------------------------------------------------------------------------
void init_system_heap(void){
	system_heap.start = (heap_mcb *)(__segment_begin( "HEAP"));
	system_heap.hsize = (unsigned char *)__segment_end( "HEAP") - (unsigned char *)__segment_begin( "HEAP"); 
	system_heap.freem = system_heap.start;
	heapinit( &system_heap );
}

//----------------------------------------------------------------------------
// Инициализирует 'heap'.
// *heap - Указатель на структуру описываюшую heap
// После инициализации heap представляет собой один свободный блок,
// который имеет максимальный размер минус размер MCB.
//----------------------------------------------------------------------------
void heapinit( heap_t *heap ){
    heap_mcb *fmcb;
    fmcb = heap->start;    
    fmcb->next = fmcb; // Циклическая структура    
    fmcb->prev = fmcb; // Указатель на предыдущий MCB указывает сам на себя    
    fmcb->ts.bits.size = heap->hsize - sizeof(heap_mcb); // Размер области памяти    
    fmcb->ts.bits.type = MARK_FREE; // Область памяти свободна
    heap->blocks_count = 1;
    heap->free_blocks_count = 1;
    heap->work_size = 0;
}

void *pvPortMalloc( size_t xWantedSize ){
  return malloc_z( &system_heap, xWantedSize, MARK_SYSTEM);
}
void vPortFree( void *pv ){
  free_z( &system_heap, (pv) );
}

//----------------------------------------------------------------------------
// Зарезервировать блок
//----------------------------------------------------------------------------
void *malloc_z( heap_t *heap, size_t size, int type ){
      vTaskSuspendAll();
      heap_mcb *tptr; // Указатель на выделяемый блок. Поиск начинается с первого свободного блока
      heap_mcb *xptr = NULL;
      void *fptr;
#ifdef HEAP_ALIGN
      if( size &( HEAP_ALIGN - 1 ) )size = size + ( HEAP_ALIGN - ( size &( HEAP_ALIGN - 1 )) );
#endif /* HEAP_ALIGN */
      tptr = heap->start;
      while(1){
        if( tptr->ts.bits.type == MARK_FREE ){
          if(tptr->ts.bits.size >= size){ //блок подходящего размера
            if( xptr == NULL){
              xptr = tptr;
            }else{
              if( xptr->ts.bits.size > tptr->ts.bits.size)xptr = tptr;
            }
          }
        }
        tptr = tptr->next; // переходим к следующему блоку
        if( tptr == heap->start ){ // прошли по кругу 
          break;
        }            
      }  
      if(xptr != NULL)heap->freem = xptr;
      tptr = heap->freem;
      while(1){   
        if( tptr->ts.bits.type == MARK_FREE ){          
	  if( tptr->ts.bits.size == size ){	// Требуемый и найденный размеры памяти равны
            	tptr->ts.bits.type = type;              		// Резервируем блок
                fptr = (unsigned char *)tptr+sizeof(heap_mcb);
                heap->free_blocks_count--;
                heap->work_size += tptr->ts.bits.size;
                tptr->owner = fptr;
		break;
	  }else{ 
              unsigned long sz = size + sizeof(heap_mcb) + 4 ;
              if( (unsigned long)(tptr->ts.bits.size) > sz){ // Массив достаточен для размещения блока и его MCB - делим блок
		xptr = (heap_mcb *)( (unsigned char *)tptr + sizeof(heap_mcb) + size ); //уменьшаем свободный блок
                xptr->next = tptr->next;
                xptr->prev = tptr;
                xptr->ts.bits.size = ( tptr->ts.bits.size - size - sizeof(heap_mcb) );
                xptr->ts.bits.type = MARK_FREE;
                xptr->owner = NULL;
                ( xptr->next )->prev = xptr;
                tptr->next = xptr;
                tptr->ts.bits.size = size;
                tptr->ts.bits.type = type; 
                heap->blocks_count++;
                heap->freem = xptr;
                heap->work_size += size;
                fptr = (unsigned char *)tptr + sizeof(heap_mcb); 
                tptr->owner = fptr;
                break;
              }
          }
        }        
        tptr = tptr->next; // переходим к следующему блоку
        if( tptr == heap->freem ){ // прошли по кругу и подходящего блока не нашли
          fptr = NULL; 	
          PRINTF("Error HEAP malloc \r\n"); //Ошибка// No Memory
          while (1){};
          break;
        }        
     }
     xTaskResumeAll();
     return( tptr->owner );
}

//----------------------------------------------------------------------------
// Освободить зарезервированный блок и объединить соседние с ним пустые блоки
//----------------------------------------------------------------------------
void free_z( heap_t *heap, void *mem_ptr ){
     if(mem_ptr == NULL)return;
     vTaskSuspendAll();
     heap_mcb *xptr;
     heap_mcb *vptr;
     heap_mcb *tptr = heap->freem;  // указатель на заголовок освобождаемого блока
     while(1){
       if(tptr->owner == (void*)mem_ptr){
        tptr->ts.bits.type = MARK_FREE;
        tptr->owner = NULL;
        heap->work_size -= tptr->ts.bits.size;
        heap->free_blocks_count++;        
        while(1){
          xptr = tptr->next; // следующий за освобождаемым блок памяти
          if(xptr == heap->start)break;
          if(  xptr->ts.bits.type == MARK_FREE ){   
                vptr = (heap_mcb *)( (unsigned char *)tptr + sizeof(heap_mcb) + tptr->ts.bits.size );
                if( vptr == xptr )mergeBlocks(heap, tptr,xptr); // Объединяем текущий (tptr) и следующий (xptr) MCB
                else { break; }
	  }else{ break; }
        }
        while(1){
          xptr = tptr->prev; // предшествующий освобождаемому блок памяти
          if(tptr == heap->start)break;
    	  if( xptr->ts.bits.type == MARK_FREE ){
                vptr = (heap_mcb *)( (unsigned char *)xptr + sizeof(heap_mcb) + xptr->ts.bits.size );
                if( (void*)vptr == (void*)tptr )mergeBlocks(heap, xptr,tptr); // Объединяем текущий (tptr) и предыдущий (xptr) блоки
                else { break; }
		tptr = xptr;		      // tptr указывает всегда на освободившийся блок.
          }else{ break; }
        }
        heap->freem = tptr;
        break;
       }
       tptr = tptr->next; // переходим к следующему блоку
       if( tptr == heap->freem )
       { // прошли по кругу и нужного блока не нашли
         PRINTF("Error HEAP free \r\n"); //Ошибка// No Memory
         while (1){};
          break;
       }                
     }
     mem_ptr = NULL;
     xTaskResumeAll();
}

// Объединить два рядом расположенных блока. Результат в tptr
//---------------------------------------------------------------------------
void mergeBlocks( heap_t *heap, heap_mcb* tptr, heap_mcb* xptr ){
  tptr->ts.bits.size = (tptr->ts.bits.size + xptr->ts.bits.size + sizeof(heap_mcb));
  tptr->next = xptr->next;
  xptr = xptr->next; // Если следующий MCB не последний, то меняем в нем mcb.prev на текущий
  xptr->prev = tptr;
  heap->free_blocks_count--;  
  heap->blocks_count--; 
}

// Поменять местами занятый и свободный блоки. Свободный блок изначально расположен перед занятым.
// Может быть, когда-нибудь понадобится перемещаемая память
//------------------------------------------------------------------------------------------------
void shiftBlocks( heap_t *heap, heap_mcb* tptr, heap_mcb* fptr ){
     heap_mcb* xptr = tptr->next;
     heap_mcb ffptr = *fptr;
     heap_mcb ttptr = *tptr;
     unsigned long len = tptr->ts.bits.size + sizeof(heap_mcb); //длина переписываемых данных
     memmove(fptr, tptr, len);
     tptr = fptr;
     tptr->ts.bits.size = ttptr.ts.bits.size;
     tptr->prev = ffptr.prev;
     tptr->owner = (unsigned char *)tptr + sizeof(heap_mcb);
     fptr = (heap_mcb *)( (unsigned char *)tptr + sizeof(heap_mcb) + tptr->ts.bits.size );
     tptr->next = fptr;
     fptr->prev = tptr;
     fptr->next = ttptr.next;
     fptr->ts.bits.size= ffptr.ts.bits.size;
     xptr->prev = fptr;     
}

// Получить максимальный размер свободного блока
//---------------------------------------------------------------------------
unsigned long getHeapFreeSize(){
  unsigned long ret = 0;
  heap_mcb *tptr = system_heap.start; 
  heap_mcb *xptr = system_heap.start;    
  vTaskSuspendAll();
  while(1){
        if( tptr->ts.bits.type == MARK_FREE ){
            if(ret < tptr->ts.bits.size) ret = tptr->ts.bits.size;
        }
        tptr = tptr->next; // переходим к следующему блоку
        if( tptr == xptr ){ // прошли по кругу 
          break;
        }            
  }
  xTaskResumeAll();
  return ret;
}

unsigned long getHeapSize(){
  return system_heap.hsize;
}

// Получить максимальный размер свободного блока
//---------------------------------------------------------------------------
unsigned long getHeapMaxSize( heap_t *heap ){
  unsigned long ret = 0;
  heap_mcb *tptr = heap->freem; 
  heap_mcb *xptr = heap->freem;      
  vTaskSuspendAll();
  while(1){
        if(ret < tptr->ts.bits.size) ret = tptr->ts.bits.size;
        tptr = tptr->next; // переходим к следующему блоку
        if( tptr == xptr ){ // прошли по кругу 
          break;
        }            
  }
  xTaskResumeAll();
  return ret;
}

// Объединить все свободные блоки, если это возможно
//---------------------------------------------------------------------------
void defragHeap( heap_t *heap){
  vTaskSuspendAll();
  heap_mcb *tptr = heap->start;
  heap_mcb *xptr = NULL; 
  heap_mcb *vptr = NULL;
  while(1){
       if(tptr->ts.bits.type != MARK_SYSTEM){
         if(tptr != heap->start){   // первый блок не удаляется
          xptr = tptr->next;
          if(xptr->ts.bits.type == MARK_FREE){ 
            vptr = (heap_mcb *)( (unsigned char *)tptr + sizeof(heap_mcb) + tptr->ts.bits.size ); 
            if((void *)xptr == (void *)vptr){
              mergeBlocks(heap, tptr, xptr); //следующий блок свободный, объединяем
            }
          }else{//следующий блок занят, сдвигаем его, если это можно
//            shiftBlocks(heap, xptr, tptr);
          }
         }
       }
       tptr = tptr->next; // переходим к следующему блоку
       if( tptr == heap->start){ //прошли по кругу 
            break;
       }
  }  
  xTaskResumeAll();  
}

/*


void *pvPortMalloc( size_t xWantedSize )
{
void *pvReturn;

	vTaskSuspendAll();
	{
		pvReturn = malloc( xWantedSize );
		traceMALLOC( pvReturn, xWantedSize );
	}
	( void ) xTaskResumeAll();

	#if( configUSE_MALLOC_FAILED_HOOK == 1 )
	{
		if( pvReturn == NULL )
		{
			extern void vApplicationMallocFailedHook( void );
			vApplicationMallocFailedHook();
		}
	}
	#endif

	return pvReturn;
}


void vPortFree( void *pv )
{
	if( pv )
	{
		vTaskSuspendAll();
		{
			free( pv );
			traceFREE( pv, 0 );
		}
		( void ) xTaskResumeAll();
	}
}

*/

