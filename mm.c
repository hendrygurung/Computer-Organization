/*
 * In this approach, we create malloc, free and realloc functions using an explicit 
 * list to handle the operations more efficiently. Pointers are used to track allocated 
 * and freed blocks of memory in the heap. They then allocate, reallocate and free the 
 * blocks in the list. The program has some conditions to find the best case scenario 
 * to improve efficiency. The program also has some helper functions to facilitate 
 * the process and to make the code more readable. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "SleepDeprivedStudent",
    /* First member's full name */
    "Hendry Gurung",
    /* First member's email address */
    "kgurung@email.wm.edu",
    /* Second member's full name (leave blank if none) */
    "Matt Bihl",
    /* Second member's email address (leave blank if none) */
    "mlbihl@email.wm.edu",
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* declare globals */
#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE 24
#define MINIMUM 16
#define OVERHEAD 8

/*finding max of 2 numbers */
#define MAX(x, y)   ((x) > (y)? (x) : (y))

/*pack a size and allocation bit into the word*/
#define PACK(size,alloc) ((size)|(alloc))

/*read and write words*/
#define GET(p)      (*(unsigned int*)(p))
#define PUT(p, val) (*(unsigned int*)(p) = (val))

/*read the size and allocated fields*/
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p)    (GET(p) & 0x1)

/*compute address of header and footer*/
#define HDRP(bp)    ((char*)(bp) - WSIZE)
#define FTRP(bp)    ((char*)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* compute address of next/previous blocks*/
#define NEXT_BLKP(bp) ((void *)(bp) + GET_SIZE(HDRP(bp)))
#define PREV_BLKP(bp) ((void *)(bp) - GET_SIZE(HDRP(bp) - WSIZE))

/* Address of free block's predecessor and successor on the segregated list */
#define NEXT_FREE(bp) (*(void **)(bp + WSIZE)) //***!!! this is where double pointers come into play
#define PREV_FREE(bp) (*(void **)(bp)) //***!!!

/*Empty heap and explicit list */
static char *heap_listp = NULL;
static char *freeListp = NULL;

/*helper functions */
static void *extend_heap(size_t words);
static void *coalesce(void *bp);
static void *find_fit(size_t size);
static void place(void *bp, size_t asize);
static void removeNode(void *bp);
static void insertBegin(void *bp);

/* 
 * mm_init - initialize the malloc package and create the heap
 */
int mm_init(void)
{
  /*Create the empty heap*/
  if ((heap_listp = mem_sbrk(2*MINIMUM)) == NULL) {
    return -1;
  }

  /*Allocate the memory needed */
  PUT(heap_listp, 0);
  PUT(heap_listp + (1 * WSIZE), PACK(MINIMUM, 1));
  PUT(heap_listp + (2 * WSIZE), PACK(0, 1));
  PUT(heap_listp + (3 * WSIZE), PACK(0, 1));
  PUT(heap_listp + MINIMUM, PACK(MINIMUM, 1));
  PUT(heap_listp + WSIZE + MINIMUM, PACK(0,1));

  freeListp = heap_listp + DSIZE;
  /*extend empty heap w/ block of CHUNKSIZE*/
  if (extend_heap(CHUNKSIZE / WSIZE) == NULL) {
    return -1;
  }
  return 0;
}

/*
* Calling extend_heap will extend the heap and insert the free block into 
* an appropriate spot on the list. 
*/
static void *extend_heap(size_t words) {

  char *bp;
  size_t size;
  /*allocate even number of words for alignment */
  size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
  if (size < MINIMUM)
  {
    size = MINIMUM;
  }

  /* extend the heap */
  if ((long)(bp = mem_sbrk(size)) == -1) {
    return NULL;
  }
  /*initialize free block header/footer */
  PUT(HDRP(bp), PACK(size, 0)); /*header*/
  PUT(FTRP(bp), PACK(size, 0)); /*footer*/
  PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); /*epilogue*/
  
  return coalesce(bp);
}

/* 
 * mm_malloc - Allocate a block by placing it in a free block. The 
 * heap extends if necessary. The blocks are padded with boundary tags
 * and their lengths conform to the alignment
 */
void *mm_malloc(size_t size)
{
    size_t asize; /* Adjusted block size */
    size_t extendsize;/* How much to extend by */
    char *bp; /*pointer to traverse */
    
    if(freeListp == 0){
        mm_init();
    }
    /* if no block, dont do anything */
    if (size == 0) {
      return NULL;
    }
    
    asize = MAX(ALIGN(size) + DSIZE, MINIMUM);
    
    /*see if the block will fit*/
    if ((bp = find_fit(asize)) != NULL) {
      place(bp, asize);
      return bp;
    }
    
    /* in the case the block will not fit, extend*/
    extendsize = MAX(asize, CHUNKSIZE);
    if ((bp = extend_heap(extendsize / WSIZE)) == NULL) {
      return NULL;
    }

    /* place the block in*/
    place(bp, asize);
    
    return bp;
}

/*
 * mm_free - Add the block to the list and coalesce it
 */
