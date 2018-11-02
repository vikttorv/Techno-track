#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "stack_2.cpp"
#include <unistd.h> 


int pop_unittest(struct stack* stk);
int do_pop_unittest( );


const int MAXTEST = 10000;


int main( ) 
{
    do_pop_unittest( );
    return EXIT_SUCCESS;
}


int do_pop_unittest( )
{
    srand((unsigned)time(0));
    SIZE_STACK stk_capacity = rand( ) % 200;
    bool is_canaries = (rand( ) % 2) == 1;
    bool is_control_sum = (rand( ) % 2) == 1;
    struct stack* stk = stack_get(stk_capacity, is_canaries, is_control_sum);


    for (int ind = 0; ind < stk_capacity; ind++)
    {
    	double new_elem = (double)(rand( ) % 5000 - 2500);
        stack_push(stk, new_elem);
    }


    for (int ind = 0; ind < stk_capacity + 10; ind++)
    {
        pop_unittest(stk);
    }
    free(stk);


    return SUCCESS;
}


int pop_unittest(struct stack* stk)
{
	assert(stk);
    

    double theory_pop_elem = (stk -> data)[stk -> count - 1];
    double pop_elem = stack_pop(stk);
    if (errno == STACK_EMPTY)
    {
    	printf("It should be  10 stack_empty!!!");
    	printf("Stack empty!!!\n");
    }
    else
    {
        printf("pop_elem = %g; expected: %g\n", pop_elem, theory_pop_elem);
        sleep(2);
    }
    return SUCCESS;
}
