// stack.c ... simple Stack of Strings
// Written by John Shepherd, September 2015

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Stack.h"

typedef struct Node *Link;

typedef struct Node {
	int val;
	Link  next;
} Node;
	
typedef struct StackRep {
	Link  top;
	int size;
} StackRep;

// Function signatures

Stack newStack();
void  dropStack(Stack);
void  pushOnto(Stack, int);
int popFrom(Stack);
int   emptyStack(Stack);
void  showStack(Stack s);

static Link newNode(int);
static void disposeNode(Link);


// newStack()
// - create an initially empty Stack
Stack newStack()
{
	Stack new = malloc(sizeof(StackRep));
	assert(new != NULL);
	new->top = NULL;
	new->size = 0;
	return new;
}

// disposeStack(Stack)
// - clean up memory associated with Stack
void dropStack(Stack s)
{
	if (s == NULL) return;
	Link next, curr = s->top;
	while (curr != NULL) {
		next = curr->next;
		disposeNode(curr);	
		curr = next;
	}
}

// pushOnto(Stack,Str)
// - add Item to top of Stack
void pushOnto(Stack s, int item)
{
	Link new = newNode(item);
	new->next = s->top;
	s->top = new;
	s->size++;
}

// popFrom(Stack)
// - return int from top of Stack
int popFrom(Stack s)
{
	assert (s != NULL && s->top != NULL);
    int item = s->top->val;
	Link old = s->top;
	s->top = old->next;
	free(old);
	s->size--;
	return item;
}

// emptyStack(Stack)
// - check whether Stack is empty
int stackCard(Stack s)
{
	return s->size;
}

// Helper functions
static Link newNode(int item)
{
	Link new = malloc(sizeof(Node));
	assert(new != NULL);
	new->val = item;
	new->next = NULL;
	return new;
}

static void disposeNode(Link curr)
{
	assert(curr != NULL);
	free(curr);
}

//Stack to array, top first
int *stacktoa(Stack s) {
	int *values = malloc(stackCard(s) * sizeof(int));
	Link curr = s->top;
	for (int i= 0; curr != NULL; curr = curr->next, i++) {
		values[i] = curr->val;
	}
	return values;
}
