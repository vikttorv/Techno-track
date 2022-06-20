#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "stack_2.cpp"
#include <unistd.h> 


int push_unittest(struct stack* stk);
int do_push_unittest( );


const int MAXTEST = 10000;


int main( ) 
{
    do_push_unittest( );
    return EXIT_SUCCESS;
}


int do_push_unittest( )
{
    srand((unsigned)time(0));
    SIZE_STACK stk_capacity = rand( ) % 5000;
    bool is_canaries = (rand( ) % 2) == 1;
    bool is_control_sum = (rand( ) % 2) == 1;
    struct stack* stk = stack_get(stk_capacity, is_canaries, is_control_sum);


	srand((unsigned)time(0));
    for (int ind = 0; ind < 10000; ind++)
    {
        push_unittest(stk);
    }
    free(stk);


    return SUCCESS;
}


int push_unittest(struct stack* stk)
{
	assert(stk);


    double new_elem = (double)(rand( ) % 5000 - 2500);
    int code_push = 0xBEDA;
    SIZE_STACK old_capacity = stk -> capacity;
    code_push = stack_push(stk, new_elem);


    if (code_push)
    {
    	printf("New_capacity = %llu; expected capacity = %llu\n", stk -> capacity, old_capacity * 2);
        printf("Expected value: %f; new_elem = %f\n", new_elem, (stk -> data)[(stk -> count) - 1]);
        sleep(2); 
        return SUCCESS;
    }


    printf("Expected value: %.f; new_elem = %.f\n", new_elem, (stk -> data)[(stk -> count) - 1]);
    sleep(2); 
    return SUCCESS; 
}
