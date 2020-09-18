/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
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
    "ateam",
    /* First member's full name */
    "Harry Bovik",
    /* First member's email address */
    "bovik@cs.cmu.edu",
    /* econd member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* Global Variable */
void *heap_listp;    /* Point the initial empty heap */
/* End of Global Variable */

/* Auxiliary Function */
static void *extend_heap(size_t words);		/* Extend heap with new free block */
static void *coalesce(void *bp);			/* Merge adjacent free block */
static void *find_fit(size_t asize);		/* Search the free list for a fit.(First adaptive search)*/
static void place(void *bp,size_t asize);	/* Place requested block into free block */
/* End of auxiliary function*/

/* Basic constants and macros */
#define WSIZE 4	    /* Word and header/footer size (bytes) */
#define DSIZE 8		/* Double word size (bytes) */
#define ALIGNMENT 8 /* single word (4) or double word (8) alignment */
#define CHUNKSIZE (1<<12) /* Extended heap by this amount(bytes) */

#define MAX(x,y) ((x) > (y)? (x) : (y))

/* Pack a size and allocated bit into a word */
#define PACK(size,alloc) ((size) | (alloc))

/* Read and wirte a word at address p */
#define GET(p)			(*(unsigned int *)(p))
#define PUT(p,val)      (*(unsigned int *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)		(GET(p) & ~0x7)
#define GET_ALLOC(p)	(GET(p) & 0x1)

/* Given block ptr bp,compute address of its header and footer */
#define HDRP(bp)		((char *)(bp) - WSIZE)
#define FTRP(bp)		((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)    /* Why it is DSIZE here? */
																	   /* From here,we know that size is len(hdr+data+ftr) */	

/* Given block ptr bp,compute address of next and previous blocks */
#define NEXT_BLKP(bp)	((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp)	((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
	/* Create the initial empty heap */
	if((heap_listp = mem_sbrk(4 * WSIZE)) == (void *)-1)
		return -1;
	
	PUT(heap_listp,0);			/* Alignment padding */
	PUT(heap_listp + (1 * WSIZE),PACK(DSIZE,1));		/* Prologue header */
	PUT(heap_listp + (2 * WSIZE),PACK(DSIZE,1));		/* Prologue header */
	PUT(heap_listp + (3 * WSIZE),PACK(0,1));			/* Epilogue header */
	heap_listp += (2 * WSIZE);	/* Real data block? Why the offset is 2 * WSIZE? */
								/* From picture 9-42 in page of 598,we have two prologue header and heap_listp point the second prologue header */
	
	/* Extended the empty heap with a free block of CHUNKSIZE bytes */ 
	if (extend_heap(CHUNKSIZE/WSIZE) == NULL)
		return -1;	
	return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
	size_t asize;		/* Adjusted block size */
	size_t extendsize;	/* Amount to extend heap if not fit */
	char *bp;

	/* Ignore spurious requests */
	if (size == 0)
		return NULL;

	/* Adjust block size to include overhead and alignment reqs. */
	if (size <= DSIZE)
		asize = 2 * DSIZE;		/* Because each chunk has a hdr and a ftr,so the minize size of a chunk is 2*DSiZE */
	else
		asize = (DSIZE) * ((size + (DSIZE) + (DSIZE - 1)) / DSIZE);


	/* Search the free list for a fit */
	if( (bp = find_fit(asize)) != NULL){
		place(bp,asize);
		return bp;
	}

	/* No fit found,Get more memory and place the block */
	extendsize = MAX(asize,CHUNKSIZE);
	if ((bp = extend_heap(extendsize/WSIZE)) == NULL)
		return NULL;
	place(bp,asize);
	return bp;
	
	/*
    int newsize = ALIGN(size + SIZE_T_SIZE);
    void *p = mem_sbrk(newsize);
    if (p == (void *)-1)
	return NULL;
    else {
        *(size_t *)p = size;
        return (void *)((char *)p + SIZE_T_SIZE);
    }
	*/
}

