// struct processor.cpp V.T.0.1
// #include "stack2.cpp"; <assert.h>; <errno.h>;
// <stdlib.h>; <stdio.h>; <math.h>; <time.h>; <unistd.h>; 
#include "stack2.cpp"
#include "proc_consts.h"


const int NUM_REGSTR = 10;
const LLU LEN_RAM = 10000;


enum proc_errors 
{
    GET_STACK_ERROR = -100,
    // push errors
    ERROR_PUSH,
    OPERATIVE_IND_NOT_INTEGER_PUSH,
    OPERATIVE_IND_NOT_INTEGER_RGSTR_PUSH,
    // pop errors
    ERROR_POP,
    OPERATIVE_IND_NOT_INTEGER_POP,
    OPERATIVE_IND_NOT_INTEGER_RGSTR_POP,
    // jump errors
    INCORRECT_LABEL_JMP, 
    INCORRECT_LABEL_JA,
    INCORRECT_LABEL_JB,
    INCORRECT_LABEL_JC,
    INCORRECT_LABEL_JAE,
    INCORRECT_LABEL_JBE,
    INCORRECT_LABEL_JNE,
    // other errors
    NEGATIVE_SQRT_ELEM,
    EMERGENCY_STOP,
    NULL_FAILURE
};


struct processor
{
    double registers[NUM_REGSTR];
    struct stack* stk;
    double ram[LEN_RAM];
};


struct processor* get_proc( )
{
    struct processor* proc = (struct processor*)calloc(1, sizeof(struct processor));
    if ((proc -> stk = stack_get(START_LEN)) == nullptr)
    {
        errno = GET_STACK_ERROR;
        return nullptr;
    }


    return proc;
}


bool processor_ok(struct processor* proc)
{
    bool is_ok = proc && stack_ok(proc -> stk) && proc -> registers && proc -> ram;


    return is_ok;
}


int processor_dump(struct processor* proc)
{
    if (!proc)
    {
        printf("proc == nullptr!!!\n");
        return SUCCESS;
    }
    
    
    printf("processor \"proc\" [%p]\n{\n", proc);
    printf("proc -> stk:\n\n");
    stack_dump(proc -> stk);


    if (!(proc -> registers))
    {
    	printf("proc -> registers == nullptr!!!\n");
    	return SUCCESS;
    }
    printf("proc -> registers:\n");
    int ind = 0;
    printf("NUM_REGSTR = %d\n", NUM_REGSTR);
    for (; ind < NUM_REGSTR; ind++)
    {
        printf("registers[%d] = %g\n", ind, proc -> registers[ind]);
    } 


    if (!(proc -> ram))
    {
    	printf("proc -> ram == nullptr!!!\n");
    	return SUCCESS;
    }
    printf("proc -> ram:\n");
    printf("LEN_RAM = %llu\n", LEN_RAM);
    for (ind = 0; ind < NUM_REGSTR; ind++)
    {
        printf("ram[%d] = %g\n", ind, proc -> ram[LEN_RAM]);
    } 
    printf("}\n");


    return SUCCESS;
}


int do_push(struct processor* proc)
// do command push, return code of error.
{
    if (!processor_ok(proc))
	{
        processor_dump(proc);
        assert(!"Processor_fail(do_push_command)");
	}


    double what_elem = stack_pop(proc -> stk);
    int what_push = stack_pop(proc -> stk);
    if (what_push == ELEM)
    {
        stack_push(proc -> stk, what_elem);
    }
    else if (what_push == REGSTR)
    {
    	stack_push(proc -> stk, (proc -> registers)[(int)what_elem]);
    }
    else 
    {    
        processor_dump(proc);
        return ERROR_PUSH;
    }


    if (!processor_ok(proc))
	{
        processor_dump(proc);
        assert(!"Processor_fail(do_push_command)");
	}
	return SUCCESS;
}


