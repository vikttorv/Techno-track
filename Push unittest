#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "stack_2.cpp"
#include <unistd.h> 


int push_unittest(struct stack* stk);
int do_push_unittest(struct stack* stk);


const int MAXTEST = 10000;


int main( ) 
{
    struct stack* test_stack = stack_get(4);
    do_push_unittest(test_stack);
    free(test_stack);
    return EXIT_SUCCESS;
}


int do_push_unittest(struct stack* stk)
{
    assert(stk);


	srand((unsigned)time(0));
    for (int ind = 0; ind < 10000; ind++)
    {
        push_unittest(stk);
    }
    return SUCCESS;
}


int push_unittest(struct stack* stk)
{
	assert(stk);


    double new_elem = (double)((rand( ) - 20000) / 1000);
    int code_push = 0xBEDA;
    SIZE_STACK old_capacity = stk -> capacity;
    code_push = stack_push(stk, new_elem);


    if (code_push)
    {
    	printf("New_capacity = %llu; expected capacity = %llu\n", stk -> capacity, old_capacity * 2);
        printf("Expected value: %.f; new_elem = %.f\n", new_elem, (stk -> data)[(stk -> count) - 1]);
        sleep(2); 
        return SUCCESS;
    }


    printf("Expected value: %.f; new_elem = %.f\n", new_elem, (stk -> data)[(stk -> count) - 1]);
    sleep(2); 
    return SUCCESS; 
}
