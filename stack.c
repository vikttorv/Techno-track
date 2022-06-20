/* Stack_2 V.T.0.1*/
/* #include <assert.h>; <errno.h>; <stdlib.h> */
typedef unsigned long long SIZE_STACK;
const int SUCCESS = 0;
const int ERROR_OF_SIZE = -1;
const int STACK_EMPTY = -2;
const double ERROR_VALUE = -666666;
const double POISON = -666;
const char CANARY_VALUE = '7';
struct stack
{
	char canary1; // to find mistakes
    SIZE_STACK count;
    double* data; 
    SIZE_STACK capacity;
    bool protection[2]; // first for canaries, second for control sum.
    SIZE_STACK control_sum; // to find mistakes 
    char canary2; // to find mistakes
};


bool stack_ok(struct stack* stk)
{
	bool is_ok = stk && 0 <= stk -> count && stk -> count <= stk -> capacity && stk -> data;


    if ((stk -> protection)[0])
        is_ok = is_ok && stk -> canary1 == CANARY_VALUE && stk -> canary1 == CANARY_VALUE;
    if ((stk -> protection)[0])
    {
    	SIZE_STACK real_cntr_sum = 0;
    	for (SIZE_STACK ind = 0; ind < stk -> capacity; ind++)
        {
            real_cntr_sum += (stk -> data)[ind] * (ind + 1);
        }   
        is_ok = is_ok && stk -> control_sum == real_cntr_sum; 
    }
    return (is_ok);
}


int stack_dump(struct stack* stk)
{
    printf("stack \"stk\" [%p]\n{\n", stk);
    printf("capacity = %llu;\n", stk -> capacity);
    printf("count = %llu;\n", stk -> count);
    printf("data[%llu] [%p]", stk -> capacity, stk -> data);
    SIZE_STACK ind = 0;
    for (; ind < stk -> count; ind++)
    {
        printf("*  [%llu]: %g\n", ind, (stk -> data)[ind]); // * means that this element lies in stack.
    }
    for (;ind < stk -> capacity; ind++)
    {
        printf("   [%llu]: %g\n", ind, (stk -> data)[ind]);
    }
    printf("}\n");


    printf("stack_protection:\n [0] = %d;\n [1] = %d\n", (int)(stk -> protection)[0], (int)(stk -> protection)[1]);

    if ((stk -> protection)[0])
    {
        printf("canary1 == %c; expected: %c\n", stk -> canary1, CANARY_VALUE);
        printf("canary2 == %c; expected: %c\n", stk -> canary2, CANARY_VALUE);
    }


    if ((stk -> protection)[1])
    {
    	SIZE_STACK real_cntr_sum = 0;
    	for (SIZE_STACK ind = 0; ind < stk -> capacity; ind++)
        {
            real_cntr_sum += (stk -> data)[ind] * (ind + 1);
        } 
        printf("real control sum = %llu, expected: %llu", real_cntr_sum, stk -> control_sum);  
    }   
    return SUCCESS;
}


struct stack* stack_get(SIZE_STACK stack_capacity, bool canaries = false, bool ctrl_sum = false)
{
// Pointing canaries you can find more mistakes, but it will be slower.
// ctrl_sum include control_sum. The effect of it is similar effect of canaries, but it is much stronger.
	if (stack_capacity <= 0)
	{
		errno = ERROR_OF_SIZE;
	    return nullptr;
	}


    struct stack* stk = (struct stack*)calloc(1, 3 * sizeof(SIZE_STACK) + sizeof(double*) + 
    	2 * sizeof(char) + 2 * sizeof(bool));
    stk -> data = (double*)calloc(stack_capacity, sizeof(double));
    stk -> capacity = stack_capacity;
    stk -> canary1 = CANARY_VALUE;
    stk -> canary2 = CANARY_VALUE;
    stk -> control_sum = 0;