int do_push_ram(struct processor* proc)
// do command push_ram, return code of error.
{
    if (!processor_ok(proc))
	{
        processor_dump(proc);
        assert(!"Processor_fail(do_push_ram_command)");
	}


    double what_elem = stack_pop(proc -> stk);
    int what_push = stack_pop(proc -> stk);


    if (what_push == ELEM)
    {
        if (what_elem != (int)what_elem)
        {
            return OPERATIVE_IND_NOT_INTEGER_PUSH;
        }
        stack_push(proc -> stk, (proc -> ram)[(int)what_elem]);
    }
    else if (what_push == REGSTR)
    {
        double what_rgstr_value = (proc -> registers)[(int)what_elem];
        if ((int)what_rgstr_value != what_rgstr_value)
        {
            return OPERATIVE_IND_NOT_INTEGER_RGSTR_PUSH;
        }


        stack_push(proc -> stk, (proc -> ram)[(int)what_rgstr_value]);
    }
    else 
    {    
        processor_dump(proc);
        return ERROR_PUSH;
    }


    if (!processor_ok(proc))
	{
        processor_dump(proc);
        assert(!"Processor_fail(do_push_ram_command)");
	}
	return SUCCESS;
}


int do_pop_regstr(struct processor* proc)
// do command pop_regstr, return code of error.
{
    if (!processor_ok(proc))
	{
        processor_dump(proc);
        assert(!"Processor_fail(do_pop_regstr_command)");
	}
    if ((proc -> stk) -> count <= 0)
    {
        return EMERGENCY_STOP;
    }


    int what_rgstr = stack_pop(proc -> stk);
    (proc -> registers)[what_rgstr] = stack_pop(proc -> stk);


    if (!processor_ok(proc))
	{
        processor_dump(proc);
        assert(!"Processor_fail(do_pop_regstr_command)");
	}
    return SUCCESS;
}


int do_pop_ram(struct processor* proc)
// do command pop_ram, return code of error.
{
    if (!processor_ok(proc))
	{
        processor_dump(proc);
        assert(!"Processor_fail(do_pop_ram_command)");
	}
    if ((proc -> stk) -> count <= 0)
    {
        return EMERGENCY_STOP;
    }

    double what_elem = stack_pop(proc -> stk);
    int what_pop = stack_pop(proc -> stk);


    if (what_pop == ELEM)
    {
        if (what_elem != (int)what_elem)
        {
            return OPERATIVE_IND_NOT_INTEGER_POP;
        }
        (proc -> ram)[(int)what_elem] = stack_pop(proc -> stk);
    }
    else if (what_pop == REGSTR)
    {
        double what_rgstr_value = (proc -> registers)[(int)what_elem];
        if ((int)what_rgstr_value != what_rgstr_value)
        {
            return OPERATIVE_IND_NOT_INTEGER_RGSTR_POP;
        }


        (proc -> ram)[(int)what_rgstr_value] = stack_pop(proc -> stk);
    }
    else 
    {    
        processor_dump(proc);
        return ERROR_POP;
    }


    if (!processor_ok(proc))
	{
        processor_dump(proc);
        assert(!"Processor_fail(do_pop_ram_command)");
	}
    return SUCCESS;
}


int do_sqrt(struct processor* proc)
// do command push, return code of error.
{
    if (!processor_ok(proc))
    {
        processor_dump(proc);
        assert(!"Processor_fail(do_sqrt_command)");
    }


    double what_elem = stack_pop(proc -> stk);
    int what_push = stack_pop(proc -> stk);
    if (what_push == ELEM)
    {
        if (what_elem < 0)
        {
            return NEGATIVE_SQRT_ELEM;
        }


        what_elem = sqrt(what_elem);
        stack_push(proc -> stk, what_elem);
    }
    else if (what_push == REGSTR)
    {
        if ((proc -> registers)[(int)what_elem] < 0)
        {
            return NEGATIVE_SQRT_ELEM;
        }

        what_elem = sqrt((proc -> registers)[(int)what_elem]);
        stack_push(proc -> stk, what_elem);
    }
    else 
    {    
        processor_dump(proc);
        return ERROR_PUSH;
    }


    if (!processor_ok(proc))
    {
        processor_dump(proc);
        assert(!"Processor_fail(do_sqrt_command)");
    }
    return SUCCESS;
}


