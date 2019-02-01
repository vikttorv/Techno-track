// processor.cpp (breif: proc.cpp)  V.T.1.0
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "processor.cpp"


enum errors 
{
    INCORRECT_WAY = 1,
    STAT_ERROR,
    READ_ERROR,
    STRANGE_COMMAND
};


double* get_code(char* way_command_file, LLU* num_commands_ptr);
int do_code(double* command_array, LLU num_commands);
int do_command(struct processor* proc, double* command_array, LLU* ind, LLU num_commands);
int prepare_stack(struct processor* proc, int times, double* command_array, LLU* ind);
int printf_errors(int code_error);


int main(int argc, char* argv[ ])
{
    if (argc <= 1)
    {
        printf("You didn't enter name of file with compiled code!!!\n");
        return EXIT_FAILURE;
    }
    

    double* command_array = nullptr;
    LLU num_commands = 0;
    command_array = get_code(argv[1], &num_commands);
    int errno_tmp = errno;
    if (errno_tmp != SUCCESS)
    {
    	return printf_errors(errno_tmp);
    }


    return printf_errors(do_code(command_array, num_commands));
}


double* get_code(char* way_command_file, LLU* num_commands_ptr)
{
    assert(way_command_file != nullptr);
    assert(num_commands_ptr != nullptr);


    int code_error = 0xBEDA;
    FILE* command_file = fopen(way_command_file, "rb");
    if (command_file == nullptr)
    {
    	fclose(command_file);
    	errno = INCORRECT_WAY;
    	return nullptr;
    }


    struct stat buf = {0};
    code_error = stat(way_command_file, &buf);
    if (code_error == -1)
    {
        errno = STAT_ERROR;
        return nullptr;
    }
    LLU num_commands = buf.st_size / sizeof(double);


    double* command_array = (double*)calloc(num_commands, sizeof(double));
    LLU real_volume = 0xBEDA;
    real_volume = fread(command_array, sizeof(double), num_commands, command_file);
    if (real_volume != num_commands)
    {
    	fclose(command_file);
    	errno = READ_ERROR;
    	return nullptr;
    }

    *num_commands_ptr = num_commands;
    fclose(command_file);
    errno = SUCCESS;
    return command_array;
}


int do_code(double* command_array, LLU num_commands)
{
	assert(command_array);
    assert(num_commands >= 0);
    struct processor* proc = get_proc( );
    if (proc == nullptr)
    {
        int errno_value = errno;
        return errno_value;
    }


    int code_error = 0xBEDA;
    LLU ind = 0;
    while (command_array[ind] != 9)
    {
        if ((code_error = do_command(proc, command_array, &ind, num_commands)) != SUCCESS)
        {
        	return code_error;
        }
    }

    
    free(command_array);
    return do_end(proc);
}


int do_command(struct processor* proc, double* command_array, LLU* ind, LLU num_commands)
// do command and return code of error.
{
    bool is_ok = (int)processor_ok(proc);
    assert(is_ok);
	assert(command_array);
    assert(num_commands >= 0);
	assert(ind);
    

    switch((int)command_array[*ind])
    {
    	case PUSH:
    	    prepare_stack(proc, 2, command_array, ind);
            return do_push(proc);
        case PUSH_RAM:
            prepare_stack(proc, 2, command_array, ind);
            return do_push_ram(proc);
        case POP_REGSTR:
            prepare_stack(proc, 1, command_array, ind);
            return do_pop_regstr(proc);
        case POP_RAM:
            prepare_stack(proc, 2, command_array, ind);
            return do_pop_ram(proc);
        case ADD:
            prepare_stack(proc, 0, command_array, ind);
            return do_add(proc);
        case MUL:
            prepare_stack(proc, 0, command_array, ind);
            return do_mul(proc);
        case SUB:
            prepare_stack(proc, 0, command_array, ind);
            return do_sub(proc);
        case DIV:
            prepare_stack(proc, 0, command_array, ind);
            return do_div(proc);
        case OUT:
            prepare_stack(proc, 0, command_array, ind);
            return do_out(proc);
        case END:
            return SUCCESS;
        case SQRT:
            prepare_stack(proc, 2, command_array, ind);
            return do_sqrt(proc);
        case SQRT_RAM:
            prepare_stack(proc, 1, command_array, ind);
            return do_sqrt_ram(proc);
        case JMP:
            prepare_stack(proc, 1, command_array, ind);
            return do_jmp(proc, ind, num_commands);
        case JA:
            prepare_stack(proc, 1, command_array, ind);
            return do_ja(proc, ind, num_commands);
        case JB:
            prepare_stack(proc, 1, command_array, ind);
            return do_jb(proc, ind, num_commands);
        case JC:
            prepare_stack(proc, 1, command_array, ind);
            return do_jc(proc, ind, num_commands);
        case JAE:
            prepare_stack(proc, 1, command_array, ind);
            return do_jae(proc, ind, num_commands);
        case JBE:
            prepare_stack(proc, 1, command_array, ind);
            return do_jbe(proc, ind, num_commands);
        case JNE:
            prepare_stack(proc, 1, command_array, ind);
            return do_jne(proc, ind, num_commands);
        case GET:
            prepare_stack(proc, 0, command_array, ind);
            return do_get(proc);
        default:
            return STRANGE_COMMAND;
    }
}