    if (canaries == true)
    {
        (stk -> protection)[0] = true;
    }
    else
    {
        (stk -> protection)[0] = false;
    }
    if (ctrl_sum == true)
    {
        for (SIZE_STACK ind = 0; ind < stk -> capacity; ind++)
        {
            stk -> control_sum += (stk -> data)[ind] * (ind + 1);
        }
    }  
    else
    {
        (stk -> protection)[1] = false;
    }


    errno = SUCCESS;
    return(stk);
}


int stack_push(struct stack* stk, double elem)
{
	if (!stack_ok(stk))
	{
        stack_dump(stk);
        assert(!"stack_push");
	}


    int become_larger = 0; // It is used to understand when stack becomes larger.
    if (stk -> count >= stk -> capacity)
    {
        double* tmp_stack = (double*)calloc(stk -> capacity, sizeof(double));
        for (SIZE_STACK ind = 0; ind < stk -> capacity; ind++)
        {
            tmp_stack[ind] = (stk -> data)[ind];
        }
        free(stk -> data);


        stk -> capacity = (stk -> capacity) * 2; //Stack becomes 2 times larger.
        stk -> data = (double*)calloc(stk -> capacity, sizeof(double)); 
        SIZE_STACK tmp_capacity = (stk -> capacity) / 2;// less operations for CPU
        for (SIZE_STACK ind = 0; ind < tmp_capacity; ind++)
        {
            (stk -> data)[ind] = tmp_stack[ind];
        }
        free(tmp_stack);
        become_larger = 1;
    }
    

    stk -> data[(stk -> count)++] = elem;


    if (stk -> protection[1])
    {
        stk -> control_sum += elem *  (stk -> count + 1);
    }


    if (!stack_ok(stk))
	{
        stack_dump(stk);
        assert(!"stack_push");
	}
    return become_larger;
}


double stack_pop(struct stack* stk)
{
	if (!stack_ok(stk))
	{
        stack_dump(stk);
        assert(!"stack_pop");
	}


    if (stk -> count == 0)
    {
        errno = STACK_EMPTY;
        return ERROR_VALUE;
    }


    (stk -> count)--;
    

    if (stk -> protection[1])
    {
        stk -> control_sum -= (stk -> data)[stk -> count - 1] *  (stk -> count - 1);
    }
    if (!stack_ok(stk))
	{
        stack_dump(stk);
        assert(!"stack_push");
	}


    errno = SUCCESS;
    return (stk -> data[(stk -> count)]);
}


int stack_printf(struct stack* stk)
{
	if (!stack_ok(stk))
	{
        stack_dump(stk);
        assert(!"stack_pop");
	}


    for (SIZE_STACK ind = 0; ind < stk -> count; ind++)
    {
        printf("%g\n", stk -> data[ind]);
    }
    putchar('\n');


    if (!stack_ok(stk))
	{
        stack_dump(stk);
        assert(!"stack_pop");
	}
    return SUCCESS;
} 


int stack_clear(struct stack* stk)
{
	if (!stack_ok(stk))
	{
        stack_dump(stk);
        assert(!"stack_pop");
	}


	stk -> count = 0;
	return SUCCESS;
}


SIZE_STACK stack_size(struct stack* stk)
{
	if (!stack_ok(stk))
	{
        stack_dump(stk);
        assert(!"stack_pop");
	}


	return stk -> count;
}


SIZE_STACK stack_capacity(struct stack* stk)
{
	if (!stack_ok(stk))
	{
        stack_dump(stk);
        assert(!"stack_pop");
	}


    return stk -> capacity;
}


double stack_top(struct stack* stk)
{
	if (!stack_ok(stk))
	{
        stack_dump(stk);
        assert(!"stack_pop");
	}


    if (stk -> count == 0)
    {
        errno = STACK_EMPTY;
        return ERROR_VALUE;
    }


    errno = SUCCESS;
    return (stk -> data)[stk -> count - 1];
}