int do_sqrt_ram(struct processor* proc)
// do command sqrt_ram, return code of error.
{
    if (!processor_ok(proc))
    {
        processor_dump(proc);
        assert(!"Processor_fail(do_sqrt_ram_command)");
    }


    int what_elem = stack_pop(proc -> stk);
    if ((proc -> ram)[what_elem] < 0)
    {
        return NEGATIVE_SQRT_ELEM;
    }

    what_elem = sqrt((proc -> ram)[what_elem]);
    stack_push(proc -> stk, what_elem);



    if (!processor_ok(proc))
    {
        processor_dump(proc);
        assert(!"Processor_fail(do_sqrt_ram_command)");
    }
    return SUCCESS;
}


int do_jmp(struct processor* proc, LLU* ind, LLU num_commands)
// do command jmp, return code of error.
{
    if (!processor_ok(proc))
    {
        processor_dump(proc);
        assert(!"Processor_fail(do_jmp_command)");
    }    
    assert(ind);
    assert(num_commands >= 0);

    
    LLU tmp_ind = stack_pop(proc -> stk);
    if (tmp_ind < 0 || tmp_ind >= num_commands)
    {
        return INCORRECT_LABEL_JMP;
    }
    *ind = tmp_ind;


    if (!processor_ok(proc))
    {
        processor_dump(proc);
        assert(!"Processor_fail(do_jmp_command)");
    }
    return SUCCESS;
}


int do_ja(struct processor* proc, LLU* ind, LLU num_commands)
// do command ja, return code of error.
{
    if (!processor_ok(proc))
    {
        processor_dump(proc);
        assert(!"Processor_fail(do_ja_command)");
    }    
    assert(ind);
    assert(num_commands >= 0);

    
    LLU tmp_ind = stack_pop(proc -> stk);
    if (tmp_ind < 0 || tmp_ind >= num_commands)
    {
        return INCORRECT_LABEL_JA;
    }
    if ((proc -> stk) -> count <= 1)
    {
        return EMERGENCY_STOP;
    }


    double second_elem = stack_pop(proc -> stk);
    double first_elem = stack_pop(proc -> stk);
    if (first_elem > second_elem)
    {    
        *ind = tmp_ind;
    }


    if (!processor_ok(proc))
    {
        processor_dump(proc);
        assert(!"Processor_fail(do_ja_command)");
    }
    return SUCCESS;
}


int do_jb(struct processor* proc, LLU* ind, LLU num_commands)
// do command jb, return code of error.
{
    if (!processor_ok(proc))
    {
        processor_dump(proc);
        assert(!"Processor_fail(do_jb_command)");
    }    
    assert(ind);
    assert(num_commands >= 0);

    
    LLU tmp_ind = stack_pop(proc -> stk);
    if (tmp_ind < 0 || tmp_ind >= num_commands)
    {
        return INCORRECT_LABEL_JB;
    }
    if ((proc -> stk) -> count <= 1)
    {
        return EMERGENCY_STOP;
    }


    double second_elem = stack_pop(proc -> stk);
    double first_elem = stack_pop(proc -> stk);
    if (first_elem < second_elem)
    {    
        *ind = tmp_ind;
    }


    if (!processor_ok(proc))
    {
        processor_dump(proc);
        assert(!"Processor_fail(do_jb_command)");
    }
    return SUCCESS;
}


