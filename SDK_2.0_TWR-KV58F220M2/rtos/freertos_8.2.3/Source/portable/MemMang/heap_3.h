#ifndef __HEAP_Z_H
#define __HEAP_Z_H

#pragma pack( push, 1 )
typedef union type_size {
	unsigned long mark;
	struct {
	   unsigned size:24;
	   unsigned type:8;
	}bits;
}type_size;

// Memory Control Block (MCB)
//----------------------------------------------------------------------------
typedef struct heap_mcb {
	struct heap_mcb *next;   	// Указатель на следующий MCB. mcb.next  последнего MCM всегда указывает на  первый MCB.
	struct heap_mcb *prev;   	// Указатель на предыдущий MCB. Для первого MCB этот указатель указывает сам на себя.
	union type_size ts;  		// Размер блока памяти  Контролируемый блок памяти расположен сразу за MCB
        unsigned char* owner;                    // Указатель на блок памяти. Возвращается функциями выделения памяти и не меняется при перемещении блоков
} heap_mcb;


// Структура кучи 
//----------------------------------------------------------------------------
typedef struct heap_t { 	
	struct heap_mcb *start; // Указатель на начало heap (первый MCB)	
	struct heap_mcb *freem; // Указатель на первый свободный MCB	
	unsigned long hsize; // полный размер кучи
        long work_size; // занятый объем кучи
        long blocks_count; // общее число блоков
        long free_blocks_count; // число свободных блоков
} heap_t;
#pragma pack( pop )

void init_system_heap( void ); // Инициализация 
void heapinit( heap_t *heap ); // Инициализация произвольной памяти в качестве heap

// Резервирует память в куче 'heap', размером 'size' байт.
// Возвращает указатель на зарезервированную память
// Если в куче не достаточно памяти, то возвращает NULL
void *malloc_z( heap_t *heap, size_t size, int type );

// Освобождает зарезервированную память, на которую указывает 'ptr' в 'heap'.
// Если указатель указывает на память, которая не была выделена ранее
// malloc_z() или указатель 'ptr' равен 0, то ничего не происходит
void free_z( heap_t *heap, void *ptr );

//----------------------------------------------------------------------------
enum MCB_MARK {
	MARK_FREE = 0,
	MARK_SYSTEM = 1
};

// Получить общий свободный объем
//----------------------------------------------------------------------------
unsigned long getHeapFreeSize();

// Получить максимальный размер свободного блока
//----------------------------------------------------------------------------
unsigned long getHeapMaxSize( heap_t *heap);
unsigned long getHeapSize();

void mergeBlocks( heap_t *heap, heap_mcb* tptr, heap_mcb* xptr );
void shiftBlocks( heap_t *heap, heap_mcb* tptr, heap_mcb* fptr );
void defragHeap( heap_t *heap);

// добавляет к куче ещё один блок памяти
void heapadd( heap_t *heap, heap_mcb *addr, int size );
#endif /* __HEAP_Z_H */