/*
 * mm_free - Freeing a block does nothing.
 * And coalesce it.
 * FIXME:Do we need to check whether this block has been free or not?
 */
void mm_free(void *ptr)
{
	size_t size = GET_SIZE(HDRP(ptr));

	/* Free block header and block footer */
	PUT(HDRP(ptr),PACK(size,0));
	PUT(FTRP(ptr),PACK(size,0));
	
	/* Merge */
	coalesce(ptr);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}


/* Auxiliary Function*/

/*
 * extend_heap - Extend heap with a new free chunk which's size is words.
 * The real size should be double words aligned.And the old epilogue block
 * will become this block's header and the last one block of this new chunk
 * will become the new epilogue.
 */
static void *extend_heap(size_t words)
{
	char *bp;
	size_t size;

	/* Allocate an even number of words to maintain alignment */
	size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
	if((long)(bp = mem_sbrk(size)) == -1)
		return NULL;

	/* Initialize free block header/footer and the epilogue header */
	PUT(HDRP(bp),PACK(size,0));			/* Free block header */
	PUT(FTRP(bp),PACK(size,0));			/* Free block footer */
	PUT(HDRP(NEXT_BLKP(bp)),PACK(0,1));	/* New epilogue header */
	
	/* Coalesce if the previous block was free */
	return coalesce(bp);
}

/*
 * coalesce - Merge free chunk by four routines expressed in picture 9-40.
 */
static void *coalesce(void *bp)
{
	size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
	size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
	size_t size = GET_SIZE(HDRP(bp));

	if (prev_alloc & next_alloc ) {		/* Case 1 */
		return bp;
	}
	else if (prev_alloc && !next_alloc) {	/* Case 2 */
		size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
		PUT(HDRP(bp),PACK(size,0));
		PUT(FTRP(bp),PACK(size,0));
	}
	else if (!prev_alloc && next_alloc) {	/* Case 3 */
		size += GET_SIZE(HDRP(PREV_BLKP(bp)));
		PUT(FTRP(bp),PACK(size,0));
		PUT(HDRP(PREV_BLKP(bp)),PACK(size,0));
		bp = PREV_BLKP(bp);
	}
	else {									/* Case 4 */
		size += GET_SIZE(HDRP(PREV_BLKP(bp))) + 
			GET_SIZE(FTRP(NEXT_BLKP(bp)));
		PUT(HDRP(PREV_BLKP(bp)),PACK(size,0));
		PUT(FTRP(NEXT_BLKP(bp)),PACK(size,0));
		bp = PREV_BLKP(bp);
	}
	return bp;
}

/*
 * find_fit - Search the free list for a fit block.
 * First adaptive search.
 */
static void *find_fit(size_t asize)
{
	char *bp = ((char *)heap_listp + DSIZE);
	size_t curSize,curAlloc;

	curAlloc = GET_ALLOC(HDRP(bp));
	curSize = GET_SIZE(HDRP(bp));
	
	/* Search until firt fit */
	while(curAlloc || curSize < asize)
	{
		if (!curSize)		/* Current block is epilogue. */
			return NULL;
		
		bp = NEXT_BLKP(bp);	/* Search next block. */
		curAlloc = GET_ALLOC(HDRP(bp));
		curSize = GET_SIZE(HDRP(bp));
	}

	return bp;
}

/*
 * place - Place request block into free block.
 * Ensure tha asize le currentSize.
 */
static void place(void *bp,size_t asize)
{
	size_t currentSize = GET_SIZE(HDRP(bp));

	if ((currentSize - asize) < (2 * DSIZE))
	{
		PUT(HDRP(bp),PACK(asize,1));	
		PUT(FTRP(bp),PACK(asize,1));
	}
	else
	{
		PUT(FTRP(bp),PACK(currentSize - asize,0));
		PUT(HDRP(bp),PACK(asize,1));
		PUT(FTRP(bp),PACK(asize,1));
		PUT(HDRP(NEXT_BLKP(bp)),PACK(currentSize - asize,0));
	}
}

/* End of Auxiliary Function */