int prepare_stack(struct processor* proc, int times, double* command_array, LLU* ind)
// prepare stack to make a command.
{
    bool is_ok = (int)processor_ok(proc);
    assert(is_ok);
	assert(command_array);
	assert(ind);
    assert(times >= 0);


    for (int i = 0; i < times; i++)
    {
        stack_push(proc -> stk, command_array[++(*ind)]);
    }
    (*ind)++;

    return SUCCESS;
}


int printf_errors(int code_error)
{
    switch(code_error)
    {
    	case SUCCESS:
    	    return EXIT_SUCCESS;
        case INCORRECT_WAY:
            printf("You enter incorrect file to do!!!\n");
            return EXIT_FAILURE;
        case STAT_ERROR:
            printf("Error in function stat( )!!!\n");
            return EXIT_FAILURE;
        case READ_ERROR:
            printf("Error in function fread( )!!!\n");
            return EXIT_FAILURE;
        case STRANGE_COMMAND:
            printf("You enter incorrect command!!!\n");
            return EXIT_FAILURE;


        // push errors
        case ERROR_PUSH:
            printf("Strange what_push!!!\n");
            return EXIT_FAILURE;
        case OPERATIVE_IND_NOT_INTEGER_PUSH:
            printf("Construction push[elem] failed because of double value of elem!!!\n");
            return EXIT_FAILURE;
        case OPERATIVE_IND_NOT_INTEGER_RGSTR_PUSH:
            printf("Construction push[register] failed because of the double value of register!!!\n");
            return EXIT_FAILURE;


        // pop errors
        case ERROR_POP:
            printf("Strange what_pop!!!\n");
            return EXIT_FAILURE;
        case OPERATIVE_IND_NOT_INTEGER_POP:
            printf("Construction pop[elem] failed because of double value of elem!!!\n");
            return EXIT_FAILURE;
        case OPERATIVE_IND_NOT_INTEGER_RGSTR_POP:
            printf("Construction pop[register] failed because of the double value of register!!!\n");
            return EXIT_FAILURE;


        // other errors    
        case GET_STACK_ERROR:
            printf("Error of creating processor!!!\n");
            return EXIT_FAILURE;
        case NEGATIVE_SQRT_ELEM:https://github.com/Darkness11111/Ded/wiki
            printf("Elem of comand sqrt is negative!!!\n");
            return EXIT_FAILURE;
        case EMERGENCY_STOP:
            printf("Incorrect logic of program!!!\n");        
            return EXIT_FAILURE;
        case NULL_FAILURE:
            printf("Attempt of division on zero!!!\n");        
            return EXIT_FAILURE;


        // jump errors
        case INCORRECT_LABEL_JMP:
            printf("Too large or negative label for jmp!!!\n");
            return EXIT_FAILURE;
        case INCORRECT_LABEL_JA:
            printf("Too large or negative label for ja!!!\n");
            return EXIT_FAILURE;
        case INCORRECT_LABEL_JB:
            printf("Too large or negative label for jb!!!\n");
            return EXIT_FAILURE;
        case INCORRECT_LABEL_JC:
            printf("Too large or negative label for jc!!!\n");
            return EXIT_FAILURE;
        case INCORRECT_LABEL_JAE:
            printf("Too large or negative label for jae!!!\n");
            return EXIT_FAILURE;
        case INCORRECT_LABEL_JBE:
            printf("Too large or negative label for jbe!!!\n");
            return EXIT_FAILURE;
        case INCORRECT_LABEL_JNE:
            printf("Too large or negative label for jne!!!\n");
            return EXIT_FAILURE;
        default:
            printf("Strange_error!!!\n");
            return EXIT_FAILURE;

    }
}