void mm_free(void *ptr)
{
  if(!ptr) return;
  /* block size*/
  size_t size = GET_SIZE(HDRP(ptr));
  
  if(freeListp == 0){
      mm_init();
  }
  /* flag for allocation */
  PUT(HDRP(ptr), PACK(size, 0));
  PUT(FTRP(ptr), PACK(size, 0));

  /* coalesce free block*/
  coalesce(ptr);
}

/*
* insert_node - Insert pointer to the beginning of the free list
*/

static void insertBegin(void *bp){

    /* if the prev is NULL, that means its at the front of the list and add it*/
    NEXT_FREE(bp) = freeListp;
    PREV_FREE(freeListp) = bp;
    PREV_FREE(bp)= NULL;
    freeListp = bp;
}

/*
* coalesce - Combine the adjacent free blocks. Then add the new free block into the
* the front of the list.
*/
static void *coalesce(void *bp) {
  /* get the sizes of the blocks */
  size_t prev_alloc;
  prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp))) || PREV_BLKP(bp) == bp;
  size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
  size_t size = GET_SIZE(HDRP(bp));
  
  /* merge the free blocks, different actions for different conditions */
  if (prev_alloc && !next_alloc) {
    size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
    removeNode(NEXT_BLKP(bp));
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
  }
  else if (!prev_alloc && next_alloc) {
    size += GET_SIZE(HDRP(PREV_BLKP(bp)));
    bp = PREV_BLKP(bp);
    removeNode(bp);
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
  }
  else if(!prev_alloc && !next_alloc){
    size += (GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(HDRP(NEXT_BLKP(bp))));
    removeNode(PREV_BLKP(bp));
    removeNode(NEXT_BLKP(bp));
    bp= PREV_BLKP(bp);
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
  }
  /* at the end put the block at the beginning*/
     insertBegin(bp); 
  return bp;
  }


/*
* mm_realloc - Reallocate the block, aligning it and putting the proper buffers. 
* Different actions will be performed when different conditions are fulfilled. 
* Adding these heuristics increases the efficiency greatly.
*/
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;

    int asize = MAX(ALIGN(size) + DSIZE, MINIMUM);
    
    /* in the case of null pointer*/
    if(!ptr){
        return mm_malloc(size);
    }

    /* in the case that the block is size 0*/
    if(size == 0){
        mm_free(ptr);
        return NULL;
    }

    /* in the case of the free block being very convenient*/
    if(!GET_ALLOC(HDRP(NEXT_BLKP(ptr)))){
        int combineSize = GET_SIZE(HDRP(ptr)) + GET_SIZE(HDRP(NEXT_BLKP(ptr)));
        if(asize <= combineSize){
            removeNode(NEXT_BLKP(ptr));
            PUT(HDRP(ptr), PACK(combineSize, 1));
            PUT(FTRP(ptr), PACK(combineSize, 1));
            place(ptr, asize);
            return ptr;
        }
    }

    void *newptr;
    size_t copySize;
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = GET_SIZE(HDRP(oldptr));
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}
/*
* place - places a block of asize bytes at the start of the list and
* split if needed (minimum block size)
*/
static void place(void *bp, size_t asize)
{
    size_t csize = GET_SIZE(HDRP(bp));   
    int reallocCheck = GET_ALLOC(HDRP(bp));
    /*block has enough room at the end for block*/
    if ((csize - asize) > (32)) { 

        if(!reallocCheck){
            removeNode(bp);
        }
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp), PACK(csize-asize, 0));
        PUT(FTRP(bp), PACK(csize-asize, 0));
        coalesce(bp);
    }
    /*in the case of no room     */
    else { 
        PUT(HDRP(bp), PACK(csize, 1));
        PUT(FTRP(bp), PACK(csize, 1));
        if(!reallocCheck){
            removeNode(bp);
        }
    }
}

/* 
 * find_fit - Find a fit for a block with asize bytes 
 */

static void *find_fit(size_t asize)
{
    void *bp;
    void *quickP = NULL;

    /* will find block with exact fit if possible*/;
    for (bp = freeListp; GET_ALLOC(HDRP(bp)) == 0; bp = NEXT_FREE(bp)) {
        if (asize <= (size_t)GET_SIZE(HDRP(bp))) {
            if (asize >= (size_t)(GET_SIZE(HDRP(bp)) - 16)){
            return bp;
        } else if (!quickP){
            quickP = bp;
        }
        }
    }
    if(quickP){
        return quickP;
    }
    return NULL; /* in the case of no fit */
}
/* 
 * removeNode - removes a block and then reconcatenates the list
 */
static void removeNode(void *bp){

    /*if theres a previous element, fix the list. If not move the pointer to the head*/
    PREV_FREE(NEXT_FREE(bp)) = PREV_FREE(bp);
    if (PREV_FREE(bp)){
        /*fixing the list*/
        NEXT_FREE(PREV_FREE(bp)) = NEXT_FREE(bp);
    } else{
        freeListp = NEXT_FREE(bp);
    }
}