int do_jc(struct processor* proc, LLU* ind, LLU num_commands)
// do command jc, return code of error.
{
    if (!processor_ok(proc))
    {
        processor_dump(proc);
        assert(!"Processor_fail(do_jc_command)");
    }    
    assert(ind);
    assert(num_commands >= 0);

    
    LLU tmp_ind = stack_pop(proc -> stk);
    if (tmp_ind < 0 || tmp_ind >= num_commands)
    {
        return INCORRECT_LABEL_JC;
    }
    if ((proc -> stk) -> count <= 1)
    {
        return EMERGENCY_STOP;
    }


    double second_elem = stack_pop(proc -> stk);
    double first_elem = stack_pop(proc -> stk);
    if (first_elem == second_elem)
    {    
        *ind = tmp_ind;
    }


    if (!processor_ok(proc))
    {
        processor_dump(proc);
        assert(!"Processor_fail(do_jc_command)");
    }
    return SUCCESS;
}


int do_jae(struct processor* proc, LLU* ind, LLU num_commands)
// do command jae, return code of error.
{
    if (!processor_ok(proc))
    {
        processor_dump(proc);
        assert(!"Processor_fail(do_jae_command)");
    }    
    assert(ind);
    assert(num_commands >= 0);

    
    LLU tmp_ind = stack_pop(proc -> stk);
    if (tmp_ind < 0 || tmp_ind >= num_commands)
    {
        return INCORRECT_LABEL_JAE;
    }
    if ((proc -> stk) -> count <= 1)
    {
        return EMERGENCY_STOP;
    }


    double second_elem = stack_pop(proc -> stk);
    double first_elem = stack_pop(proc -> stk);
    if (first_elem >= second_elem)
    {    
        *ind = tmp_ind;
    }


    if (!processor_ok(proc))
    {
        processor_dump(proc);
        assert(!"Processor_fail(do_jae_command)");
    }
    return SUCCESS;
}


int do_jbe(struct processor* proc, LLU* ind, LLU num_commands)
// do command jbe, return code of error.
{
    if (!processor_ok(proc))
    {
        processor_dump(proc);
        assert(!"Processor_fail(do_jbe_command)");
    }    
    assert(ind);
    assert(num_commands >= 0);

    
    LLU tmp_ind = stack_pop(proc -> stk);
    if (tmp_ind < 0 || tmp_ind >= num_commands)
    {
        return INCORRECT_LABEL_JBE;
    }
    if ((proc -> stk) -> count <= 1)
    {
        return EMERGENCY_STOP;
    }


    double second_elem = stack_pop(proc -> stk);
    double first_elem = stack_pop(proc -> stk);
    if (first_elem <= second_elem)
    {    
        *ind = tmp_ind;
    }


    if (!processor_ok(proc))
    {
        processor_dump(proc);
        assert(!"Processor_fail(do_jbe_command)");
    }
    return SUCCESS;
}


int do_jne(struct processor* proc, LLU* ind, LLU num_commands)
// do command jne, return code of error.
{
    if (!processor_ok(proc))
    {
        processor_dump(proc);
        assert(!"Processor_fail(do_jne_command)");
    }    
    assert(ind);
    assert(num_commands >= 0);

    
    LLU tmp_ind = stack_pop(proc -> stk);
    if (tmp_ind < 0 || tmp_ind >= num_commands)
    {
        return INCORRECT_LABEL_JNE;
    }
    if ((proc -> stk) -> count <= 1)
    {
        return EMERGENCY_STOP;
    }


    double second_elem = stack_pop(proc -> stk);
    double first_elem = stack_pop(proc -> stk);
    if (first_elem != second_elem)
    {    
        *ind = tmp_ind;
    }


    if (!processor_ok(proc))
    {
        processor_dump(proc);
        assert(!"Processor_fail(do_jne_command)");
    }
    return SUCCESS;
}


