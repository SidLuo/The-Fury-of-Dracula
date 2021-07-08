//
//  COMP1927 Assignment 1 - Vlad: the memory allocator
//  allocator.c ... implementation
//
//  Created by Liam O'Connor on 18/07/12.
//  Modified by John Shepherd in August 2014, August 2015
//  Copyright (c) 2012-2015 UNSW. All rights reserved.
//
//  Assignment 1 1927 16s2
//  Modified by Guozhao Luo (Sid) (z5096182) on 26/08/2016.

#include "allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define FREE_HEADER_SIZE  sizeof(struct free_list_header)  
#define ALLOC_HEADER_SIZE sizeof(struct alloc_block_header)  
#define MAGIC_FREE     0xDEADBEEF
#define MAGIC_ALLOC    0xBEEFDEAD

#define BEST_FIT       1
#define WORST_FIT      2
#define RANDOM_FIT     3

typedef unsigned char byte;
typedef u_int32_t vsize_t;
typedef u_int32_t vlink_t;
typedef u_int32_t vaddr_t;

typedef struct free_list_header {
   u_int32_t magic;  // ought to contain MAGIC_FREE
   vsize_t size;     // # bytes in this block (including header)
   vlink_t next;     // memory[] index of next free block
   vlink_t prev;     // memory[] index of previous free block
} free_header_t;

typedef struct alloc_block_header {
   u_int32_t magic;  // ought to contain MAGIC_ALLOC
   vsize_t size;     // # bytes in this block (including header)
} alloc_header_t;

// Global data

static byte *memory = NULL;   // pointer to start of allocator memory
static vaddr_t free_list_ptr; // index in memory[] of first block in free list
static vsize_t memory_size;   // number of bytes malloc'd in memory[]
static u_int32_t strategy;    // allocation strategy (by default BEST_FIT)

// Private functions

static void vlad_merge();

// Converts offset to pointers
static byte * itop(vlink_t offset)
{
   byte * ptr;
   ptr = memory + offset;
   return ptr;
}

// Converts pointer to offset
static vlink_t ptoi(void * ptr) 
{
   vlink_t offset;
   offset = (byte *)ptr - memory;
   return offset;
}

// Input: size - number of bytes to make available to the allocator
// Output: none
// Precondition: Size >= 1024
// Postcondition: `size` bytes are now available to the allocator
//
// (If the allocator is already initialised, this function does nothing,
//  even if it was initialised with different size)

