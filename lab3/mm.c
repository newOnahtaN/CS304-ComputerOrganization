/*
 * Nathan Owen, Malloc lab
 *
 * This is an intermediate implementation of malloc, realloc, and free, using the implementation of
 * a implicit free list. This approach is simplistic and will yield only a 84% performance index.
 * The context of this program is almost entirely drawn from CSAPP with just a few changes. The allocator uses next fit 
 * placement and boundary tag coalescing, rather than the prescribed first fit placement used in the examples in our book.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

team_t team = {
    /* Team name */
    "JSurf",
    /* First member's full name */
    "Nathan Owen",
    /* First member's email address */
    "ncowen@email.wm.edu",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* Basic constants and macros */
#define WSIZE       4       /* Word and header/footer size (bytes) */ //line:vm:mm:beginconst
#define DSIZE       8       /* Doubleword size (bytes) */
#define CHUNKSIZE  (1<<12)  /* Extend heap by this amount (bytes) */  //line:vm:mm:endconst 

#define MAX(x, y) ((x) > (y)? (x) : (y))  

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc)  ((size) | (alloc)) //line:vm:mm:pack

/* Read and write a word at address p */
#define GET(p)       (*(unsigned int *)(p))            //line:vm:mm:get
#define PUT(p, val)  (*(unsigned int *)(p) = (val))    //line:vm:mm:put

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)  (GET(p) & ~0x7)                   //line:vm:mm:getsize
#define GET_ALLOC(p) (GET(p) & 0x1)                    //line:vm:mm:getalloc

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)       ((char *)(bp) - WSIZE)                      //line:vm:mm:hdrp
#define FTRP(bp)       ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE) //line:vm:mm:ftrp

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp)  ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE))) //line:vm:mm:nextblkp
#define PREV_BLKP(bp)  ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE))) //line:vm:mm:prevblkp

/* Global variables */
static char *heap_listp = 0;  /* Pointer to first block */  
static char *nextFit_pointer; 


/* Function prototypes for internal helper routines */
static void *extend_heap(size_t words);
static void place(void *bp, size_t asize);
static void *find_fit(size_t asize);
static void *coalesce(void *bp);
static void printblock(void *bp); 
static void mm_check(int verbose);
static void checkblock(void *bp);

/* 
 * Initialize the malloc package as a whole. 
 */
int mm_init(void) 
{
  /* Create the initial empty heap */
  if ((heap_listp = mem_sbrk(4*WSIZE)) == (void *)-1) //line:vm:mm:begininit
    return -1;
  PUT(heap_listp, 0);                          /* Alignment padding */
  PUT(heap_listp + (1*WSIZE), PACK(DSIZE, 1)); /* Prologue header */ 
  PUT(heap_listp + (2*WSIZE), PACK(DSIZE, 1)); /* Prologue footer */ 
  PUT(heap_listp + (3*WSIZE), PACK(0, 1));     /* Epilogue header */
  heap_listp += (2*WSIZE);                     //line:vm:mm:endinit  

  nextFit_pointer = heap_listp;

  /* Extend the empty heap with a free block of CHUNKSIZE bytes */
  if (extend_heap(CHUNKSIZE/WSIZE) == NULL) 
    return -1;
  return 0;
}


/* 
 * Allocate a block by locating the next free block, and use the
 * extend heap function if needed. Each block is padded, and the size is adjusted 
 * accordingly.
 */
void *mm_malloc(size_t size) 
{
  size_t asize;      /* Adjusted block size */
  size_t extendsize; /* Amount to extend heap if no fit */
  char *bp;      

  if (heap_listp == 0){
    mm_init();
  }

  /* Ignore spurious requests */
  if (size == 0)
    return NULL;

  /* Adjust block size to include overhead and alignment reqs. */
  if (size <= DSIZE)                                        
    asize = 2*DSIZE;                                        
  else
    asize = DSIZE * ((size + (DSIZE) + (DSIZE-1)) / DSIZE); 

  /* Search the free list for a fit */
  if ((bp = find_fit(asize)) != NULL) {  
    place(bp, asize);                
    return bp;
  }

  /* No fit found. Get more memory and place the block */
  extendsize = MAX(asize,CHUNKSIZE);                 
  if ((bp = extend_heap(extendsize/WSIZE)) == NULL)  
    return NULL;                                 
  place(bp, asize);                               
  return bp;
} 


