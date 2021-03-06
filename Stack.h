// stack.h ... interface to simple Stack of Strings
// Written by John Shepherd, September 2015

#ifndef STACK_H
#define STACK_H

typedef struct StackRep *Stack;

// Function signatures

Stack newStack();
void  dropStack(Stack);
void  pushOnto(Stack,int);
int popFrom(Stack);
int   emptyStack(Stack);
void  showStack(Stack s);
int stackCard(Stack s);
int *stacktoa(Stack s);

#endif