void vlad_init(u_int32_t size)
{
	if (memory == NULL) {
	if ((size != 0) && !(size & (size - 1))) memory = (byte *)malloc (size);
	else {
		u_int32_t ini_size = size;
		for (size = 1024; size < ini_size; size *= 2);
		memory = (byte *)malloc (size);
	}
	if (memory == NULL) {
		fprintf(stderr, "vlad_init: Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	free_list_ptr = 0;
	free_header_t * free = (free_header_t *)memory;
	free -> magic = MAGIC_FREE;
	free -> size = size;
	free -> next = free_list_ptr;
	free -> prev = free_list_ptr;
   memory_size = size;
   strategy = BEST_FIT;
	}
}


// Input: n - number of bytes requested
// Output: p - a pointer, or NULL
// Precondition: n < size of largest available free block
// Postcondition: If a region of size n or greater cannot be found, p = NULL 
//                Else, p points to a location immediately after a header block
//                      for a newly-allocated region of some size >= 
//                      n + header size.

void *vlad_malloc(u_int32_t n)
{
	u_int32_t ini_n = n;
   for (n = 8; n < ini_n; n += 4);

	free_header_t * curr = (free_header_t *)itop(free_list_ptr);
	
	if (curr -> magic != MAGIC_FREE) {
		fprintf(stderr, "vald_alloc: Memory corruption\n");
		exit(EXIT_FAILURE);
	}
	
	vsize_t alloc_size = n + ALLOC_HEADER_SIZE;
	vsize_t threshold = alloc_size + 2 * FREE_HEADER_SIZE;
	
	free_header_t * available = (free_header_t *)itop(free_list_ptr);

	if (curr -> next == ptoi(curr)) {
		if (curr -> size < threshold) return NULL; //can't fit
		else available = curr; //one free space that fits
	} else {
		vsize_t best = memory_size; //set best to the largest free space

		if (curr -> size >= alloc_size) {
			best = curr -> size - alloc_size;
			available = curr;
		}
		curr = (free_header_t *)itop(curr -> next);
		while (ptoi(curr) != free_list_ptr) {
			if (curr -> magic != MAGIC_FREE) {
				fprintf(stderr, "vald_alloc: Memory corruption\n");
				exit(EXIT_FAILURE);
			}
			if (curr -> size >= alloc_size) {
				// if size can fit and smaller than best
				if ((curr -> size - alloc_size) < best) {
					best = curr -> size - alloc_size;
					available = curr;
				}
			}
			curr = (free_header_t *)itop(curr -> next);
		}
		if (best == memory_size) return NULL; //didn't find a free space
	}

	alloc_header_t * chosen_ptr = (alloc_header_t *)available;
	vlink_t ava_offset = ptoi(available);

	if (available -> size >= threshold) { //split
		free_header_t * new_free = (free_header_t *)itop(ava_offset + alloc_size);
		new_free -> magic = MAGIC_FREE;
		new_free -> size = available -> size - alloc_size;

		// if this is the last free space
		if (available -> next == ptoi(available))
			new_free -> next = new_free -> prev = ptoi(new_free);
		else {
			new_free -> next = available -> next;
			new_free -> prev = available -> prev;
			((free_header_t *)(itop)(available -> prev)) -> next = ptoi(new_free);
			((free_header_t *)(itop)(available -> next)) -> prev = ptoi(new_free);
		}
		if (ptoi(available) == free_list_ptr)
			free_list_ptr = ptoi(new_free);  // set free_list_ptr
	} else {
		alloc_size = available -> size;
		((free_header_t *)(itop)(available -> prev)) -> next = available -> next;
		((free_header_t *)(itop)(available -> next)) -> prev = available -> prev;
		if (ptoi(available) == free_list_ptr) 
			free_list_ptr = available -> next; // set free_list_ptr
	}

	chosen_ptr -> magic = MAGIC_ALLOC;
	chosen_ptr -> size = alloc_size;

   return ((void*)chosen_ptr + ALLOC_HEADER_SIZE);
}


// Input: object, a pointer.
// Output: none
// Precondition: object points to a location immediately after a header block
//               within the allocator's memory.
// Postcondition: The region pointed to by object has been placed in the free
//                list, and merged with any adjacent free blocks; the memory
//                space can be re-allocated by vlad_malloc

void vlad_free(void *object)
{
	alloc_header_t * alloc = (alloc_header_t *)(object - ALLOC_HEADER_SIZE);
	vlink_t new = ptoi(alloc);
	if (new < 0) {
		fprintf(stderr, "vlad_free: Attempt to free via invalid pointer\n");
		exit(EXIT_FAILURE);
	}
	if (alloc->magic != MAGIC_ALLOC) {
		fprintf(stderr, "vlad_free: Attempt to free non-allocated memory\n");
		exit(EXIT_FAILURE);
	}
	
	vsize_t size = alloc -> size;
	
	free_header_t * free = (free_header_t *)itop(new);
	free -> magic = MAGIC_FREE;
	free -> size = size;

	free_header_t * curr = (free_header_t *)itop(free_list_ptr);
	while (curr -> next != free_list_ptr) {
		if (curr -> magic != MAGIC_FREE) {
			fprintf(stderr, "vald_alloc: Memory corruption\n");
			exit(EXIT_FAILURE);
		}
		if ((curr -> next > new) && (ptoi(curr) < new)) break;
		curr = (free_header_t *)itop(curr -> next);
	}

	// only one free space left and it's after new free region
	if ((curr -> next == ptoi(curr)) && (ptoi(curr) > new)) {
		free -> next = free -> prev = curr -> next;
		curr -> prev = curr -> next = new;
	} else {
		((free_header_t *)itop(curr -> next)) -> prev = new;
		free -> next = curr -> next;
		free -> prev = ptoi(curr);
		curr -> next = new;
	}

	//  if the new free region has the lowest memory address
	if (free -> prev > ptoi(free)) free_list_ptr = new;

   vlad_merge();
}

// Input: current state of the memory[]
// Output: new state, where any adjacent blocks in the free list
//            have been combined into a single larger block; after free,
//            there should be no region in the free list whose next
//            reference is to a location just past the end of the region

static void vlad_merge()
{
	free_header_t * curr = (free_header_t *)itop(free_list_ptr);
	while (curr -> next != free_list_ptr) {
		if (curr -> magic != MAGIC_FREE) {
			fprintf(stderr, "vald_alloc: Memory corruption\n");
			exit(EXIT_FAILURE);
		}
		if (curr -> next == ptoi(curr) + curr -> size) {
			free_header_t *temp = (free_header_t *)itop(curr -> next);
			curr -> next = temp -> next;
			((free_header_t *)(itop)(temp -> next)) -> prev = temp -> prev;
			curr -> size += temp -> size;
			continue;
		}
		curr = (free_header_t *)itop(curr -> next);
	}
}

// Stop the allocator, so that it can be init'ed again:
// Precondition: allocator memory was once allocated by vlad_init()
// Postcondition: allocator is unusable until vlad_int() executed again

void vlad_end(void)
{
   free (memory);
	memory = NULL;
	memory_size = 0;
}


// Precondition: allocator has been vlad_init()'d
// Postcondition: allocator stats displayed on stdout

void vlad_stats(void)
{
	printf("free_list_ptr: %d\n", free_list_ptr);
   free_header_t *curr = (free_header_t *) itop(free_list_ptr);
	int i;
   for (i = 0; i < 5; i++) {
      // printf("%d")
      printf("\tOffset index %d, size %d, region %s, next %d, prev %d\n", 
				ptoi(curr), curr->size, 
				(curr->magic == MAGIC_FREE) ? "FREE" : "ALLOC", 
				curr->next, curr->prev);
		printf("\tCurrent address: %p\n", curr);
      curr = (free_header_t *) itop(curr->next);
   }
	alloc_header_t *header = (alloc_header_t *) memory;
	printf("\tCurrent address: %p\n", header);
	printf("\tOffset index %d, size %d, region %s\n", 
			ptoi(header), header->size, (header->magic == MAGIC_ALLOC) ? "ALLOC" : "FREE");
}