/* 
 * Free a block by clearing it and coalescing it.
 */

void mm_free(void *bp)
{
  if(bp == 0) 
    return;

  size_t size = GET_SIZE(HDRP(bp));

  if (heap_listp == 0){
    mm_init();
  }


  PUT(HDRP(bp), PACK(size, 0));
  PUT(FTRP(bp), PACK(size, 0));
  coalesce(bp);
}


/*
 * Boundary tag coalescing. Return ptr to coalesced block.
 * This function is the CSAPP implementation.
 */

static void *coalesce(void *bp) 
{
  size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
  size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
  size_t size = GET_SIZE(HDRP(bp));

  if (prev_alloc && next_alloc) {            /* Case 1 */
    return bp;
  }

  else if (prev_alloc && !next_alloc) {      /* Case 2 */
    size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size,0));
  }

  else if (!prev_alloc && next_alloc) {      /* Case 3 */
    size += GET_SIZE(HDRP(PREV_BLKP(bp)));
    PUT(FTRP(bp), PACK(size, 0));
    PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
    bp = PREV_BLKP(bp);
  }

  else {                                     /* Case 4 */
    size += GET_SIZE(HDRP(PREV_BLKP(bp))) + 
        GET_SIZE(FTRP(NEXT_BLKP(bp)));
    PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
    PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
    bp = PREV_BLKP(bp);
  }

  /* Make sure the nextFit_pointer isn't pointing into the free block */
  /* that we just coalesced */
  if ((nextFit_pointer > (char *)bp) && (nextFit_pointer < NEXT_BLKP(bp))) 
  nextFit_pointer = bp;

  return bp;
}


/*
 * This reallocates a block in place and extend the heap if we need to.
 * A very simple implementation based on CSAPP - malloc a new block, copy the old data, free
 * the block it belonged to and switch.
 */
void *mm_realloc(void *ptr, size_t size)
{
  size_t oldsize;
  void *newptr;

  /* If size == 0 then this is just free, and we return NULL. */
  if(size == 0) {
    mm_free(ptr);
    return 0;
  }

  /* If oldptr is NULL, then this is just malloc. */
  if(ptr == NULL) {
    return mm_malloc(size);
  }

  newptr = mm_malloc(size);

  /* If realloc() fails the original block is left untouched  */
  if(!newptr) {
    return 0;
  }

  /* Copy the old data. */
  oldsize = GET_SIZE(HDRP(ptr));
  if(size < oldsize) oldsize = size;
      memcpy(newptr, ptr, oldsize);

  /* Free the old block. */
  mm_free(ptr);

  return newptr;
}

/* 
 * Extend heap with free block and return its block pointer.
 */
static void *extend_heap(size_t words) 
{
  char *bp;
  size_t size;

  /* Allocate an even number of words to maintain alignment */
  size = (words % 2) ? (words+1) * WSIZE : words * WSIZE; 
  if ((long)(bp = mem_sbrk(size)) == -1)  
    return NULL;                                       

  /* Initialize free block header/footer and the epilogue header */
  PUT(HDRP(bp), PACK(size, 0));         /* Free block header */   
  PUT(FTRP(bp), PACK(size, 0));         /* Free block footer */   
  PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); /* New epilogue header */ 

  /* Coalesce if the previous block was free */
  return coalesce(bp);                                         
}

/* 
 * This will place block of an arbitrary amount of bytes at start of a
 * free block (*bp) and split if it has any portion within it that is the
 * minimum block size.
 */