int do_add(struct processor* proc)
// do command add, return code of error.
{
    if (!processor_ok(proc))
	{
        processor_dump(proc);
        assert(!"Processor_fail(do_add_command)");
	}
    if ((proc -> stk) -> count <= 1)
    {
        return EMERGENCY_STOP;
    }
 

    double second = stack_pop(proc -> stk);
    double first = stack_pop(proc -> stk);
    double sum = first + second;
    stack_push(proc -> stk, sum);


    if (!processor_ok(proc))
	{
        processor_dump(proc);
        assert(!"Processor_fail(do_add_command)");
	}
    return SUCCESS;
}


int do_mul(struct processor* proc)
// do command mul, return code of error.
{
    if (!processor_ok(proc))
	{
        processor_dump(proc);
        assert(!"Processor_fail(do_mul_command)");
	}
    if ((proc -> stk) -> count <= 1)
    {
        return EMERGENCY_STOP;
    }


    double second = stack_pop(proc -> stk);
    double first = stack_pop(proc -> stk);
    double composition = first * second;
    stack_push(proc -> stk, composition);


    if (!processor_ok(proc))
	{
        processor_dump(proc);
        assert(!"Processor_fail(do_mul_command)");
	}
    return SUCCESS;
}


int do_sub(struct processor* proc)
// do command sub, return code of error.
{
    if (!processor_ok(proc))
	{
        processor_dump(proc);
        assert(!"Processor_fail(do_sub_command)");
	}
    if ((proc -> stk) -> count <= 1)
    {
        return EMERGENCY_STOP;
    }
 

    double second = stack_pop(proc -> stk);
    double first = stack_pop(proc -> stk);
    double difference = first - second;
    stack_push(proc -> stk, difference);


    if (!processor_ok(proc))
	{
        processor_dump(proc);
        assert(!"Processor_fail(do_sub_command)");
	}
    return SUCCESS;
}


int do_div(struct processor* proc)
// do command div, return code of error.
{
    if (!processor_ok(proc))
    {
        processor_dump(proc);
        assert(!"Processor_fail(do_div_command)");
    }
    if ((proc -> stk) -> count <= 1)
    {
        return EMERGENCY_STOP;
    }
 

    double second = stack_pop(proc -> stk);
    double first = stack_pop(proc -> stk);
    if (second == 0)
    {
        return NULL_FAILURE;
    }
    double division = first / second;
    stack_push(proc -> stk, division);


    if (!processor_ok(proc))
    {
        processor_dump(proc);
        assert(!"Processor_fail(do_div_command)");
    }
    return SUCCESS;
}


int do_out(struct processor* proc)
// do command sub, return code of error.
{
    if (!processor_ok(proc))
	{
        processor_dump(proc);
        assert(!"Processor_fail(do_out_command)");
	}
    if ((proc -> stk) -> count <= 0)
    {
        return EMERGENCY_STOP;
    }


    double elem = stack_pop(proc -> stk);
    printf("%g\n", elem);


    if (!processor_ok(proc))
	{
        processor_dump(proc);
        assert(!"Processor_fail(do_out_command)");
	}
    return SUCCESS;
}


int do_get(struct processor* proc)
// do command sub, return code of error.
{
    if (!processor_ok(proc))
    {
        processor_dump(proc);
        assert(!"Processor_fail(do_get_command)");
    }
 

    printf("Enter number!\n");
    double number = -666;
    int code_scanf = scanf("%lf", &number);
    while (code_scanf != 1)
    {
        printf("You enter incorrect number!!!\n");
        code_scanf = scanf("%lf", &number);    
    }
    stack_push(proc -> stk, number);
    

    if (!processor_ok(proc))
    {
        processor_dump(proc);
        assert(!"Processor_fail(do_get_command)");
    }
    return SUCCESS;
}


int do_end(struct processor* proc)
// Free all patterns that was used.
{
    if (!processor_ok(proc))
	{
        processor_dump(proc);
        assert(!"Processor_fail(do_end_command)");
	}


    free((proc -> stk) -> data);
    free(proc -> stk);
    free(proc);


    return SUCCESS;
}