static void place(void *bp, size_t asize)
{
  size_t csize = GET_SIZE(HDRP(bp));   

  if ((csize - asize) >= (2*DSIZE)) { 
    PUT(HDRP(bp), PACK(asize, 1));
    PUT(FTRP(bp), PACK(asize, 1));
    bp = NEXT_BLKP(bp);
    PUT(HDRP(bp), PACK(csize-asize, 0));
    PUT(FTRP(bp), PACK(csize-asize, 0));
  }
  else { 
    PUT(HDRP(bp), PACK(csize, 1));
    PUT(FTRP(bp), PACK(csize, 1));
  }
}

/* 
 * This method encapsulates the process for find the next free block in the
 * implicit free list. 
 */
static void *find_fit(size_t asize)
{
 
  /* Next fit search */
  char *oldnextFit_pointer = nextFit_pointer;

  /* Search from the nextFit_pointer to the end of list */
  for ( ; GET_SIZE(HDRP(nextFit_pointer)) > 0; nextFit_pointer = NEXT_BLKP(nextFit_pointer))
    if (!GET_ALLOC(HDRP(nextFit_pointer)) && (asize <= GET_SIZE(HDRP(nextFit_pointer))))
      return nextFit_pointer;

  /* search from start of list to old nextFit_pointer */
  for (nextFit_pointer = heap_listp; nextFit_pointer < oldnextFit_pointer; nextFit_pointer = NEXT_BLKP(nextFit_pointer))
    if (!GET_ALLOC(HDRP(nextFit_pointer)) && (asize <= GET_SIZE(HDRP(nextFit_pointer))))
      return nextFit_pointer;

  return NULL;  /* no fit found */
}



/*
 * Checks the block to ensure that it is aligned and that the header
 * matches the footer.
 */
static void checkblock(void *bp) 
{
  if ((size_t)bp % 8)
    printf("Error: %p is not doubleword aligned\n", bp);
  if (GET(HDRP(bp)) != GET(FTRP(bp)))
    printf("Error: header does not match footer\n");
}

/*
 * Used by mm_chech to print out a block
 *
static void printblock(void *bp) 
{
  size_t hsize, halloc, fsize, falloc;

  mm_check(0);
  hsize = GET_SIZE(HDRP(bp));
  halloc = GET_ALLOC(HDRP(bp));  
  fsize = GET_SIZE(FTRP(bp));
  falloc = GET_ALLOC(FTRP(bp));  

  if (hsize == 0) {
    printf("%p: EOL\n", bp);
    return;
  }

  printf("%p: header: [%p:%c] footer: [%p:%c]\n", bp, 
  hsize, (halloc ? 'a' : 'f'), 
  fsize, (falloc ? 'a' : 'f')); 
}*/


/* 
 * mm_check - Check the heap for consistency 
 * Checks the epilogue and prologue blocks for size and allocation bit.
 * Checks the 8-byte address alignment for each block in the free list.
 * Checks each free block to see if its next and previous pointers are 
 * within heap bounds.
 * Checks the consistency of header and footer size and allocation bits 
 * for each free block.
 *

void mm_check(int verbose) 
{
  char *bp = heap_listp;

  if (verbose)
  printf("Heap (%p):\n", heap_listp);
    
  
  // If first block's header's size or allocation bit is wrong, 
  // the prologue header is wrong 
  
    if ((GET_SIZE(HDRP(heap_listp)) != DSIZE) || !GET_ALLOC(HDRP(heap_listp)))
  printf("Bad prologue header\n");
    checkblock(heap_listp);

  // Print the stats of every free block in the free list 
    for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
        if (verbose) 
            printblock(bp);
        checkblock(bp);
    }

    // Print the stats of the last block in the heap 
    if (verbose)
        printblock(bp);

 // If last block's header's size or allocation bit is wrong, 
 // the epilogue's header is wrong

    if ((GET_SIZE(HDRP(bp)) != 0) || !(GET_ALLOC(HDRP(bp))))
  printf("Bad epilogue header\n");
}*/
















